#include "DeployMng.h"
#include "Gimmick/Hurricane.h"
#include "../tmx/MapObject.h"
#include "../SceneMng.h"
#include "../../common/FontMng.h"
#include "../../common/ImageMng.h"
#include "../../common/Collision.h"
#include "../../common/Easing.h"
#include "../tmx/TmxObj.h"
#include "../Obj/RouteCtl.h"
#include "../Obj/Plant/KnightPlant.h"
#include "../Obj/Plant/WarriorPlant.h"
#include "../Obj/Plant/ArcherPlant.h"
#include "../SystemObj/WarSituationGraph.h"
#include "../SystemObj/InputString.h"
#include "../SystemObj/Camera.h"
#include "../UI/Circle.h"
#include "../UI/Box.h"

#include "../../_debug/_DebugConOut.h"

//ギミックサイズ
constexpr int GIMMICK_SIZE_X = 120;
constexpr int GIMMICK_SIZE_Y = 120;

//風の分割サイズ
constexpr int WIND_DIVCNT_X = 8;
constexpr int WIND_DIVCNT_Y = 1;

//円移動量
constexpr int CIRCLEMOVE = 100;
//円初期位置
constexpr int DEF_CIRCLE_POS_X = 20 - CIRCLEMOVE;
//円画像サイズ
constexpr int CIRCLE_SIZE_X = 180;
constexpr int CIRCLE_SIZE_Y = 180;
//円反応レンジ
constexpr int CIRCLE_RANGE_X = CIRCLE_SIZE_X + 50;
constexpr int CIRCLE_RANGE_Y = CIRCLE_SIZE_Y + 50;
//円移動時間
constexpr double CIRCLEMOVETIME = 0.8;

//ボックスサイズ
constexpr int BUTTON_SIZE_X = 64;
constexpr int BUTTON_SIZE_Y = 64;

//手持ち表示系
constexpr int DEPLOY_BETWEEN_WIDTH = 100;

//手持ちの表示オフセット
//プラント
constexpr int DEPLOY_PLANT_OFFSET_X = 20;
constexpr int DEPLOY_PLANT_OFFSET_Y = 26;

//ギミック
constexpr int DEPLOY_GIMMICK_OFFSET_X = 20;
constexpr int DEPLOY_GIMMICK_OFFSET_Y = 16;

//フォント
constexpr int DEPLOY_FONT_SIZE = 20;

//実態の宣言
DeployMng* DeployMng::s_Instance = nullptr;

DeployMng::DeployMng(std::list<std::unique_ptr<Obj>>* objlist, int offset)
{
	objList_ = objlist;

	circle_ = new Circle({ DEF_CIRCLE_POS_X ,lpSceneMng.GetViewArea().y_ - offset }, { CIRCLE_SIZE_X,CIRCLE_SIZE_Y });
	box_ = new Box({ 0,lpSceneMng.GetViewArea().y_ - BUTTON_SIZE_Y - offset }, { lpSceneMng.GetViewArea().x_,BUTTON_SIZE_Y + offset });
	box_->SetModelID(lpImageMng.GetID("woodframe02")[0]);

	showboxFlag_ = false;

	gimmickGhMap_[GimmickType::HURRICANE] = lpImageMng.GetID("wind");

	plantGhMap_[ObjType::KNIGHTPLANT] = lpImageMng.GetID("Knightplant")[0];
	plantGhMap_[ObjType::WARRIORPLANT] = lpImageMng.GetID("Warriorplant")[0];
	plantGhMap_[ObjType::ARCHERPLANT] = lpImageMng.GetID("Archerplant")[0];

	grabGimmickFlag_ = false;
	gimType_ = GimmickType::MAX;
	plantType_ = ObjType::MAX;

	nowTime_ = 0.0;
	moveFinishFlag_ = false;
	delayFlag_ = false;

	entryPlantFlag_ = false;
}

DeployMng::~DeployMng()
{
	delete circle_;
	delete box_;
}

