#include <string>
#include "GameScene.h"
#include "Transition/FadeInOut.h"
#include "ResultScene.h"
#include "SceneMng.h"
#include "Deploy/DeployMng.h"
#include "Obj/Unit/Smoke.h"
#include "Obj/Unit/AllUnitHeader.h"
#include "Obj/Plant/AllPlantHeader.h"
#include "Obj/Core.h"
#include "Obj/RouteCtl.h"
#include "Obj/SearchMapCtl.h"
#include "SystemObj/InputString.h"
#include "SystemObj/WarSituationGraph.h"
#include "UI/Circle.h"
#include "UI/Box.h"
#include "tmx/TmxObj.h"
#include "tmx/MapObject.h"
#include "../common/ImageMng.h"
#include "../common/AudioMng.h"
#include "../common/FontMng.h"
#include "../common/Easing.h"

#include "../_debug/_DebugConOut.h"

//コアの周囲何マスを自分のものにするか
constexpr int CORERANGE = 5;

//UI
constexpr int UI_OFFSET_X = 4;
constexpr int UI_OFFSET_Y = 8;
constexpr int UI_SIZE_X = 64;
constexpr int UI_SIZE_Y = 24;

//倍速
//最大倍率
constexpr double MAX_MAG = 3.0;
//一度に上がる値
constexpr double MAG_GOUP_ONCE = 1.0;

//プラント設置時の落下位置オフセット
constexpr int FALL_PLANT_POS_OFFSET = 34;
//落下時間
constexpr double FALL_PLANT_TOTAL_TIME = 0.65;

//土煙
//設置時の土煙発生数
constexpr int TOTAL_SMOKE = 16;

constexpr int CONVERGENCE_SUM = 23;
constexpr int EXPLOSION_SUM = 16;

constexpr double WAIT_TIME = 1.2;
constexpr double CONVERGENCE_TOTAL_ANIM_TIME = 1.5;
constexpr double EXP_TOTAL_ANIM_TIME = 1.6;

constexpr int BLOCK_SIZE_X = 64;
constexpr int BLOCK_SIZE_Y = 64;

//BGMボリューム
constexpr int TUTORIAL_BGM_VOLUME = 130;
constexpr int GAME_BGM_VOLUME = 180;

GameScene::GameScene(const char* mapname, bool isTutorial)
{
	//マップネームをmap+数字に加工
	mapName_ = mapname;
	musicName_ = mapname;

	mapName_.erase(0, mapName_.find_last_of("/") + 1);
	mapName_.erase(mapName_.find_last_of("."), static_cast<int>(mapName_.size()));
	musicName_ = "Resource/Audio/BGM/" + mapName_ + ".mp3";

	//チュートリアル関係初期化
	isTutorial_ = isTutorial;

	//配置クラスの作成
	lpDeployMng.Create(&objList_, lpSceneMng.GetOffset());
	lpDeployMng.Init(mapname);

	Init();
}

GameScene::~GameScene()
{
	//戦況の削除
	lpWarSituationGraph.Destroy();

	//配置クラスの破棄
	lpDeployMng.Destroy();

	//ルートクラスの破棄
	lpRouteCtl.Destroy();

	lpImageMng.DeleteID("Shadow");
	lpImageMng.DeleteID("WarSituation");
	lpImageMng.DeleteID("AllyText");
	lpImageMng.DeleteID("EnemyText");
	lpImageMng.DeleteID("BlinkLight");
	lpImageMng.DeleteID("Convergence");
	lpImageMng.DeleteID("Explosion");
	
	lpAudioMng.DeleteID("KnightAtk");
	lpAudioMng.DeleteID("ArcherAtk");
	lpAudioMng.DeleteID("WarriorAtk");
	lpAudioMng.DeleteID("AttackedCore");
	lpAudioMng.DeleteID("AttackedPlant");
	lpAudioMng.DeleteID("BrokenCore");
	lpAudioMng.DeleteID("BrokenPlant");
	lpAudioMng.DeleteID("RepelArrow");
	lpAudioMng.DeleteID("DeployPlant");
}

