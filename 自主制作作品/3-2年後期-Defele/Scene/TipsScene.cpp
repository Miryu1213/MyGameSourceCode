#include <DxLib.h>
#include <cmath>
#include <memory>
#include "TipsScene.h"
#include "CountDownScene.h"
#include "GameScene.h"
#include "TutorialScene.h"
#include "Transition/FadeInOut.h"
#include "Transition/FadeOut.h"
#include "Obj/RouteCtl.h"
#include "tmx/TmxObj.h"
#include "tmx/MapObject.h"
#include "../common/ImageMng.h"
#include "../common/AudioMng.h"
#include "../common/FontMng.h"

//「TIPS」
constexpr int TIPS_OFFSET_Y = 220;

//TIPSの内容
constexpr int CONTENT_TIPS_OFFSET_Y = 340;
constexpr int CONTENT_TIPS_SIZE_Y = 70;

//「―」
constexpr int DASH_OFFSET_Y = 10;

//下部テキスト
constexpr int TEXT_NOWLOADING_OFFSET_X = 304;
constexpr int TEXT_NOWLOADING_OFFSET_Y = 46;

constexpr int DOT_OFFSET_X = 50;
constexpr int DOT_OFFSET_Y = 22;

constexpr int DOT_SIZE_X = 10;

//一つの点の表示時間
constexpr double ONE_DOT_TIME = 0.8;
//表示ポイント数
constexpr int ONE_DOT_SUM = 3;

//改行文字数
constexpr size_t NEWLINE_NUMBER = 19;

//フォント
constexpr int TIPS_FONT_SIZE = 40;
constexpr int CONTENT_TIPS_FONT_SIZE = 26;

constexpr double BACKLIGHT_TIME = 0.1;
constexpr int BACKLIGHT_SUM = 40;

constexpr double CLICK_LIGHT_TIME = 0.04;
constexpr int CLICK_LIGHT_SUM = 14;

constexpr int CLICK_LIGHT_SIZE_X = 300;
constexpr int CLICK_LIGHT_SIZE_Y = 180;

//フェード時間
constexpr double FADE_TIME = 0.6;

TipsScene::TipsScene(std::string mapName)
{
	mapName_ = mapName;

	//数字だけに変換
	mapName.erase(0, mapName.find_first_of("p") + 1);
	mapName.erase(mapName.find_first_of("."), mapName.size());

	mapNumber_ = atoi(mapName.c_str());

	Init();
}

TipsScene::~TipsScene()
{
	if (routeThread_.joinable())
	{
		routeThread_.join();
	}
	
	lpImageMng.DeleteID("NowLoading");
	lpImageMng.DeleteID("PushTheScreen");
	lpImageMng.DeleteID("dot");
	lpImageMng.DeleteID("BackLight");
	lpImageMng.DeleteID("OneBackLight");

	lpAudioMng.DeleteID("ClickStar");
	lpAudioMng.DeleteID("EndLoading");
}

bool TipsScene::Init(void)
{
	//メンバ初期化
	loadingTime_ = 0.0;
	backLightTime_ = 0.0;
	clickLightTime_ = 0.0;

	alpha_ = 0.0;
	IsFade_ = false;

	endLoadingFlag_ = false;
	oneRingFlag_ = false;
	clickLightFlag_ = false;

	//初期画面
	curScreen_ = CURSCREEN::START;

	//音楽を作成してた場合止める
	lpAudioMng.StopMusicAll();

	//ルートクラスの作成
	lpRouteCtl.Create();

	//TMXロード
	lptmxobj.LoadTMX(mapName_);

	auto blocksize = lpRouteCtl.GetBlockSize();
	auto tilesize = lptmxobj.GettileSize();
	auto worldarea = lptmxobj.GetworldArea();

	//マップの初期化
	lpRouteCtl.InitMap(worldarea * tilesize / blocksize);

	CreateCostMap();

	//別スレッドでルートの作成
	routeThread_ = std::thread([this] { InitByThread(); });

	lpAudioMng.GetID("Resource/Audio/SE/Unit/AttackedCore.mp3", "AttackedCore");
	lpAudioMng.GetID("Resource/Audio/SE/Unit/AttackedPlant.mp3", "AttackedPlant");
	lpAudioMng.GetID("Resource/Audio/SE/Unit/BrokenCore.mp3", "BrokenCore");
	lpAudioMng.GetID("Resource/Audio/SE/Unit/BrokenPlant.mp3", "BrokenPlant");

	lpAudioMng.GetID("Resource/Audio/SE/ClickStar.mp3", "ClickStar");
	lpAudioMng.GetID("Resource/Audio/SE/EndLoading.mp3", "EndLoading");

	return true;
}