void DeployMng::DeployUIFunction(void)
{
	//カーソルが範囲内に来た時出てくる
	auto curPos = lpSceneMng.cursol_->GetPos();
	if (Collision::CheckCircle({ DEF_CIRCLE_POS_X + CIRCLEMOVE,lpSceneMng.GetViewArea().y_ - lpSceneMng.GetOffset() }, CIRCLE_RANGE_X, curPos) &&
		delayFlag_ == false && lpRouteCtl.routeChangeFlag_ == false)
	{
		//範囲内
		if (nowTime_ <= 0)
		{
			nowTime_ = CIRCLEMOVETIME;
		}
	}
	else
	{
		//範囲外
		if (moveFinishFlag_ && !showboxFlag_)
		{
			//戻る
			CircleMoveLeft();
		}
	}

	if (nowTime_ > 0.0 && moveFinishFlag_ == false && delayFlag_ == false)
	{
		//進む
		CircleMoveRight();
	}

	//連続して出てくるのを防止
	if (delayFlag_)
	{
		nowTime_ -= lpSceneMng.GetDeltaTime();
		if (nowTime_ <= 0)
		{
			delayFlag_ = false;
		}
	}

}

void DeployMng::DrawindentGraph(Vector2 pos, Vector2 size, int gh)
{
	DrawExtendGraph(pos.x_, pos.y_, pos.x_ + size.x_, pos.y_ + size.y_, gh, true);
}

void DeployMng::SetEntryFlag(bool flag)
{
	entryPlantFlag_ = flag;
}

bool DeployMng::Init(std::string mapname)
{
	//初期化
	gimList_.clear();

	//マップネームをmap+数字に加工
	mapname.erase(0, mapname.find_last_of("/") + 1);
	mapname.erase(mapname.find_last_of("."), static_cast<int>(mapname.size()));

	auto mapObj = MapObject::GetMapObject(mapname);

	plantSum_[ObjType::ARCHERPLANT] = mapObj.apNum_;
	plantSum_[ObjType::KNIGHTPLANT] = mapObj.kpNum_;
	plantSum_[ObjType::WARRIORPLANT] = mapObj.wpNum_;

	gimmickSum_[GimmickType::HURRICANE] = mapObj.huricaneNum_;

	//施設の場所をまとめて格納
	for (auto& plantpos : lptmxobj.GetAllyPlantPos())
	{
		plantPosList.push_back(plantpos.second);
	}
	for (auto& plantpos : lptmxobj.GetEnemyPlantPos())
	{
		plantPosList.push_back(plantpos.second);
	}

	return true;
}

void DeployMng::UpDate(Vector2 camoffset)
{
	if (lpRouteCtl.routeChangeFlag_)
	{
		return;
	}

	//UIの更新
	circle_->Update();

	//カーソルの座標はオフセット分減算する
	auto curPos = lpSceneMng.cursol_->GetPos();

	if (showboxFlag_ == true)
	{
		//下のボックスからカーソルが外れれば消える
		if (!box_->CheckColition(curPos))
		{
			showboxFlag_ = false;
		}
	}

	if (circle_->CheckColition(curPos))
	{
		//サークルに当たれば下のボックス表示
		showboxFlag_ = true;
	}

	//各ギミックの更新
	for (auto& gimmick : gimList_)
	{
		if (!gimmick->UpDate())
		{
			//ギミックの生存フラグをfalseへ
			gimmick->aliveFlag_ = false;
		}
	}

	//持ちアクション
	GrabFunction(camoffset);

	//下UIの処理
	DeployUIFunction();

	//ギミックの削除
	gimList_.remove_if([](std::unique_ptr<BaseGimmick>& gim) { return gim->aliveFlag_ == false; });
}

void DeployMng::Draw()
{
	for (auto& gimmick : gimList_)
	{
		gimmick->Draw();
	}
}