bool GameScene::Init()
{
	auto blocksize = lpRouteCtl.GetBlockSize();
	auto tilesize = lptmxobj.GettileSize();
	auto worldarea = lptmxobj.GetworldArea();

	//マップデータをもとにサイズの決定
	lpSceneMng.SetWorldSize(Vector2(worldarea.x_ * tilesize.x_, worldarea.y_ * tilesize.y_));
	//画面サイズ再設定
	offsetGh_ = MakeScreen(lpSceneMng.GetWorldSize().x_, lpSceneMng.GetWorldSize().y_, true);
	//最大ブロック数の設定
	limitNumBlock_ = (worldarea * tilesize) / blocksize;

	//カメラの初期化
	camera_->InitArea();

	//初期配置セット
	SetPlantAndCore();

	//戦況の作成
	lpWarSituationGraph.Create(limitNumBlock_);
	//初期マス目の追加
	lpWarSituationGraph.AddInitSquares(CORERANGE);

	//UI関係の作成
	CreateUI();

	//音楽を作成してた場合止める
	lpAudioMng.StopMusicAll();
	
	if (isTutorial_)
	{
		SetVolumeMusicMem(TUTORIAL_BGM_VOLUME, lpAudioMng.GetID(musicName_));
	}
	else
	{
		SetVolumeMusicMem(GAME_BGM_VOLUME, lpAudioMng.GetID(musicName_));
	}
	//音楽再生
	PlaySoundMem(lpAudioMng.GetID(musicName_), DX_PLAYTYPE_LOOP);

	//メンバ初期化
	deployPlantFunc_ = nullptr;
	isBackGroundLoading_ = false;

	effectFlag_ = false;
	oneFlag_ = false;
	waitEndFlag_ = false;
	convergenceEndFlag_ = false;
	expAnimTime_ = 0.0;
	
	return true;
}

UniqueScene GameScene::UpDate(UniqueScene ownScene)
{
	//カーソルの更新
	lpSceneMng.cursol_->Update();

	//カメラの更新
	camera_->UpDate();

	//オブジェクトの更新 追加 シグナルの作成
	for (auto objitr = objList_.begin(); objitr != objList_.end();)
	{
		//更新
		if (!(*objitr)->UpDate(objList_, camera_))
		{
			//オブジェクト消滅時
			(*objitr)->aliveFlag_ = false;
		}

		//エフェクト
		if ((*objitr)->effectFlag_)
		{
			if (!oneFlag_)
			{
				corePos_ = (*objitr)->GetPos();
				effectFlag_ = true;
				oneFlag_ = true;
			}
		}

		//追加
		if ((*objitr)->GetGenerateFlag())
		{
			//ルートの空チェック
			if ((*objitr)->GetRoute().empty())
			{
				continue;
			}

			//ユニットの生成・追加
			GenerateUnit(objitr);
			(*objitr)->SetGenerateFlag(false);
		}

		//シグナルの追加
		if ((*objitr)->signalFlag_)
		{
			SignalType signalType;
			if ((*objitr)->GetGenerateType() == GenerateType::PLANT)
			{
				signalType = SignalType::ENEMYPLANT;
			}
			else
			{
				if ((*objitr)->GetArmyType() == ArmyType::ALLY)
				{
					signalType = SignalType::ALLYCOREATTACKED;
				}
				else
				{
					signalType = SignalType::ENEMYCOREATTACK;
				}
			}
			
			signalList_.push_back(std::make_unique<Signal>(camera_->GetPos(), (*objitr)->GetPos(), camera_->GetDrawOffset(), signalType));

			(*objitr)->signalFlag_ = false;
		}
		objitr++;
	}

	//オブジェクトの削除
	for (auto objitr = objList_.begin(); objitr != objList_.end();)
	{
		//削除
		if (!(*objitr)->aliveFlag_)
		{
			//コア破壊時
			if ((*objitr)->GetGenerateType() == GenerateType::CORE)
			{
				if ((*objitr)->GetArmyType() == ArmyType::ENEMY)
				{
					lpSceneMng.clearMap_[mapName_] = true;
				}
				return std::make_unique<FadeInOut>(std::move(ownScene), std::make_unique<ResultScene>((*objitr)->GetArmyType()), 1.0);
			}

			//自分のアドレスを他が持っていれば削除
			std::list<UniqueObj>::iterator tmpItr = objList_.begin();
			while (1)
			{
				const auto& checkItr = std::find_if(tmpItr, objList_.end(), [&](UniqueObj& obj) { return obj->enemy_ == &(*(*objitr)); });
				if (checkItr == objList_.end())
				{
					//他のオブジェクト全てが持っていない場合
					break;
				}
				(*checkItr)->enemy_ = nullptr;

				tmpItr = checkItr;
			}

			objitr = objList_.erase(objitr);
			continue;
		}
		objitr++;
	}

	//プラントの追加
	if (lpDeployMng.GetEntryFlag())
	{
		auto blocksize = lpRouteCtl.GetBlockSize();
		//施設が落ちてくる演出終了時設置
		if (deployPlantFunc_ == nullptr)
		{
			auto entryPlantPos = lpDeployMng.GetEntryPlantPos();

			deployPlantFunc_ = new MoveObjectVoidVoid(
				Vector2(entryPlantPos.x_, entryPlantPos.y_ - FALL_PLANT_POS_OFFSET),
				entryPlantPos, MoveType::OUTEXP, FALL_PLANT_TOTAL_TIME,
				lpImageMng.ConvertObjTypeToKey(lpDeployMng.GetPlantType(), ArmyType::ALLY),
				std::bind(&GameScene::DeployPlant, this));
		}
		
		//処理終了時にプラント追加処理を行う
		deployPlantFunc_->Update()();
	}

	//プラント設置時の土煙
	if (!smokeVArray_.empty())
	{
		for (auto smokeItr = smokeVArray_.begin(); smokeItr != smokeVArray_.end();)
		{
			if ((*smokeItr)->Update())
			{
				smokeItr = smokeVArray_.erase(smokeItr);
				continue;
			}
			smokeItr++;
		}
	}

	if (lpRouteCtl.routeChangeEndFlag_)
	{
		//ルート変更を終了
		lpRouteCtl.routeChangeEndFlag_ = false;
	}

	UpDateUI();

	//持ちアクション系の更新
	lpDeployMng.UpDate(camera_->GetDrawOffset());

	return std::move(ownScene);
}