UniqueScene TipsScene::UpDate(UniqueScene ownScene)
{
	//カーソルの更新
	lpSceneMng.cursol_->Update();

	if (endLoadingFlag_)
	{
		if (!oneRingFlag_)
		{
			PlaySoundMem(lpAudioMng.GetID("EndLoading"), DX_PLAYTYPE_BACK);
			oneRingFlag_ = true;
		}

		if (lpSceneMng.cursol_->GetMouseInput(InputType::NOW)[MouseInputID::LEFT])
		{
			if (!clickLightFlag_)
			{
				PlaySoundMem(lpAudioMng.GetID("ClickStar"), DX_PLAYTYPE_BACK);
				clickLightFlag_ = true;
			}
		}
	}

	//NOWLOADING
	loadingTime_ += lpSceneMng.GetDeltaTime();
	if (ONE_DOT_TIME * (ONE_DOT_SUM + 1) < loadingTime_)
	{
		loadingTime_ = 0.0;
	}

	//バックライト
	backLightTime_ += lpSceneMng.GetDeltaTime();
	if (BACKLIGHT_TIME * BACKLIGHT_SUM < backLightTime_)
	{
		backLightTime_ = 0.0;
	}

	//クリックライト
	if (clickLightFlag_)
	{
		clickLightTime_ += lpSceneMng.GetDeltaTime();
		if (CLICK_LIGHT_TIME * CLICK_LIGHT_SUM < clickLightTime_)
		{
			//ゲームシーンへ
			if (clickLightFlag_)
			{
				nextScene_->InitMusic();
				return std::make_unique<FadeInOut>(std::move(ownScene), std::move(nextScene_), FADE_TIME);
			}
		}
	}

	return ownScene;
}