void DeployMng::DrawDeploy(SharedCamera& camera, bool isTutorial)
{
	//α値
	int alpha = 255;

	//現在のチュートリアルが自分以外 かつオフ状態でなければ薄くする
	if (lpInputString.GetNowPlace() != PLACE::DEPLOY && lpInputString.GetNowPlace() != PLACE::MAX)
	{
		alpha = lpSceneMng.alpha_;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	//円の描画
	circle_->Draw();

	if (showboxFlag_ == true)
	{
		//ボックス表示時
		box_->Draw();
		//ボックス内の描画
		DrawinBox();
	}

	//持っているものを描画
	DrawWhatPlayerHas(camera->GetDrawOffset(), isTutorial);
}

void DeployMng::DrawWhatPlayerHas(Vector2 camOffset, bool isTutorial)
{
	if (grabGimmickFlag_)
	{
		auto blocksize = lpRouteCtl.GetBlockSize();
		auto armyMap = lpWarSituationGraph.GetArmyMap();

		//カーソル自体の座標
		auto curPos = lpSceneMng.cursol_->GetPos();
		//マップ全体から見たカーソルの座標
		auto curMapPos = lpSceneMng.cursol_->GetPos() - Vector2(0, lpSceneMng.GetOffset()) - camOffset;

		//ブロック値へ変換
		Vector2 blockPos = (curMapPos / blocksize) * blocksize;

		//現在の選択ブロックを白く
		DrawBox(blockPos.x_ + camOffset.x_, blockPos.y_ + lpSceneMng.GetOffset(), blockPos.x_ + camOffset.x_ + blocksize, blockPos.y_ + blocksize + +lpSceneMng.GetOffset(), 0xeeeeee, true);

		if (armyMap[(blockPos.y_) / blocksize][blockPos.x_ / blocksize] != SceneMng::Blue)
		{
			//設置範囲外
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 158);
		}

		if (gimType_ == GimmickType::MAX)
		{
			//施設の描画
			DrawGraph(curPos.x_ - blocksize / 2, curPos.y_ - blocksize / 2, plantGhMap_[plantType_], true);
		}
		else
		{
			//ギミックの描画
			DrawGraph(curPos.x_ - GIMMICK_SIZE_X / 2, curPos.y_ - GIMMICK_SIZE_Y / 2, gimmickGhMap_[gimType_][5], true);
		}

		//α値
		int alpha = 255;
		if (isTutorial)
		{
			//チュートリアル時のα値を変える
			alpha = lpSceneMng.alpha_;
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	}
}

void DeployMng::DrawinBox(void)
{
	//下のギミック一覧の表示
	auto boxpos = box_->GetPos();
	int i = 0;

	auto fontHandle = lpFontMng.GetFontSizeHandle(FontType::DEFAULT, DEPLOY_FONT_SIZE);

	//施設表示
	DrawFormatStringToHandle(20, boxpos.y_ + 5, 0xffffff, fontHandle, "[施設]");
	for (auto& plant : plantGhMap_)
	{
		DrawGraph(i * DEPLOY_BETWEEN_WIDTH + DEPLOY_PLANT_OFFSET_X, boxpos.y_ + DEPLOY_PLANT_OFFSET_Y, plant.second, true);
		DrawFormatStringToHandle(i * DEPLOY_BETWEEN_WIDTH + lpRouteCtl.GetBlockSize() - 10, boxpos.y_ + DEPLOY_PLANT_OFFSET_Y + lpRouteCtl.GetBlockSize(), 0xffffff, fontHandle, "x %d", plantSum_[plant.first]);
		i++;
	}

	//ギミック表示
	DrawFormatStringToHandle(static_cast<int>(plantGhMap_.size())* DEPLOY_BETWEEN_WIDTH + 26, boxpos.y_ + 5, 0xffffff, fontHandle, "[ギミック]");
	for(auto gimmick : gimmickGhMap_)
	{
		DrawModiGraph(i * DEPLOY_BETWEEN_WIDTH + DEPLOY_GIMMICK_OFFSET_X + 5, boxpos.y_ + DEPLOY_GIMMICK_OFFSET_Y + 10,
			i * DEPLOY_BETWEEN_WIDTH + DEPLOY_GIMMICK_OFFSET_X + 5 + 84, boxpos.y_ + DEPLOY_GIMMICK_OFFSET_Y + 10,
			i * DEPLOY_BETWEEN_WIDTH + DEPLOY_GIMMICK_OFFSET_X + 5 + 84, boxpos.y_ + DEPLOY_GIMMICK_OFFSET_Y + 10 + 64,
			i * DEPLOY_BETWEEN_WIDTH + DEPLOY_GIMMICK_OFFSET_X + 5, boxpos.y_ + DEPLOY_GIMMICK_OFFSET_Y + 10 + 64, gimmick.second[5], true);
		
		DrawFormatStringToHandle(i * DEPLOY_BETWEEN_WIDTH + 5 + 74, boxpos.y_ + DEPLOY_GIMMICK_OFFSET_Y + 64 + 8, 0xffffff,fontHandle, "x %d", gimmickSum_[gimmick.first]);
		i++;
	}
}

void DeployMng::SetGimmick(GimmickType gimmicktype, Vector2 pos, Vector2 size)
{
	switch (gimmicktype)
	{
	case GimmickType::HURRICANE :
		gimList_.push_back(std::make_unique<Hurricane>(pos,size));
		break;
	default:
		//何もしない
		break;
	}
}

void DeployMng::GrabFunction(Vector2 camoffset)
{
	if (lpRouteCtl.routeChangeFlag_)
	{
		//ルートチェンジ時は除く
		return;
	}

	//持ちアクション
	auto boxpos = box_->GetPos();
	auto blocksize = lpRouteCtl.GetBlockSize();
	
	if (lpSceneMng.cursol_->GetMouseInput(InputType::NOW)[MouseInputID::LEFT])
	{
		//マウス左クリック時
		if (grabGimmickFlag_)
		{
			//持ち状態の場合
			return;
		}
		if (!showboxFlag_)
		{
			//手持ちが表示されていない場合
			return;
		}

		int i = 0;

		//そのマスのオブジェクトをつかみ状態に
		if (Collision::CheckSquare({ i * DEPLOY_BETWEEN_WIDTH + DEPLOY_PLANT_OFFSET_X,boxpos.y_ + DEPLOY_PLANT_OFFSET_Y }, { blocksize,blocksize }, lpSceneMng.cursol_->GetPos()))
		{
			gimType_ = GimmickType::MAX;
			plantType_ = ObjType::KNIGHTPLANT;
			grabGimmickFlag_ = true;
		}

		i++;

		if (Collision::CheckSquare({ i * DEPLOY_BETWEEN_WIDTH + DEPLOY_PLANT_OFFSET_X,boxpos.y_ + DEPLOY_PLANT_OFFSET_Y }, { blocksize,blocksize }, lpSceneMng.cursol_->GetPos()))
		{
			gimType_ = GimmickType::MAX;
			plantType_ = ObjType::WARRIORPLANT;
			grabGimmickFlag_ = true;
		}

		i++;

		if (Collision::CheckSquare({ i * DEPLOY_BETWEEN_WIDTH + DEPLOY_PLANT_OFFSET_X,boxpos.y_ + DEPLOY_PLANT_OFFSET_Y }, { blocksize,blocksize }, lpSceneMng.cursol_->GetPos()))
		{
			gimType_ = GimmickType::MAX;
			plantType_ = ObjType::ARCHERPLANT;
			grabGimmickFlag_ = true;
		}

		i++;

		if (Collision::CheckSquare({ i * DEPLOY_BETWEEN_WIDTH + DEPLOY_GIMMICK_OFFSET_X,boxpos.y_ + DEPLOY_GIMMICK_OFFSET_Y }, { GIMMICK_SIZE_X,GIMMICK_SIZE_Y }, lpSceneMng.cursol_->GetPos()))
		{
			gimType_ = GimmickType::HURRICANE;
			plantType_ = ObjType::MAX;
			grabGimmickFlag_ = true;
		}
	}
	else
	{
		if (!grabGimmickFlag_)
		{
			return;
		}

		//離したとき
		grabGimmickFlag_ = false;

		auto curPos = lpSceneMng.cursol_->GetPos() - Vector2(0, lpSceneMng.GetOffset()) - camoffset;

		if (gimType_ != GimmickType::MAX)
		{
			//離したときにギミック発生
			if (gimmickSum_[gimType_] != 0)
			{
				SetGimmick(GimmickType::HURRICANE, curPos, { GIMMICK_SIZE_X,GIMMICK_SIZE_Y });
				gimmickSum_[gimType_]--;
			}
		}
		else
		{
			auto armyMap = lpWarSituationGraph.GetArmyMap();
			if (plantSum_[plantType_] == 0)
			{
				//残量がない場合
				lpInputString.CreateFadeOutString("所持数が0です");
				return;
			}

			if (lpRouteCtl.costMap_[curPos.y_ / blocksize][curPos.x_ / blocksize] == lpRouteCtl.GetCantMovePoint())
			{
				//設置不可マス
				lpInputString.CreateFadeOutString("そのマスは設置できません");
				return;
			}

			if (armyMap[curPos.y_ / blocksize][curPos.x_ / blocksize] != SceneMng::Blue)
			{
				//自領地でない
				lpInputString.CreateFadeOutString("自領地ではありません");
				return;
			}

			auto allyCorePos = lptmxobj.GetAllyCorePos();
			//コアは1マスずれている
			if ((curPos.x_ / blocksize == allyCorePos.x_ && curPos.y_ / blocksize == allyCorePos.y_ - 1) ||
				(curPos.x_ / blocksize == allyCorePos.x_ && curPos.y_ / blocksize == allyCorePos.y_))
			{
				//味方コアとその上のマス選択時
				lpInputString.CreateFadeOutString("コアの上には設置できません");
				return;
			}

			auto enemyCorePos = lptmxobj.GetEnemyCorePos();
			if ((curPos.x_ / blocksize == enemyCorePos.x_ && curPos.y_ / blocksize == enemyCorePos.y_ - 1) ||
				(curPos.x_ / blocksize == enemyCorePos.x_ && curPos.y_ / blocksize == enemyCorePos.y_))
			{
				//敵コアとその上のマス選択時
				lpInputString.CreateFadeOutString("コアの上には設置できません");
				return;
			}

			for (auto& plant : plantPosList)
			{
				if (curPos / blocksize == plant)
				{
					//初期配置施設の上を選択時
					lpInputString.CreateFadeOutString("そのマスにはすでに設置されています");
					return;
				}
			}
			
			//離したとき施設設置
			entryPlantFlag_ = true;
			plantPosList.push_back(curPos / blocksize);

			entryPlantPos_ = curPos / blocksize * blocksize;

			//カウントを減らす
			plantSum_[plantType_]--;
		}
	}
}

void DeployMng::CircleMoveRight(void)
{
	int amountofmove = static_cast<int>(circle_->GetPos().x_ + Easing::OutQuart(lpSceneMng.GetDeltaTime(), CIRCLEMOVETIME) * CIRCLEMOVE);

	circle_->SetPos({ amountofmove, circle_->GetPos().y_ });

	nowTime_ -= lpSceneMng.GetDeltaTime();

	//移動制限
	if (circle_->GetPos().x_ > DEF_CIRCLE_POS_X + CIRCLEMOVE)
	{
		circle_->SetPos({ DEF_CIRCLE_POS_X + CIRCLEMOVE, circle_->GetPos().y_ });

		nowTime_ = 0.0;

		moveFinishFlag_ = true;
	}
}

void DeployMng::CircleMoveLeft(void)
{
	int amount = abs((int)(Easing::OutQuart(lpSceneMng.GetDeltaTime(), CIRCLEMOVETIME) * CIRCLEMOVE));
	circle_->SetPos({ circle_->GetPos().x_ + (-amount), circle_->GetPos().y_ });

	//移動制限
	if (circle_->GetPos().x_ <= DEF_CIRCLE_POS_X)
	{
		circle_->SetPos({ DEF_CIRCLE_POS_X , circle_->GetPos().y_ });
		delayFlag_ = true;
		nowTime_ = 0.2;
		moveFinishFlag_ = false;
	}
}