void GameScene::UpDateUI(void)
{
	//マウスの値の取得
	auto mouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::NOW);
	auto oldmouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::OLD);

	auto blocksize = lpRouteCtl.GetBlockSize();

	//シグナルの更新
	for (auto signalItr = signalList_.begin(); signalItr != signalList_.end();)
	{
		if ((*signalItr)->Update(camera_->GetPos(),camera_->GetDrawOffset()))
		{
			//削除
			signalItr = signalList_.erase(signalItr);
			continue;
		}
		signalItr++;
	}

	//ボタン系
	//ルート選択中のUI（ボタン）処理
	if (lpRouteCtl.routeChangeFlag_)
	{
		bool buttonOn = false;
		for (auto& ui : lpRouteCtl.GetUIList())
		{
			//ボタン押下チェック

			//マウス時のチェック
			if (!((mouseInput[MouseInputID::LEFT] && !oldmouseInput[MouseInputID::LEFT]) &&
				(ui->CheckColition(lpSceneMng.cursol_->GetPos()))))
			{
				if (mouseInput[MouseInputID::LEFT] && !oldmouseInput[MouseInputID::LEFT])
				{
					//入力されていれば
					buttonOn = true;
				}

				//パッド時のチェック
				if (!(ui->CheckButtonInput(mouseInput[ui->GetCheckId()], oldmouseInput[ui->GetCheckId()])))
				{
					continue;
				}
			}

			buttonOn = true;
			if (ui->GetFunction()())
			{
				//ルート変更終了時
				//ルートチェンジしていたものを取り出しルート変更
				
				for (const auto& route : lpRouteCtl.GetRouteAll(ArmyType::ALLY))
				{
					for (auto& object : objList_)
					{
						if (object->GetArmyType() == ArmyType::ENEMY)
						{
							continue;
						}
						if (object->GetGenerateType() != GenerateType::PLANT)
						{
							continue;
						}
						if (route.first != object->GetGeneOrder())
						{
							continue;
						}
						if (route.second == object->GetRoute())
						{
							continue;
						}
						object->SetRoute(route.second);
					}
					/*auto routeChangeObj = std::find_if(objList_.begin(), objList_.end(), [&](UniqueObj& obj) { return obj->GetRoute() != route[obj->GetGeneOrder()]; });
					if (routeChangeObj != objList_.end())
					{
						(*routeChangeObj)->SetRoute(lpRouteCtl.GetRoute(ArmyType::ALLY, (*routeChangeObj)->GetGeneOrder()));
						(*routeChangeObj)->SetRouteChangeFlag(false);
					}*/
				}
			}
		}

		//ボタン押しチェック
		if (buttonOn)
		{
			//施設を選択できていないとき　かつ　ルートがリセットされている場合
			if (lpRouteCtl.currentPlantOrder_ == 0 && lpRouteCtl.routeResetFlag_)
			{
				lpInputString.CreateFadeOutString("味方の施設を選択してください");
			}
		}
	}

	//UI処理
	//ルートチェンジ時とルートチェンジ終了は処理しない
	if (!lpRouteCtl.routeChangeFlag_ && !lpRouteCtl.routeChangeEndFlag_)
	{
		//UIリストの当たり判定
		for (auto& ui : btnList_)
		{
			//範囲内チェック
			auto curPos = lpSceneMng.cursol_->GetPos();

			if (ui->CheckColition(lpSceneMng.cursol_->GetPos()))
			{
				//ボタン押下チェック
				if ((mouseInput[MouseInputID::LEFT] && !oldmouseInput[MouseInputID::LEFT]) ||
					(ui->CheckButtonInput(mouseInput[ui->GetCheckId()], oldmouseInput[ui->GetCheckId()])))
				{
					ui->GetFunction()();
				}
			}
		}
	}

	//マウスでの倍速処理
	if (mouseInput[MouseInputID::MIDDLE] != 0 && oldmouseInput[MouseInputID::MIDDLE] == 0)
	{
		if (mouseInput[MouseInputID::MIDDLE] < 0)
		{
			CntDownDoubleElapTime();
		}
		if (0 < mouseInput[MouseInputID::MIDDLE])
		{
			//増加処理
			CntUpDoubleElapTime();
		}
	}
}