void TipsScene::Draw(void)
{
	//バイト数の都合上2倍する
	int newLineNum = NEWLINE_NUMBER * 2;
	//「TIPS」の描画
	auto tipsHandle = lpFontMng.GetFontSizeHandle(FontType::DEFAULT, TIPS_FONT_SIZE);
	int tipsWidth = GetDrawStringWidthToHandle("TIPS", static_cast<int>(strlen("TIPS")), tipsHandle);
	DrawFormatStringToHandle((lpSceneMng.GetViewArea().x_ - tipsWidth) / 2, TIPS_OFFSET_Y, 0xffffff, tipsHandle, "TIPS");

	std::string tipsText = lpTips.GetTip(mapNumber_);
	
	std::vector<std::string> tipsVArray;
	//チップスの改行用切り取り
	for (;;)
	{
		if (tipsText.size() < newLineNum)
		{
			tipsVArray.push_back(tipsText);
			break;
		}
		//NEWLINE_NUMBER分切り取る
		tipsVArray.push_back(tipsText.substr(0, min(newLineNum, tipsText.size())));
		tipsText.erase(0, min(newLineNum, tipsText.size()));
	}

	//TIPS内容の描画
	auto tipsTextHandle = lpFontMng.GetFontSizeHandle(FontType::DEFAULT, CONTENT_TIPS_FONT_SIZE);

	//高さの取得
	int height = 0;
	GetDrawFormatStringSizeToHandle(nullptr, &height, nullptr, tipsTextHandle, tipsVArray.front().c_str());

	std::string dash;
	for (int i = 0; i < newLineNum * 2 / 3; i++)
	{
		//―の作成
		dash += "―";
	}
	int dashWidth = GetDrawStringWidthToHandle(dash.c_str(), static_cast<int>(strlen(dash.c_str())), tipsTextHandle);

	for (int i = 0; i < static_cast<int>(tipsVArray.size()); i++)
	{
		DrawFormatStringToHandle((lpSceneMng.GetViewArea().x_ - dashWidth) / 2,
			CONTENT_TIPS_OFFSET_Y + CONTENT_TIPS_SIZE_Y * i - CONTENT_TIPS_SIZE_Y / 2,
			0xffffff, tipsTextHandle, dash.c_str());

		int tipsTextWidth = GetDrawStringWidthToHandle(tipsVArray[i].c_str(), static_cast<int>(strlen(tipsVArray[i].c_str())), tipsTextHandle);
		DrawFormatStringToHandle((lpSceneMng.GetViewArea().x_ - tipsTextWidth) / 2, CONTENT_TIPS_OFFSET_Y + CONTENT_TIPS_SIZE_Y * i, 0xffffff, tipsTextHandle, tipsVArray[i].c_str());
	}

	DrawFormatStringToHandle((lpSceneMng.GetViewArea().x_ - dashWidth) / 2,
		CONTENT_TIPS_OFFSET_Y + CONTENT_TIPS_SIZE_Y * static_cast<int>(tipsVArray.size()) - CONTENT_TIPS_SIZE_Y / 2,
		0xffffff, tipsTextHandle, dash.c_str());
	
	if (!endLoadingFlag_)
	{
		//NOWLOADING
		DrawGraph(lpSceneMng.GetViewArea().x_ - TEXT_NOWLOADING_OFFSET_X, lpSceneMng.GetViewArea().y_ - TEXT_NOWLOADING_OFFSET_Y, lpImageMng.GetID("NowLoading")[0], true);
		for (int i = 0; i < ONE_DOT_SUM; i++)
		{
			if (ONE_DOT_TIME * (i + 1) < loadingTime_)
			{
				DrawGraph(lpSceneMng.GetViewArea().x_ - DOT_OFFSET_X + DOT_SIZE_X * i, lpSceneMng.GetViewArea().y_ - DOT_OFFSET_Y, lpImageMng.GetID("dot")[0], true);
			}
		}
	}
	else
	{
		//バックライト
		SetDrawBlendMode(DX_BLENDMODE_ADD, 180);
		int extendRate = 2;
		DrawExtendGraph(lpSceneMng.GetViewArea().x_ - TEXT_NOWLOADING_OFFSET_X * extendRate, lpSceneMng.GetViewArea().y_ - TEXT_NOWLOADING_OFFSET_Y * extendRate,
			lpSceneMng.GetViewArea().x_ + TEXT_NOWLOADING_OFFSET_X , lpSceneMng.GetViewArea().y_ + TEXT_NOWLOADING_OFFSET_Y,
			lpImageMng.GetID("BackLight")[static_cast<int>(backLightTime_ / BACKLIGHT_TIME)], true);

		if (IsFade_)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_));
		}
		else
		{
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}

		//PUSHTHESCREEN
		DrawGraph(lpSceneMng.GetViewArea().x_ - TEXT_NOWLOADING_OFFSET_X, lpSceneMng.GetViewArea().y_ - TEXT_NOWLOADING_OFFSET_Y, lpImageMng.GetID("PushTheScreen")[0], true);
	}

	//カーソルの描画
	lpSceneMng.cursol_->Draw(false);

	//クリックライト
	SetDrawBlendMode(DX_BLENDMODE_ADD, 180);
	auto curPos = lpSceneMng.cursol_->GetPos();
	DrawExtendGraph(curPos.x_ - CLICK_LIGHT_SIZE_X / 2, curPos.y_ - CLICK_LIGHT_SIZE_Y / 2,
		curPos.x_ + CLICK_LIGHT_SIZE_X / 2, curPos.y_ + CLICK_LIGHT_SIZE_Y / 2,
		lpImageMng.GetID("OneBackLight")[static_cast<int>(clickLightTime_ / CLICK_LIGHT_TIME)], true);
	if (IsFade_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_));
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}