void GameScene::Draw(void)
{
	BeforeDraw();

	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	auto BlockSize = lpRouteCtl.GetBlockSize();
	Vector2 camOffset = camera_->GetDrawOffset();

	//後ろの背景描画
	if (!isTutorial_)
	{
		DrawExtendGraph(0, 0, lpSceneMng.GetViewArea().x_, static_cast<int>(lpSceneMng.GetOffset() * 2.0), lpImageMng.GetID(mapName_)[0], true);
	}

	//オフセット分ずらして描画
	//マップの描画
	if (!isBackGroundLoading_)
	{
		if (isTutorial_)
		{
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		//背景をロードしていなければロード
		SaveDrawnTMX();
		if (isTutorial_)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, lpSceneMng.alpha_);
		}
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
	}
	DrawGraph(camOffset.x_, camOffset.y_ + lpSceneMng.GetOffset(), mapGh_, true);

	//戦況の描画
	lpWarSituationGraph.Draw(camOffset);

	//オブジェクト系の描画
	DrawGraph(camOffset.x_, camOffset.y_ + lpSceneMng.GetOffset(), offsetGh_, true);

	//手持ちの描画
	lpDeployMng.DrawDeploy(camera_, isTutorial_);
	//UIの描画
	DrawUI();
	//シグナルの描画
	for (auto& signal : signalList_)
	{
		signal->Draw(isTutorial_);
	}

	DrawBreakedCore();

	//カメラの描画
	camera_->Draw();

	//システムメッセージの描画
	lpInputString.FedeOutDraw();

	//カーソルの描画
	lpSceneMng.cursol_->Draw(isTutorial_);
}

void GameScene::BeforeDraw(void)
{
	//オフセット分ずらして描画
	SetDrawScreen(offsetGh_);
	ClearDrawScreen();

	//ブロックサイズ
	auto BlockSize = lpRouteCtl.GetBlockSize();
	//カメラオフセット
	Vector2 camOffset = camera_->GetDrawOffset();

	//ルート変更中の描画
	DrawRouteChange();

	//土煙描画
	if (!smokeVArray_.empty())
	{
		for (auto& smoke : smokeVArray_)
		{
			smoke->Draw();
		}
	}

	//各オブジェクトの描画
	for (auto& obj : objList_)
	{
		obj->Draw(camera_);
	}

	//施設の落下描画
	if (deployPlantFunc_ != nullptr)
	{
		deployPlantFunc_->Draw();
	}

	//ギミック描画
	lpDeployMng.Draw();
}