void TipsScene::SetCantMovePoint(void)
{
	//マップネームをmap+数字に加工
	std::string tmpMapName = mapName_;
	tmpMapName.erase(0, tmpMapName.find_last_of("/") + 1);
	tmpMapName.erase(tmpMapName.find_last_of("."), static_cast<int>(tmpMapName.size()));
	//マップから移動不可領域IDの設定
	for (auto& dontMovePoint : MapObject::GetMapObject(tmpMapName).dontMoveList_)
	{
		lpRouteCtl.cantMoveTileId_.push_back(dontMovePoint);
	}
}

void TipsScene::CreateCostMap(void)
{
	//移動不可ポイントの作成
	SetCantMovePoint();

	auto blocksize = lpRouteCtl.GetBlockSize();
	auto tilesize = lptmxobj.GettileSize();
	auto worldarea = lptmxobj.GetworldArea();

	//最大ブロック数の設定
	Vector2 limitNumBlock_ = {
		worldarea.x_ * tilesize.x_ / blocksize,
		worldarea.y_ * tilesize.y_ / blocksize };

	//現在のレイヤーID
	int id = 0;
	//id - layerIdの最小ナンバー
	int minlayerid = 5000;
	//決定レイヤー
	int decLayer = 0;

	//マップの初期化
	lpRouteCtl.InitMap(limitNumBlock_);

	//TMXマップデータ全体
	auto mapdata = lptmxobj.GetMapData();
	//初めのID
	auto firstid = lptmxobj.GetfirstGID();

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

				//移動マップの作成
				//一時的に
				lpRouteCtl.CreateCostMap(Vector2(x, y), decLayer, blocksize, tilesize);
			}
		}
	}
}

void TipsScene::InitByThread(void)
{
	InitRoute();

	//次のシーンの作成
	if (mapName_ == "Scene/tmx/map0.tmx")
	{
		nextScene_ = std::make_unique<TutorialScene>(mapName_.c_str(), std::make_unique<GameScene>(mapName_.c_str(), true));
		//音楽を作成してた場合止める
		lpAudioMng.StopMusicAll();
	}
	else
	{
		nextScene_ = std::make_unique<CountDownScene>(std::make_unique<GameScene>(mapName_.c_str(), false));
	}

	//ロード終了
	endLoadingFlag_ = true;
}

void TipsScene::InitRoute(void)
{
	//ブロックサイズの取得
	auto blocksize = lpRouteCtl.GetBlockSize();

	//敵味方施設とコアの位置の取得
	const auto& allyPlantPosVec = lptmxobj.GetAllyPlantPos();
	const auto& enemyPlantPosVec = lptmxobj.GetEnemyPlantPos();
	auto allyCorePos = lptmxobj.GetAllyCorePos();
	auto enemyCorePos = lptmxobj.GetEnemyCorePos();

	//初期ルートの格納と施設の追加
	//味方
	for (auto allyPlantPos : allyPlantPosVec)
	{
		//施設生成数 次の値を格納するため+1
		int orderNum = lpRouteCtl.GetRouteSum(ArmyType::ALLY) + 1;
		const auto route = lpRouteCtl.CreateRoute(allyPlantPos.second, enemyCorePos, SceneMng::Blue);

		//ルートの生成
		lpRouteCtl.SetRoute(ArmyType::ALLY, orderNum, route);
	}
	//敵
	for (auto enemyPlantPos : enemyPlantPosVec)
	{
		//施設生成数 次の値を格納するため+1
		auto orderNum = lpRouteCtl.GetRouteSum(ArmyType::ENEMY) + 1;

		//ルートの生成	
		lpRouteCtl.SetRoute(ArmyType::ENEMY, orderNum, lpRouteCtl.CreateRoute(enemyPlantPos.second, allyCorePos, SceneMng::Blue));
	}
}