void GameScene::DrawUI(void)
{
	//α値
	int alpha = 255;

	//現在のチュートリアルが自分以外だと薄くする
	if (lpInputString.GetNowPlace() != PLACE::BUTTON && lpInputString.GetNowPlace() != PLACE::MAX)
	{
		alpha = lpSceneMng.alpha_;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	if (lpRouteCtl.routeChangeFlag_)
	{
		//ルート変更中の上部の描画
		for (auto& ui : lpRouteCtl.GetUIList())
		{
			//範囲内時枠線表示
			bool frameFlag = false;
			if (ui->CheckColition(lpSceneMng.cursol_->GetPos()))
			{
				frameFlag = true;
			}
			ui->Draw(frameFlag);
		}
	}
	else
	{
		//通常時上部UI
		//UIリストの描画-上の倍速など
		for (auto& ui : btnList_)
		{
			//範囲内時枠線表示
			bool frameFlag = false;
			if (ui->CheckColition(lpSceneMng.cursol_->GetPos()))
			{
				frameFlag = true;
			}
			ui->Draw(frameFlag,20, std::to_string(static_cast<int>(lpSceneMng.doubleValue_)));
		}
	}
}

void GameScene::SaveDrawnTMX(void)
{
	//画面作成
	offsetGh_ = MakeScreen(lpSceneMng.GetWorldSize().x_, lpSceneMng.GetWorldSize().y_, true);
	mapGh_ = 0;

	auto blocksize = lpRouteCtl.GetBlockSize();
	auto worldarea = lptmxobj.GetworldArea();
	auto tilesize = lptmxobj.GettileSize();

	mapGh_ = MakeScreen(worldarea.x_ * blocksize, worldarea.y_ * blocksize, true);

	if (SetDrawScreen(mapGh_) == -1)
	{
		return;
	}
	ClsDrawScreen();

	//現在のレイヤーID
	int id = 0;
	//id - layerIdの最小ナンバー
	int minlayerid = 5000;
	//決定レイヤー
	int decLayer = 0;

	//TMXマップデータ全体
	auto mapdata = lptmxobj.GetMapData();
	//初めのID
	auto firstid = lptmxobj.GetfirstGID();

	//マップデータの描画
	for (auto layer = --mapdata.end(); layer != --mapdata.begin(); layer--)
	{
		for (int y = 0; y < worldarea.y_; y++)
		{
			for (int x = 0; x < worldarea.x_; x++)
			{
				id = lptmxobj.GetMapData(layer->first, x, y);

				//idが0の場合
				if (id == 0)
				{
					continue;
				}

				minlayerid = 5000;

				for (auto layerId : firstid)
				{
					//id-layeridがもっとも小さいものをlayeridとする
					if (id - layerId < minlayerid && 0 <= id - layerId)
					{
						minlayerid = id - layerId;
						decLayer = layerId;
					}
				}
				//ImageMng側が0から始まっているためfistid分引く
				id = id - decLayer;

				DrawGraph(x * tilesize.x_, y * tilesize.y_, lpImageMng.GetID(decLayer)[id], true);
			}
		}
	}
	isBackGroundLoading_ = true;
}

void GameScene::DrawRouteChange(void)
{
	//ルート変更中の描画
	if (lpRouteCtl.routeChangeFlag_)
	{
		//ブロックサイズ
		auto BlockSize = lpRouteCtl.GetBlockSize();
		//現在の選択場所
		auto curPos = lpSceneMng.cursol_->GetPos() - Vector2{ 0, lpSceneMng.GetOffset() } -camera_->GetDrawOffset();

		Vector2 blockPos;
		blockPos.x_ = curPos.x_ / BlockSize * BlockSize;
		blockPos.y_ = curPos.y_ / BlockSize * BlockSize;

		//選択ルートの描画
		lpRouteCtl.DrawRoute(lpRouteCtl.GetCurRoute(), Vector2{ BlockSize , BlockSize }, SceneMng::Blue, lpRouteCtl.currentPlantOrder_);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 178);

		//現在の座標を白く
		DrawBox(blockPos.x_, blockPos.y_, blockPos.x_ + BlockSize, blockPos.y_ + BlockSize, 0xffffff, true);
	}

	//α値
	int alpha = 255;
	if (isTutorial_)
	{
		//チュートリアル時のα値を変える
		alpha = lpSceneMng.alpha_;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
}

void GameScene::DrawBreakedCore(void)
{
	if (!effectFlag_)
	{
		return;
	}

	expAnimTime_ += lpSceneMng.GetDeltaTime();

	if (!waitEndFlag_)
	{
		if (WAIT_TIME < expAnimTime_)
		{
			waitEndFlag_ = true;
			expAnimTime_ = 0.0;
			PlaySoundMem(lpAudioMng.GetID("Convergence"), DX_PLAYTYPE_BACK);
		}
		return;
	}

	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	Vector2 camPos = camera_->GetDrawOffset();
	camPos += Vector2(0, lpSceneMng.GetOffset());

	if (!convergenceEndFlag_)
	{
		int id = max(static_cast<int>(expAnimTime_ / (CONVERGENCE_TOTAL_ANIM_TIME / (CONVERGENCE_SUM - 1))), 0);
		DrawExtendGraph(corePos_.x_ + BLOCK_SIZE_X / 2 - BLOCK_SIZE_X * 3 + camPos.x_, corePos_.y_ - BLOCK_SIZE_Y * 3 - BLOCK_SIZE_Y / 2 + camPos.y_,
			corePos_.x_ + BLOCK_SIZE_X / 2 + BLOCK_SIZE_X * 3 + camPos.x_, corePos_.y_ + BLOCK_SIZE_Y * 3 + camPos.y_,
			lpImageMng.GetID("Convergence")[id], true);

		if (CONVERGENCE_TOTAL_ANIM_TIME < expAnimTime_)
		{
			convergenceEndFlag_ = true;
			expAnimTime_ = 0.0;
			PlaySoundMem(lpAudioMng.GetID("Explosion"), DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		int id = max(static_cast<int>(expAnimTime_ / (EXP_TOTAL_ANIM_TIME / (EXPLOSION_SUM - 1))), 0);
		DrawExtendGraph(corePos_.x_ + BLOCK_SIZE_X / 2 - BLOCK_SIZE_X * 6 + camPos.x_, corePos_.y_ - BLOCK_SIZE_Y * 6 - BLOCK_SIZE_Y / 2 + camPos.y_,
			corePos_.x_ + BLOCK_SIZE_X / 2 + BLOCK_SIZE_X * 6 + camPos.x_, corePos_.y_ + BLOCK_SIZE_Y * 6 + camPos.y_,
			lpImageMng.GetID("Explosion")[id], true);
		if (EXP_TOTAL_ANIM_TIME < expAnimTime_)
		{
			effectFlag_ = false;
			waitEndFlag_ = false;
			convergenceEndFlag_ = false;
			expAnimTime_ = 0.0;
		}
	}

	//α値
	int alpha = 255;
	if (isTutorial_)
	{
		//チュートリアル時のα値を変える
		alpha = lpSceneMng.alpha_;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
}

void GameScene::GenerateUnit(std::list<UniqueObj>::iterator& objitr)
{
	//ブロックサイズ
	auto BlockSize = lpRouteCtl.GetBlockSize();

	auto route = (*objitr)->GetRoute();

	//味方だけ
	if ((*objitr)->GetArmyType() == ArmyType::ALLY)
	{
		//ルートの最終位置にくれば元の位置へ戻る
		//反転ルート
		auto Reverseroute = (*objitr)->GetRoute();
		//末尾は被るため省く
		Reverseroute.pop_back();

		std::reverse(Reverseroute.begin(), Reverseroute.end());

		for (auto point = Reverseroute.begin(); point != Reverseroute.end(); point++)
		{
			route.push_back(*point);
		}
	}

	//ユニットの生成
	if ((*objitr)->GetObjType() == ObjType::KNIGHTPLANT)
	{
		objList_.push_back(std::make_unique<Knight>((*objitr)->GetPos(), Vector2{ BlockSize / 4,BlockSize / 4 }, route, (*objitr)->GetColor(), (*objitr)->GetArmyType(), (*objitr)->GetGeneOrder()));
	}
	if ((*objitr)->GetObjType() == ObjType::WARRIORPLANT)
	{
		objList_.push_back(std::make_unique<Warrior>((*objitr)->GetPos(), Vector2{ BlockSize / 4,BlockSize / 4 }, route, (*objitr)->GetColor(), (*objitr)->GetArmyType(), (*objitr)->GetGeneOrder()));
	}
	if ((*objitr)->GetObjType() == ObjType::ARCHERPLANT)
	{
		objList_.push_back(std::make_unique<Archer>((*objitr)->GetPos(), Vector2{ BlockSize / 4,BlockSize / 4 }, route, (*objitr)->GetColor(), (*objitr)->GetArmyType(), (*objitr)->GetGeneOrder()));
	}
}

void GameScene::SetPlantAndCore(void)
{
	//ブロックサイズの取得
	auto blocksize = lpRouteCtl.GetBlockSize();

	//敵味方施設とコアの位置の取得
	const auto& allyPlantPosVec = lptmxobj.GetAllyPlantPos();
	const auto& enemyPlantPosVec = lptmxobj.GetEnemyPlantPos();
	auto allyCorePos = lptmxobj.GetAllyCorePos();
	auto enemyCorePos = lptmxobj.GetEnemyCorePos();

	//プラントプールの作成
	//味方
	for (auto& allyObjType : allyPlantPosVec)
	{
		if (allyObjType.first == ObjType::KNIGHTPLANT)
		{
			plantAllyPool_.push_back(std::make_unique<KnightPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Blue, ArmyType::ALLY, static_cast<int>(plantAllyPool_.size()) + 1));
		}
		if (allyObjType.first == ObjType::WARRIORPLANT)
		{
			plantAllyPool_.push_back(std::make_unique<WarriorPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Blue, ArmyType::ALLY, static_cast<int>(plantAllyPool_.size()) + 1));
		}
		if (allyObjType.first == ObjType::ARCHERPLANT)
		{
			plantAllyPool_.push_back(std::make_unique<ArcherPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Blue, ArmyType::ALLY, static_cast<int>(plantAllyPool_.size()) + 1));
		}
	}
	//敵
	int cnt = 1;
	for (auto& enemyObjType : enemyPlantPosVec)
	{
		if (enemyObjType.first == ObjType::KNIGHTPLANT)
		{
			plantEnemyPool_.push_back(std::make_unique<KnightPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Red, ArmyType::ENEMY, static_cast<int>(plantEnemyPool_.size()) + 1));
		}
		if (enemyObjType.first == ObjType::WARRIORPLANT)
		{
			plantEnemyPool_.push_back(std::make_unique<WarriorPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Red, ArmyType::ENEMY, static_cast<int>(plantEnemyPool_.size()) + 1));
		}
		if (enemyObjType.first == ObjType::ARCHERPLANT)
		{
			plantEnemyPool_.push_back(std::make_unique<ArcherPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Red, ArmyType::ENEMY, static_cast<int>(plantEnemyPool_.size()) + 1));
		}
	}

	//手持ちもプールに追加
	for (auto deployplant : lpDeployMng.GetDeployPlant())
	{
		for (int i = 0; i < deployplant.second; i++)
		{
			if (deployplant.first == ObjType::KNIGHTPLANT)
			{
				plantAllyPool_.push_back(std::make_unique<KnightPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Blue, ArmyType::ALLY, static_cast<int>(plantAllyPool_.size()) + 1));
			}
			if (deployplant.first == ObjType::WARRIORPLANT)
			{
				plantAllyPool_.push_back(std::make_unique<WarriorPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Blue, ArmyType::ALLY, static_cast<int>(plantAllyPool_.size()) + 1));
			}
			if (deployplant.first == ObjType::ARCHERPLANT)
			{
				plantAllyPool_.push_back(std::make_unique<ArcherPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Blue, ArmyType::ALLY, static_cast<int>(plantAllyPool_.size()) + 1));
			}
		}
	}

	//初期ルートの格納と施設の追加
	//味方
	for (int i = 0; i < static_cast<int>(allyPlantPosVec.size()); i++)
	{
		auto plantPos = allyPlantPosVec[i].second;
		//プラントの生成
		PlantEntryObjList(plantPos * Vector2{ blocksize,blocksize }, lpRouteCtl.GetRoute(ArmyType::ALLY, i + 1), ArmyType::ALLY, allyPlantPosVec[i].first);
		//シグナルの作成
		signalList_.push_back(std::make_unique<Signal>(camera_->GetPos(), plantPos * Vector2{ blocksize,blocksize }, camera_->GetDrawOffset(), SignalType::ALLYPLANT));
	}
	//敵
	for (int i = 0; i < static_cast<int>(enemyPlantPosVec.size()); i++)
	{
		auto plantPos = enemyPlantPosVec[i].second;
		//プラントの生成
		PlantEntryObjList(plantPos * Vector2{ blocksize,blocksize }, lpRouteCtl.GetRoute(ArmyType::ENEMY, i + 1), ArmyType::ENEMY, enemyPlantPosVec[i].first);

		//シグナルの作成
		signalList_.push_back(std::make_unique<Signal>(camera_->GetPos(), plantPos * Vector2{ blocksize,blocksize }, camera_->GetDrawOffset(), SignalType::ENEMYPLANT));
	}

	//コアの生成
	objList_.push_back(std::make_unique<Core>(allyCorePos * Vector2{ blocksize,blocksize }, Vector2{ blocksize,blocksize }, 0x0000ff, ArmyType::ALLY));
	signalList_.push_back(std::make_unique<Signal>(camera_->GetPos(), allyCorePos * Vector2{ blocksize,blocksize }, camera_->GetDrawOffset(), SignalType::ALLYCOREDISPLAY));
	objList_.push_back(std::make_unique<Core>(enemyCorePos * Vector2{ blocksize,blocksize }, Vector2{ blocksize,blocksize }, 0xff0000, ArmyType::ENEMY));
	signalList_.push_back(std::make_unique<Signal>(camera_->GetPos(), enemyCorePos * Vector2{ blocksize,blocksize }, camera_->GetDrawOffset(), SignalType::ENEMYCOREDISPLAY));
}

void GameScene::PlantEntryObjList(Vector2 pos, std::vector<Vector2> route, ArmyType armytype, ObjType objtype)
{
	//一致するものを捜索
	UniqueObj tmpPlant;
	//プールから一致するものを削除しオブジェリストへ
	if (armytype == ArmyType::ALLY)
	{
		//味方
		for (auto plantItr = plantAllyPool_.begin(); plantItr != plantAllyPool_.end();)
		{
			if ((*plantItr)->GetObjType() == objtype && (*plantItr)->GetArmyType() == armytype)
			{
				tmpPlant = std::move(*plantItr);
				plantItr = plantAllyPool_.erase(plantItr);
				break;
			}
			plantItr++;
		}
	}
	else
	{
		//敵
		for (auto plantItr = plantEnemyPool_.begin(); plantItr != plantEnemyPool_.end();)
		{
			if ((*plantItr)->GetObjType() == objtype && (*plantItr)->GetArmyType() == armytype)
			{
				tmpPlant = std::move(*plantItr);
				plantItr = plantEnemyPool_.erase(plantItr);
				break;
			}
			plantItr++;
		}
	}

	//番号を生成順に直す
	int cnt = 1;
	//生成数をカウント
	for (auto& obj : objList_)
	{
		if (obj->GetGenerateType() == GenerateType::PLANT && obj->GetArmyType() == armytype)
		{
			cnt++;
		}
	}

	tmpPlant->SetGeneOrder(cnt);

	//一致したものへ座標とルートを代入
	tmpPlant->InitPlantGenerationFromPool(pos, route);

	objList_.push_front(std::move(tmpPlant));
}

void GameScene::DeployPlant(void)
{
	PlaySoundMem(lpAudioMng.GetID("DeployPlant"), DX_PLAYTYPE_BACK);

	//プラントを配置
	//配置フラグをオフに
	lpDeployMng.SetEntryFlag(false);

	//ムーブオブジェクトを削除
	delete deployPlantFunc_;
	deployPlantFunc_ = nullptr;

	//各座標とサイズの取得
	auto blocksize = lpRouteCtl.GetBlockSize();
	auto entryPlantPos = lpDeployMng.GetEntryPlantPos();
	CreateSmokeWhenDeployingPlant(entryPlantPos);

	//A*でルートを求める
	std::vector<Vector2> tmproute = lpRouteCtl.CreateRoute(entryPlantPos / blocksize, lptmxobj.GetEnemyCorePos(), SceneMng::Blue);

	//プラントをエントリー
	PlantEntryObjList(entryPlantPos, tmproute, ArmyType::ALLY, lpDeployMng.GetPlantType());
}

void GameScene::CreateSmokeWhenDeployingPlant(Vector2 plantPos)
{
	auto blockSize = lpRouteCtl.GetBlockSize();
	for (int i = 0; i < TOTAL_SMOKE; i++)
	{
		smokeVArray_.push_back(new Smoke(Vector2{ plantPos.x_ + GetRand(blockSize), plantPos.y_ + GetRand(blockSize) }, false));
	}
}

void GameScene::CreateUI(void)
{
	//倍速表示
	Vector2 uiSize = { UI_SIZE_X, UI_SIZE_Y };
	btnList_.push_back(std::make_unique<Box>(
		Vector2{ lpSceneMng.GetViewArea().x_ - uiSize.x_ * 3 - UI_OFFSET_X * 2,UI_OFFSET_Y },
		std::move(uiSize),
		std::bind(&GameScene::CntUpDoubleElapTime, this),
		MouseInputID::LEFT,
		"x"));
	btnList_.back()->SetModelID(lpImageMng.GetID("woodframe04")[0]);

	//ルート変更表示
	btnList_.push_back(std::make_unique<Box>(
		Vector2{ lpSceneMng.GetViewArea().x_ - uiSize.x_ * 2 - UI_OFFSET_X,UI_OFFSET_Y },
		std::move(Vector2{ uiSize.x_ * 2 , uiSize.y_ }),
		std::bind(&GameScene::RouteChange, this),
		MouseInputID::LEFT,
		"ルート変更"));
	btnList_.back()->SetModelID(lpImageMng.GetID("woodframe04")[0]);
}

bool GameScene::CntUpDoubleElapTime(void)
{
	//カウントアップ
	lpSceneMng.doubleValue_ += MAG_GOUP_ONCE;
	if (lpSceneMng.doubleValue_ > MAX_MAG)
	{
		//初期値に戻す
		lpSceneMng.doubleValue_ = 1.0;
	}
	return true;
}

bool GameScene::CntDownDoubleElapTime(void)
{
	//カウントダウン
	lpSceneMng.doubleValue_ -= MAG_GOUP_ONCE;
	if (lpSceneMng.doubleValue_ < 1.0)
	{
		//最大に
		lpSceneMng.doubleValue_ = MAX_MAG;
	}
	return true;
}

bool GameScene::RouteChange(void)
{
	//ルート変更フラグをオンにする
	lpRouteCtl.routeChangeFlag_ = true;
	return true;
}

void GameScene::InitFromTutorial(void)
{
	//チュートリアル関係初期化
	isTutorial_ = false;
	lpInputString.SetNowPlace(PLACE::MAX);

	//音楽を作成してた場合止める
	lpAudioMng.StopMusicAll();

	//音楽再生
	SetVolumeMusicMem(GAME_BGM_VOLUME, lpAudioMng.GetID(musicName_));
	PlaySoundMem(lpAudioMng.GetID(musicName_), DX_PLAYTYPE_LOOP);

	//カメラの初期化
	camera_->InitArea();
}

void GameScene::InitMusic(void)
{
	//音楽を作成してた場合止める
	lpAudioMng.StopMusicAll();

	//音楽再生
	if (isTutorial_)
	{
		SetVolumeMusicMem(TUTORIAL_BGM_VOLUME, lpAudioMng.GetID(musicName_));
	}
	else
	{
		//音楽再生
		SetVolumeMusicMem(GAME_BGM_VOLUME, lpAudioMng.GetID(musicName_));
	}
	PlaySoundMem(lpAudioMng.GetID(musicName_), DX_PLAYTYPE_LOOP);
}
