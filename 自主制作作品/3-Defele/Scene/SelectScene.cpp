#include "SelectScene.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "TutorialScene.h"
#include "Transition/FadeInOut.h"
#include "CountDownScene.h"
#include "TipsScene.h"
#include "Obj/Unit/UnitDefState.h"
#include "tmx/TmxObj.h"
#include "../common/ImageMng.h"
#include "../common/AudioMng.h"
#include "../common/FontMng.h"
#include "../common/Easing.h"
#include "UI/Box.h"
#include "UI//Circle.h"
#include "UI/Square.h"

#include "../_debug/_DebugConOut.h"

//フォント
constexpr int SELECT_MAP_FONT_SIZE = 20;
constexpr int SELECT_CONFIRM_FONT_SIZE = 30;

//ブロックサイズ
constexpr int BLOCK_SIZE_X = 64;
constexpr int BLOCK_SIZE_Y = 64;

//コアサイズ
constexpr int CORE_SIZE_X = 64;
constexpr int CORE_SIZE_Y = 128;

//マップセレクト
constexpr int MAP_SELECT_POS_Y = 520;
constexpr int MAP_SELECT_SIZE_X = 350;
constexpr int MAP_SELECT_SIZE_Y = 92;
constexpr int MAP_SELECT_INTERVAL = 8;

constexpr int MAP_SELECT_OFFSET_X = 350;
constexpr int MAP_SELECT_OFFSET_INV_X = 80;
constexpr double MAP_SELECT_TOTAL_MOVE_TIME = 0.5;
constexpr double MAP_SELECT_TOTAL_MOVE_TIME_INV = 0.275;

constexpr int AMO_MOVE = 30;

//ステージ
//表示位置
constexpr int STAGE_POS_X = 620;
constexpr int STAGE_POS_Y = 128;
//サイズ
constexpr int STAGE_SIZE_X = 380;
constexpr int STAGE_SIZE_Y = 90;

constexpr int STAGE_SLOPE_SIZE_X = 36;
//ステージ間隔
constexpr int STAGE_INTERVAL_X = 20;
constexpr int STAGE_INTERVAL_Y = 2;

constexpr int STAGE_BG_SIZE_X = 550;
constexpr int STAGE_BG_SIZE_Y = 400;
constexpr int STAGE_BG_SLOPE_SIZE_X = 160;

constexpr int STAGE_TEXT_OFFSET_Y = 100;

//マップ
//表示位置
constexpr int MAP_POS_X = 64;
constexpr int MAP_POS_Y = 74;
//サイズ
constexpr int MAP_SIZE_X = 702;
constexpr int MAP_SIZE_Y = 512;
//マップ拡大縮小最大時間
constexpr double MAX_MAGNI_TIME = 0.4;

constexpr int WINDOW_UI_2_OFFSET_X = 42;
constexpr int WINDOW_UI_2_OFFSET_Y = 42;
constexpr int WINDOW_UI_2_SIZE_X = 54;
constexpr int WINDOW_UI_2_SIZE_Y = 138;

constexpr int WINDOW_UI_3_OFFSET_X = 20;
constexpr int WINDOW_UI_3_OFFSET_Y = 20;

//マップボタン位置
//表示位置
constexpr int MAP_UI_POS_X = MAP_POS_X + MAP_SIZE_X + 80;
constexpr int MAP_UI_POS_Y = MAP_POS_Y + MAP_SIZE_Y;
//サイズ
constexpr int MAP_UI_SIZE_X = 128;
constexpr int MAP_UI_SIZE_Y = 64;

//所持
//表示位置
constexpr int DEPLOY_POS_X = MAP_POS_X;
constexpr int DEPLOY_POS_Y = MAP_POS_Y + MAP_SIZE_Y + 40;
//サイズ
constexpr int DEPLOY_SIZE_X = 896;
constexpr int DEPLOY_SIZE_Y = 52;
//所持施設間隔
constexpr int DEPLOY_INTERVAL = 32;
//所持施設文字間隔
constexpr int DEPLOY_STR_INTERVAL = 4;

//ステータス
constexpr int STATUS_SIZE_X = 444;
constexpr int STATUS_SIZE_Y = 340;

constexpr int STATUS_RELATION_POS_X = 362;
constexpr int STATUS_RELATION_POS_Y = 248;

constexpr int HUR_STATUS_SIZE_X = 444;
constexpr int HUR_STATUS_SIZE_Y = 186;
constexpr int HUR_STATUS_RELATION_POS_X = 420;
constexpr int HUR_STATUS_RELATION_POS_Y = 236;

constexpr double STATUS_INV_TIME = 0.1;

//キャンセル
//表示位置
constexpr int CANCEL_POS_X = 64;
constexpr int CANCEL_POS_Y = 64;
//サイズ
constexpr int CANCEL_SIZE_X = 66;
constexpr int CANCEL_SIZE_Y = 66;

//最終確認UI
//表示位置
constexpr int COMFIRM_UI_POS_X = 384;
constexpr int COMFIRM_UI_POS_Y = 384;
//サイズ
constexpr int COMFIRM_UI_SIZE_X = 124;
constexpr int COMFIRM_UI_SIZE_Y = 64;

//ギミック
//ギミックサイズ
constexpr int GIMMICK_SIZE_X = 120;
constexpr int GIMMICK_SIZE_Y = 120;

constexpr int LOGO_GIM_OFFSET_X = 8;
constexpr int LOGO_GIM_OFFSET_Y = 10;

constexpr int LOGO_GIM_SIZE_X = 24;
constexpr int LOGO_GIM_SIZE_Y = 16;

//風の分割サイズ
constexpr int WIND_DIVCNT_X = 8;
constexpr int WIND_DIVCNT_Y = 1;

//エフェクト
constexpr int BLINKLIGHT_SUM = 30;
constexpr int BACKLIGHT_SUM = 40;
constexpr int ONE_LIGHT_SUM = 16;
constexpr int CONVERGENCE_SUM = 23;
constexpr int EXPLOSION_SUM = 16;

SelectScene::SelectScene()
{
	Init();
}

SelectScene::~SelectScene()
{
	lpImageMng.DeleteID("0");
	//lpImageMng.DeleteID("map0");
	//lpImageMng.DeleteID("map1");
	lpImageMng.DeleteID("KnightStatus");
	lpImageMng.DeleteID("ArcherStatus");
	lpImageMng.DeleteID("WarriorStatus");
	lpImageMng.DeleteID("HurricaneStatus");
	lpImageMng.DeleteID("MapSelectFrame");
	lpImageMng.DeleteID("MapSelectFrameText");
	lpImageMng.DeleteID("MapSelectFrameLight");
	lpImageMng.DeleteID("Triangle");
	lpImageMng.DeleteID("ToTitle");
	lpImageMng.DeleteID("ToTitleText");

	lpAudioMng.DeleteID("select");
}

bool SelectScene::Init(void)
{
	//メンバ初期化
	stgSelectFlag_ = false;
	toTitleFlag_ = false;
	moveEndFlag_ = false;
	defScrEndFlag_ = false;
	mapMagniUpFlag_ = false;
	mapMagniDownFlag_ = false;
	mapMagniTime_ = 0.0;
	comScrFlag_ = false;
	comEndFlag_ = false;
	backGroundGh_ = -1;
	plantSum_ = 0;
	animTime_ = 0.0;
	//アニメーションセット
	SetUnitActiveID();

	//グラフィックハンドルの作成
	mapGh_ = MakeScreen(lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_, false);

	//画像のロード
	LoadImageAll();

	defBtnList_.push_back(std::make_unique<Box>(Vector2(0, MAP_SELECT_POS_Y),
		Vector2(MAP_SELECT_SIZE_X, MAP_SELECT_SIZE_Y),
		[&] {
			stgSelectFlag_ = true;
			return false; },
		MouseInputID::LEFT));
	defBtnList_.back()->SetModelID(lpImageMng.GetID("MapSelectFrame")[0]);

	defBtnList_.push_back(std::make_unique<Box>(Vector2(0, MAP_SELECT_POS_Y + MAP_SELECT_SIZE_Y + MAP_SELECT_INTERVAL),
		Vector2(MAP_SELECT_SIZE_X, MAP_SELECT_SIZE_Y),
		[&] {
			toTitleFlag_ = true;
			return false; },
		MouseInputID::LEFT));
	defBtnList_.back()->SetModelID(lpImageMng.GetID("ToTitle")[0]);

	//ステージの作成
	int stgCnt = 0;
	defStageList_.push_back(std::make_unique<Square>(
		Vector2(STAGE_POS_X, STAGE_POS_Y),
		Vector2(STAGE_POS_X - STAGE_SLOPE_SIZE_X, STAGE_POS_Y + STAGE_SIZE_Y),
		Vector2(STAGE_POS_X + STAGE_SIZE_X, STAGE_POS_Y),
		Vector2(STAGE_POS_X + STAGE_SIZE_X - STAGE_SLOPE_SIZE_X, STAGE_POS_Y + STAGE_SIZE_Y),
		Vector2(STAGE_SIZE_X, STAGE_SIZE_Y),
		"Scene/tmx/map0.tmx",
		MouseInputID::LEFT));
	defStageList_.back()->SetModelID(lpImageMng.GetID("Stage0")[0]);
	stgCnt++;
	if (lpSceneMng.clearMap_.try_emplace("map0").second)
	{
		lpSceneMng.clearMap_["map0"] = false;
	}

	defStageList_.push_back(std::make_unique<Square>(
		Vector2(STAGE_POS_X - (STAGE_SLOPE_SIZE_X)*stgCnt, STAGE_POS_Y + (STAGE_SIZE_Y + STAGE_INTERVAL_X) * stgCnt),
		Vector2(STAGE_POS_X - STAGE_SLOPE_SIZE_X - (STAGE_SLOPE_SIZE_X)*stgCnt, STAGE_POS_Y + STAGE_SIZE_Y + (STAGE_SIZE_Y + STAGE_INTERVAL_X) * stgCnt),
		Vector2(STAGE_POS_X + STAGE_SIZE_X - (STAGE_SLOPE_SIZE_X)*stgCnt, STAGE_POS_Y + (STAGE_SIZE_Y + STAGE_INTERVAL_X) * stgCnt),
		Vector2(STAGE_POS_X + STAGE_SIZE_X - STAGE_SLOPE_SIZE_X - (STAGE_SLOPE_SIZE_X)*stgCnt, STAGE_POS_Y + STAGE_SIZE_Y + (STAGE_SIZE_Y + STAGE_INTERVAL_X) * stgCnt),
		Vector2(STAGE_SIZE_X, STAGE_SIZE_Y),
		"Scene/tmx/map1.tmx",
		MouseInputID::LEFT));
	defStageList_.back()->SetModelID(lpImageMng.GetID("Stage1")[0]);
	stgCnt++;
	if (lpSceneMng.clearMap_.try_emplace("map1").second)
	{
		lpSceneMng.clearMap_["map1"] = false;
	}

	defStageList_.push_back(std::make_unique<Square>(
		Vector2(STAGE_POS_X - (STAGE_SLOPE_SIZE_X)*stgCnt, STAGE_POS_Y + (STAGE_SIZE_Y + STAGE_INTERVAL_X) * stgCnt),
		Vector2(STAGE_POS_X - STAGE_SLOPE_SIZE_X - (STAGE_SLOPE_SIZE_X)*stgCnt, STAGE_POS_Y + STAGE_SIZE_Y + (STAGE_SIZE_Y + STAGE_INTERVAL_X) * stgCnt),
		Vector2(STAGE_POS_X + STAGE_SIZE_X - (STAGE_SLOPE_SIZE_X)*stgCnt, STAGE_POS_Y + (STAGE_SIZE_Y + STAGE_INTERVAL_X) * stgCnt),
		Vector2(STAGE_POS_X + STAGE_SIZE_X - STAGE_SLOPE_SIZE_X - (STAGE_SLOPE_SIZE_X)*stgCnt, STAGE_POS_Y + STAGE_SIZE_Y + (STAGE_SIZE_Y + STAGE_INTERVAL_X) * stgCnt),
		Vector2(STAGE_SIZE_X, STAGE_SIZE_Y),
		"Scene/tmx/map2.tmx",
		MouseInputID::LEFT));
	defStageList_.back()->SetModelID(lpImageMng.GetID("Stage2")[0]);
	stgCnt++;
	if (lpSceneMng.clearMap_.try_emplace("map2").second)
	{
		lpSceneMng.clearMap_["map2"] = false;
	}

	defStageList_.push_back(std::make_unique<Square>(
		Vector2(STAGE_POS_X - (STAGE_SLOPE_SIZE_X)*stgCnt, STAGE_POS_Y + (STAGE_SIZE_Y + STAGE_INTERVAL_X) * stgCnt),
		Vector2(STAGE_POS_X - STAGE_SLOPE_SIZE_X - (STAGE_SLOPE_SIZE_X)*stgCnt, STAGE_POS_Y + STAGE_SIZE_Y + (STAGE_SIZE_Y + STAGE_INTERVAL_X) * stgCnt),
		Vector2(STAGE_POS_X + STAGE_SIZE_X - (STAGE_SLOPE_SIZE_X)*stgCnt, STAGE_POS_Y + (STAGE_SIZE_Y + STAGE_INTERVAL_X) * stgCnt),
		Vector2(STAGE_POS_X + STAGE_SIZE_X - STAGE_SLOPE_SIZE_X - (STAGE_SLOPE_SIZE_X)*stgCnt, STAGE_POS_Y + STAGE_SIZE_Y + (STAGE_SIZE_Y + STAGE_INTERVAL_X) * stgCnt),
		Vector2(STAGE_SIZE_X, STAGE_SIZE_Y),
		"Scene/tmx/map4.tmx",
		MouseInputID::LEFT));
	defStageList_.back()->SetModelID(lpImageMng.GetID("Stage5")[0]);
	stgCnt++;
	if (lpSceneMng.clearMap_.try_emplace("map4").second)
	{
		lpSceneMng.clearMap_["map4"] = false;
	}

	int cnt = 0;
	for (const auto& stage : defStageList_)
	{
		stgSelectPoses_.push_back(stage->GetPos() + Vector2(MAP_SELECT_OFFSET_X + MAP_SELECT_OFFSET_INV_X * cnt, 0));
		moveTimes_.push_back(0.0);
		cnt++;
	}

	/*defStageList_.push_back(std::make_unique<Box>(Vector2(STAGE_POS_X + (STAGE_SIZE_X + STAGE_INTERVAL_X), STAGE_POS_Y),
		Vector2(STAGE_SIZE_X, STAGE_SIZE_Y),
		"Scene/tmx/map1.tmx",
		MouseInputID::LEFT, "1"));
	defStageList_.back()->SetModelID(lpImageMng.GetID("woodframe03")[0]);*/

	//マップUI
	mapUiList_.push_back(std::make_unique<Box>(Vector2(MAP_POS_X, MAP_POS_Y),
		Vector2(MAP_SIZE_X, MAP_SIZE_Y),
		std::bind(&SelectScene::MapDownUI,this),
		MouseInputID::LEFT));

	mapUiList_.push_back(std::make_unique<Box>(Vector2(MAP_UI_POS_X, MAP_UI_POS_Y),
		Vector2(MAP_UI_SIZE_X, MAP_UI_SIZE_Y),
		std::bind(&SelectScene::CancelUI, this),
		MouseInputID::LEFT, "戻る"));
	mapUiList_.back()->SetModelID(lpImageMng.GetID("textbox")[0]);

	mapUiList_.push_back(std::make_unique<Box>(Vector2(MAP_UI_POS_X, MAP_UI_POS_Y + MAP_UI_SIZE_Y + 4),
		Vector2(MAP_UI_SIZE_X, MAP_UI_SIZE_Y),
		std::bind(&SelectScene::SetoffUI, this),
		MouseInputID::LEFT, "出撃"));
	mapUiList_.back()->SetModelID(lpImageMng.GetID("textbox")[0]);

	//キャンセル
	mapMagniUiList_.push_back(std::make_unique<Circle>(Vector2(CANCEL_POS_X, CANCEL_POS_Y),
		Vector2(CANCEL_SIZE_X, CANCEL_SIZE_Y),
		MouseInputID::LEFT));
	mapMagniUiList_.back()->SetModelID(lpImageMng.GetID("ButtonBack")[0]);

	//最終確認
	confirmUiList_.push_back(std::make_unique<Box>(Vector2(COMFIRM_UI_POS_X, COMFIRM_UI_POS_Y),
		Vector2(COMFIRM_UI_SIZE_X, COMFIRM_UI_SIZE_Y),
		std::bind(&SelectScene::CancelUI2, this),
		MouseInputID::LEFT, "いいえ"));
	confirmUiList_.back()->SetModelID(lpImageMng.GetID("woodframe04")[0]);

	confirmUiList_.push_back(std::make_unique<Box>(Vector2(COMFIRM_UI_POS_X + COMFIRM_UI_SIZE_X + 4, COMFIRM_UI_POS_Y),
		Vector2(COMFIRM_UI_SIZE_X, COMFIRM_UI_SIZE_Y),
		std::bind(&SelectScene::GoUI, this),
		MouseInputID::LEFT, "はい"));
	confirmUiList_.back()->SetModelID(lpImageMng.GetID("woodframe04")[0]);

	//音楽を作成してた場合止める
	lpAudioMng.StopMusicAll();

	//音楽再生
	PlaySoundMem(lpAudioMng.GetID("select"), DX_PLAYTYPE_LOOP);
	SetVolumeMusic(180);

	return true;
}

UniqueScene SelectScene::UpDate(UniqueScene ownScene)
{
	TRACE("%d\n", GetASyncLoadNum());
	if (toTitleFlag_)
	{
		if (GetASyncLoadNum() == 0)
		{
			//ロード終了時タイトルへ
			return std::make_unique<FadeInOut>(std::move(ownScene), std::make_unique<TitleScene>(), 1.0);
		}
		TRACE("%d\n", GetASyncLoadNum());
		return ownScene;
	}
	//カーソルの更新
	lpSceneMng.cursol_->Update();

	//マウスの値の取得
	auto mouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::NOW);
	auto oldmouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::OLD);

	auto curPos = lpSceneMng.cursol_->GetPos();

	//マップの拡大
	if (MagniMap())
	{
		//バツが押されたフレームは処理しない
		return ownScene;
	}

	if (DefMapUI())
	{
		//最終確認画面
		comScrFlag_ = true;
	}

	//確認画面
	if (ComfirmUi())
	{
		comEndFlag_ = true;
	}

	if (comEndFlag_)
	{
		if (GetASyncLoadNum() == 0)
		{
			//シーン移行tipsシーンへ
			return std::make_unique<FadeInOut>(std::move(ownScene), std::make_unique<TipsScene>(mapName_.c_str()), 1.0f);
		}
		return ownScene;
	}

	if (defScrEndFlag_)
	{
		//マップ表示中は処理しない
		return ownScene;
	}

	if (!stgSelectFlag_)
	{
		return ownScene;
	}

	if (!moveEndFlag_)
	{
		return ownScene;
	}

	if (mouseInput[MouseInputID::RIGHT] && !oldmouseInput[MouseInputID::RIGHT])
	{
		stgSelectFlag_ = false;
		std::fill(moveTimes_.begin(), moveTimes_.end(), 0.0);
		moveEndFlag_ = false;
	}

	//初期画面処理
	for (auto& ui : defStageList_)
	{
		//カーソルチェック
		if (ui->CheckColition(curPos))
		{
			//入力チェック
			if ((ui->CheckButtonInput(mouseInput[ui->GetCheckId()], oldmouseInput[ui->GetCheckId()])))
			{
				defScrEndFlag_ = true;

				std::string mapname = ui->GetMapName();
				mapName_ = mapname;

				//リセット
				lptmxobj.ResetTMX();
				plantList_.clear();

				//マップのロード
				lptmxobj.LoadTMX(mapname);

				//グラフィックハンドルの作成
				mapGh_ = MakeScreen(lptmxobj.GetworldArea().x_ * lptmxobj.GettileSize().x_ / BLOCK_SIZE_X * BLOCK_SIZE_X, lptmxobj.GetworldArea().y_ * lptmxobj.GettileSize().y_ / BLOCK_SIZE_Y * BLOCK_SIZE_Y, false);

				//マップごとの持ち物の取得
				//マップネームをmap+数字に加工
				mapname.erase(0, mapname.find_last_of("/") + 1);
				mapname.erase(mapname.find_last_of("."), static_cast<int>(mapname.size()));

				backGroundGh_ = lpImageMng.GetID(mapname)[0];

				mapObj_ = MapObject::GetMapObject(mapname);
				CreatePlantMenu();

				DrawMap();
			}
		}
	}

	return ownScene;
}

void SelectScene::Draw(void)
{
	DrawDefault();
	DrawStgSelect();
	DrawMagniMap();
	DrawDownUI();
	DrawMapScreen();
	DrawComfirmUi();

	//カーソルの描画
	lpSceneMng.cursol_->Draw(false);
}

void SelectScene::DrawDefault(void)
{
	//初期画面
	if (defScrEndFlag_)
	{
		return;
	}

	if (stgSelectFlag_)
	{
		return;
	}

	auto curPos = lpSceneMng.cursol_->GetPos();
	//マウスの値の取得
	auto mouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::NOW);
	auto oldmouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::OLD);

	//通常描画

	//背景描画
	DrawModiGraph(0, 0,
		lpSceneMng.GetViewArea().x_, 0,
		lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_,
		0, lpSceneMng.GetViewArea().y_, lpImageMng.GetID("Select")[0], true);

	int posX[2] = { 0,0 };
	int cnt = 0;
	for (const auto& defBtn : defBtnList_)
	{
		defBtn->Draw();
		if (defBtn->CheckColition(lpSceneMng.cursol_->GetPos()))
		{
			//入力チェック
			if (mouseInput[defBtn->GetCheckId()] && !oldmouseInput[defBtn->GetCheckId()])
			{
				defBtn->GetFunction()();
			}
			DrawGraph(0, defBtn->GetPos().y_, lpImageMng.GetID("MapSelectFrameLight")[0], true);
			posX[cnt] = AMO_MOVE;
		}
		cnt++;
	}
	
	DrawGraph(posX[0], MAP_SELECT_POS_Y, lpImageMng.GetID("MapSelectFrameText")[0], true);
	DrawGraph(posX[0], MAP_SELECT_POS_Y, lpImageMng.GetID("Triangle")[0], true);

	DrawGraph(posX[1], MAP_SELECT_POS_Y + MAP_SELECT_SIZE_Y + MAP_SELECT_INTERVAL, lpImageMng.GetID("ToTitleText")[0], true);
}

void SelectScene::DrawStgSelect(void)
{
	if (defScrEndFlag_)
	{
		return;
	}

	if (!stgSelectFlag_)
	{
		return;
	}

	auto curPos = lpSceneMng.cursol_->GetPos();
	//マウスの値の取得
	auto mouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::NOW);
	auto oldmouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::OLD);

	//背景描画
	DrawModiGraph(0, 0,
		lpSceneMng.GetViewArea().x_, 0,
		lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_,
		0, lpSceneMng.GetViewArea().y_, lpImageMng.GetID("Select")[0], true);

	int posX = 0;
	int cnt = 0;
	for (const auto& defBtn : defBtnList_)
	{
		defBtn->Draw();
		if (defBtn->CheckColition(lpSceneMng.cursol_->GetPos()))
		{
			//入力チェック
			if (mouseInput[defBtn->GetCheckId()] && !oldmouseInput[defBtn->GetCheckId()])
			{
				defBtn->GetFunction()();
			}
			DrawGraph(0, defBtn->GetPos().y_, lpImageMng.GetID("MapSelectFrameLight")[0], true);

			if (cnt == 1)
			{
				posX = AMO_MOVE;
			}
		}
		cnt++;
	}

	DrawGraph(0, MAP_SELECT_POS_Y, lpImageMng.GetID("MapSelectFrameLight")[0], true);
	DrawGraph(AMO_MOVE, MAP_SELECT_POS_Y, lpImageMng.GetID("MapSelectFrameText")[0], true);
	DrawGraph(AMO_MOVE, MAP_SELECT_POS_Y, lpImageMng.GetID("Triangle")[0], true);
	
	DrawGraph(posX, MAP_SELECT_POS_Y + MAP_SELECT_SIZE_Y + MAP_SELECT_INTERVAL, lpImageMng.GetID("ToTitleText")[0], true);

	cnt = 0;
	for (const auto& stage : defStageList_)
	{
		if (!moveEndFlag_)
		{
			int tmpAmoMove = static_cast<int>(Easing::OutQuart(moveTimes_[cnt], MAP_SELECT_TOTAL_MOVE_TIME + MAP_SELECT_TOTAL_MOVE_TIME_INV * cnt) * MAP_SELECT_OFFSET_X + MAP_SELECT_OFFSET_INV_X * cnt);
			DrawModiGraph(stgSelectPoses_[cnt].x_ - tmpAmoMove, stgSelectPoses_[cnt].y_,
				stgSelectPoses_[cnt].x_ + STAGE_SIZE_X - tmpAmoMove, stgSelectPoses_[cnt].y_,
				stgSelectPoses_[cnt].x_ + STAGE_SIZE_X - STAGE_SLOPE_SIZE_X - tmpAmoMove, stgSelectPoses_[cnt].y_ + STAGE_SIZE_Y,
				stgSelectPoses_[cnt].x_ - STAGE_SLOPE_SIZE_X - tmpAmoMove, stgSelectPoses_[cnt].y_ + STAGE_SIZE_Y,
				stage->GetModelID(), true);
		}
		else
		{
			stage->Draw();

			std::string tmpMapName = stage->GetMapName();
			tmpMapName.erase(0, tmpMapName.find_last_of("/") + 1);
			tmpMapName.erase(tmpMapName.find_last_of("."), static_cast<int>(tmpMapName.size()));

			if (stage->CheckColition(curPos))
			{
				Vector2 stagePos = stage->GetPos();

				DrawModiGraph(stagePos.x_, stagePos.y_,
					stagePos.x_ + STAGE_SIZE_X, stagePos.y_,
					stagePos.x_ + STAGE_SIZE_X - STAGE_SLOPE_SIZE_X, stagePos.y_ + STAGE_SIZE_Y,
					stagePos.x_ - STAGE_SLOPE_SIZE_X, stagePos.y_ + STAGE_SIZE_Y,
					lpImageMng.GetID("FrameLight")[0], true);

				DrawModiGraph(0, 0,
					STAGE_BG_SIZE_X, 0,
					STAGE_BG_SIZE_X - STAGE_BG_SLOPE_SIZE_X, STAGE_BG_SIZE_Y,
					-STAGE_BG_SLOPE_SIZE_X / 2, STAGE_BG_SIZE_Y, lpImageMng.GetID(tmpMapName)[0], true);
				DrawGraph(0, STAGE_BG_SIZE_Y - STAGE_TEXT_OFFSET_Y, lpImageMng.GetID(tmpMapName + "Text")[0], true);
			}

			if (lpSceneMng.clearMap_[tmpMapName] == false)
			{
				DrawGraph(stage->GetPos().x_, stage->GetPos().y_, lpImageMng.GetID("New")[0], true);
			}
		}
		cnt++;
	}

	cnt = 0;
	for (auto& moveTime : moveTimes_)
	{
		moveTime = min(moveTime + lpSceneMng.GetDeltaTime(), MAP_SELECT_TOTAL_MOVE_TIME + MAP_SELECT_TOTAL_MOVE_TIME_INV * cnt);
		cnt++;
	}
	if (MAP_SELECT_TOTAL_MOVE_TIME + MAP_SELECT_TOTAL_MOVE_TIME_INV * (cnt - 1) <= moveTimes_.back())
	{
		moveEndFlag_ = true;
	}
}

bool SelectScene::MagniMap(void)
{
	if (!defScrEndFlag_)
	{
		//マップを表示していない場合
		return false;
	}

	if (comScrFlag_)
	{
		//最終確認画面に到達している場合
		return false;
	}

	//マウスの値の取得
	auto mouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::NOW);
	auto oldmouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::OLD);

	auto curPos = lpSceneMng.cursol_->GetPos();

	if (0.0 < mapMagniTime_)
	{
		//マップを拡大、縮小している場合
		mapMagniTime_ -= lpSceneMng.GetDeltaTime();
	}

	if (mapMagniUpFlag_)
	{
		//拡大画面の表示時

		//右クリック時消滅
		if (mouseInput[MouseInputID::RIGHT] && !oldmouseInput[MouseInputID::RIGHT])
		{
			//拡大状態を削除
			mapMagniUpFlag_ = false;

			//縮小状態にする
			mapMagniTime_ = MAX_MAGNI_TIME;
			mapMagniDownFlag_ = true;

			return true;
		}

		//マップ表示画面のUI処理
		for (auto& ui : mapMagniUiList_)
		{
			//カーソルチェック
			if (ui->CheckColition(curPos))
			{
				//入力チェック
				if ((ui->CheckButtonInput(mouseInput[ui->GetCheckId()], oldmouseInput[ui->GetCheckId()])))
				{
					//拡大状態を削除
					mapMagniUpFlag_ = false;
					
					//縮小状態にする
					mapMagniTime_ = MAX_MAGNI_TIME;
					mapMagniDownFlag_ = true;

					return true;
				}
			}
		}
		return true;
	}

	return false;
}

void SelectScene::DrawMagniMap(void)
{
	if (!mapMagniUpFlag_)
	{
		return;
	}
	//マップの拡大描画
	auto maxMagniSize = lpSceneMng.GetViewArea() - Vector2(64, 64) - Vector2(MAP_POS_X + MAP_SIZE_X, MAP_POS_Y + MAP_SIZE_Y);

	Vector2 magniSize;
	magniSize.x_ = static_cast<int>(Easing::OUTEXP(MAX_MAGNI_TIME - mapMagniTime_, MAX_MAGNI_TIME) * maxMagniSize.x_);
	magniSize.y_ = static_cast<int>(Easing::OUTEXP(MAX_MAGNI_TIME - mapMagniTime_, MAX_MAGNI_TIME) * maxMagniSize.y_);

	//背景描画
	DrawModiGraph(0, 0,
		lpSceneMng.GetViewArea().x_, 0,
		lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_,
		0, lpSceneMng.GetViewArea().y_, backGroundGh_, true);

	DrawModiGraph(MAP_POS_X, MAP_POS_Y, MAP_POS_X + MAP_SIZE_X + magniSize.x_, MAP_POS_Y, MAP_POS_X + MAP_SIZE_X + magniSize.x_, MAP_POS_Y + MAP_SIZE_Y + magniSize.y_, MAP_POS_X, MAP_POS_Y + MAP_SIZE_Y + magniSize.y_, mapGh_, true);

	for (auto& magni : mapMagniUiList_)
	{
		magni->Draw();
	}
}

bool SelectScene::DefMapUI(void)
{
	if (!defScrEndFlag_)
	{
		//マップを表示していない場合
		return false;
	}

	if (comScrFlag_)
	{
		//最終確認画面に到達している場合
		return false;
	}

	//マウスの値の取得
	auto mouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::NOW);
	auto oldmouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::OLD);

	auto curPos = lpSceneMng.cursol_->GetPos();

	if (0.0 < mapMagniTime_)
	{
		//マップを拡大、縮小している場合
		mapMagniTime_ -= lpSceneMng.GetDeltaTime();
	}
	else
	{
		mapMagniDownFlag_ = false;
	}

	//マップ拡大
	for (auto& ui : mapUiList_)
	{
		if (ui->CheckColition(curPos))
		{
			//クリックされたとき
			if ((ui->CheckButtonInput(mouseInput[ui->GetCheckId()], oldmouseInput[ui->GetCheckId()])))
			{
				return ui->GetFunction()();
			}
		}
	}

	//右クリック時消滅
	if (mouseInput[MouseInputID::RIGHT] && !oldmouseInput[MouseInputID::RIGHT])
	{
		CancelUI();
	}

	return false;
}

void SelectScene::DrawDownUI(void)
{
	if (!mapMagniDownFlag_)
	{
		return;
	}
	//マップの縮小描画
	auto maxMagniSize = lpSceneMng.GetViewArea() - Vector2(64, 64) - Vector2(MAP_POS_X + MAP_SIZE_X, MAP_POS_Y + MAP_SIZE_Y);

	Vector2 magniSize;
	magniSize.x_ = static_cast<int>(Easing::InExp(mapMagniTime_, MAX_MAGNI_TIME) * maxMagniSize.x_);
	magniSize.y_ = static_cast<int>(Easing::InExp(mapMagniTime_, MAX_MAGNI_TIME) * maxMagniSize.y_);

	//背景描画
	DrawModiGraph(0, 0,
		lpSceneMng.GetViewArea().x_, 0,
		lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_,
		0, lpSceneMng.GetViewArea().y_, backGroundGh_, true);

	DrawModiGraph(MAP_POS_X, MAP_POS_Y, MAP_POS_X + MAP_SIZE_X + magniSize.x_, MAP_POS_Y, MAP_POS_X + MAP_SIZE_X + magniSize.x_, MAP_POS_Y + MAP_SIZE_Y + magniSize.y_, MAP_POS_X, MAP_POS_Y + MAP_SIZE_Y + magniSize.y_, mapGh_, true);
}

void SelectScene::DrawMapScreen(void)
{
	if (!defScrEndFlag_)
	{
		return;
	}

	if (mapMagniUpFlag_)
	{
		return;
	}

	if (mapMagniDownFlag_)
	{
		return;
	}
	//通常描画
	auto curPos = lpSceneMng.cursol_->GetPos();

	//背景描画
	DrawModiGraph(0, 0,
		lpSceneMng.GetViewArea().x_, 0,
		lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_,
		0, lpSceneMng.GetViewArea().y_, backGroundGh_, true);

	DrawModiGraph(MAP_POS_X - WINDOW_UI_2_OFFSET_X, MAP_POS_Y - WINDOW_UI_2_OFFSET_Y,
		MAP_POS_X + MAP_SIZE_X + WINDOW_UI_2_SIZE_X, MAP_POS_Y - WINDOW_UI_2_OFFSET_Y,
		MAP_POS_X + MAP_SIZE_X + WINDOW_UI_2_SIZE_X, MAP_POS_Y + MAP_SIZE_Y + WINDOW_UI_2_SIZE_Y,
		MAP_POS_X - WINDOW_UI_2_OFFSET_X, MAP_POS_Y + MAP_SIZE_Y + WINDOW_UI_2_SIZE_Y,
		lpImageMng.GetID("window2")[0], true);

	DrawModiGraph(MAP_POS_X - WINDOW_UI_3_OFFSET_X, MAP_POS_Y - WINDOW_UI_3_OFFSET_Y,
		MAP_POS_X + MAP_SIZE_X + WINDOW_UI_3_OFFSET_X, MAP_POS_Y - WINDOW_UI_3_OFFSET_Y,
		MAP_POS_X + MAP_SIZE_X + WINDOW_UI_3_OFFSET_X, MAP_POS_Y + MAP_SIZE_Y + WINDOW_UI_3_OFFSET_Y,
		MAP_POS_X - WINDOW_UI_3_OFFSET_X, MAP_POS_Y + MAP_SIZE_Y + WINDOW_UI_3_OFFSET_Y,
		lpImageMng.GetID("window3")[0], true);

	for (auto& ui : mapUiList_)
	{
		bool Flag = false;
		if (ui->CheckColition(curPos))
		{
			Flag = true;
		}

		Flag = comScrFlag_ == true ? false : Flag;

		ui->Draw(Flag, 28);
	}

	//マップの描画
	DrawModiGraph(MAP_POS_X, MAP_POS_Y, MAP_POS_X + MAP_SIZE_X, MAP_POS_Y, MAP_POS_X + MAP_SIZE_X, MAP_POS_Y + MAP_SIZE_Y, MAP_POS_X, MAP_POS_Y + MAP_SIZE_Y, mapGh_, true);

	DrawGraph(MAP_POS_X + 232, MAP_POS_Y - 36, lpImageMng.GetID("mapUI")[0], true);

	//フォントサイズの取得
	auto fontHandle = lpFontMng.GetFontSizeHandle(FontType::DEFAULT, SELECT_MAP_FONT_SIZE);
	int fontSize = 0;
	GetFontStateToHandle(NULL, &fontSize, NULL, fontHandle);

	DrawModiGraph(DEPLOY_POS_X - LOGO_GIM_OFFSET_X, DEPLOY_POS_Y - LOGO_GIM_OFFSET_Y,
		DEPLOY_POS_X + fontSize * 5 + LOGO_GIM_SIZE_X, DEPLOY_POS_Y - LOGO_GIM_OFFSET_Y,
		DEPLOY_POS_X + fontSize * 5 + LOGO_GIM_SIZE_X, DEPLOY_POS_Y + LOGO_GIM_SIZE_Y,
		DEPLOY_POS_X - LOGO_GIM_OFFSET_X, DEPLOY_POS_Y + LOGO_GIM_SIZE_Y, lpImageMng.GetID("cap_plant")[0], true);

	//所持ギミックの描画
	DrawModiGraph(DEPLOY_POS_X - LOGO_GIM_OFFSET_X + (plantSum_ + 1) * (BLOCK_SIZE_X + DEPLOY_INTERVAL), DEPLOY_POS_Y - LOGO_GIM_OFFSET_Y,
		DEPLOY_POS_X + fontSize * 5 + LOGO_GIM_SIZE_X + (plantSum_ + 1) * (BLOCK_SIZE_X + DEPLOY_INTERVAL), DEPLOY_POS_Y - LOGO_GIM_OFFSET_Y,
		DEPLOY_POS_X + fontSize * 5 + LOGO_GIM_SIZE_X + (plantSum_ + 1) * (BLOCK_SIZE_X + DEPLOY_INTERVAL), DEPLOY_POS_Y + LOGO_GIM_SIZE_Y,
		DEPLOY_POS_X - LOGO_GIM_OFFSET_X + (plantSum_ + 1) * (BLOCK_SIZE_X + DEPLOY_INTERVAL), DEPLOY_POS_Y + LOGO_GIM_SIZE_Y, lpImageMng.GetID("cap_gim")[0], true);
	//プラントの描画
	for (auto& plant : plantList_)
	{
		plant->Draw();
	}

	DrawStatus();

	int cnt = 0;
	if (mapObj_.kpNum_)
	{
		DrawFormatStringToHandle(DEPLOY_POS_X + BLOCK_SIZE_X + cnt * (BLOCK_SIZE_X + DEPLOY_INTERVAL) + DEPLOY_STR_INTERVAL,
			DEPLOY_POS_Y + BLOCK_SIZE_Y,
			0xffffff, fontHandle, "x%d", mapObj_.kpNum_);
		cnt++;
	}
	if (mapObj_.wpNum_)
	{
		DrawFormatStringToHandle(DEPLOY_POS_X + BLOCK_SIZE_X + cnt * (BLOCK_SIZE_X + DEPLOY_INTERVAL) + DEPLOY_STR_INTERVAL,
			DEPLOY_POS_Y + BLOCK_SIZE_Y,
			0xffffff, fontHandle, "x%d", mapObj_.wpNum_);
		cnt++;
	}
	if (mapObj_.apNum_)
	{
		DrawFormatStringToHandle(DEPLOY_POS_X + BLOCK_SIZE_X + cnt * (BLOCK_SIZE_X + DEPLOY_INTERVAL) + DEPLOY_STR_INTERVAL,
			DEPLOY_POS_Y + BLOCK_SIZE_Y,
			0xffffff, fontHandle, "x%d", mapObj_.apNum_);
		cnt++;
	}
	if (mapObj_.huricaneNum_)
	{
		DrawFormatStringToHandle(DEPLOY_POS_X + BLOCK_SIZE_X + (cnt + 1) * (BLOCK_SIZE_X + DEPLOY_INTERVAL),
			DEPLOY_POS_Y + BLOCK_SIZE_Y,
			0xffffff, fontHandle, "x%d", mapObj_.huricaneNum_);
		cnt++;
	}
}

bool SelectScene::ComfirmUi(void)
{
	if (!comScrFlag_)
	{
		return false;
	}

	//マウスの値の取得
	auto mouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::NOW);
	auto oldmouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::OLD);

	auto curPos = lpSceneMng.cursol_->GetPos();

	for (auto& ui : confirmUiList_)
	{
		if (ui->CheckColition(curPos))
		{
			//クリックされたとき
			if ((ui->CheckButtonInput(mouseInput[ui->GetCheckId()], oldmouseInput[ui->GetCheckId()])))
			{
				return ui->GetFunction()();
			}
		}
	}

	//右クリック時確認状態を消す
	if (mouseInput[MouseInputID::RIGHT] && !oldmouseInput[MouseInputID::RIGHT])
	{
		comScrFlag_ = false;
	}

	return false;
}

void SelectScene::DrawComfirmUi(void)
{
	if (!comScrFlag_)
	{
		return;
	}

	auto fontHandle = lpFontMng.GetFontSizeHandle(FontType::DEFAULT, SELECT_CONFIRM_FONT_SIZE);

	auto curPos = lpSceneMng.cursol_->GetPos();

	DrawModiGraph(COMFIRM_UI_POS_X - 20, COMFIRM_UI_POS_Y - 106,
		COMFIRM_UI_POS_X + COMFIRM_UI_SIZE_X * 2 + 30, COMFIRM_UI_POS_Y - 106,
		COMFIRM_UI_POS_X + COMFIRM_UI_SIZE_X * 2 + 30, COMFIRM_UI_POS_Y + COMFIRM_UI_SIZE_Y * 2 - 38,
		COMFIRM_UI_POS_X - 20, COMFIRM_UI_POS_Y + COMFIRM_UI_SIZE_Y * 2 - 38, lpImageMng.GetID("textbox")[0], true);

	DrawFormatStringToHandle(COMFIRM_UI_POS_X + 20, COMFIRM_UI_POS_Y - 72, 0xffffff, fontHandle, "出撃しますか？");

	for (auto& ui : confirmUiList_)
	{
		bool Flag = false;
		if (ui->CheckColition(curPos))
		{
			Flag = true;
		}
		ui->Draw(Flag, 28);
	}
}

void SelectScene::LoadImageAll(void)
{
	//非同期前に読み込んでおく
	lpImageMng.GetID("Resource/image/Background/Select.jpg", "Select");
	lpImageMng.GetID("Resource/image/UI/textbox.png", "textbox");
	lpImageMng.GetID("Resource/image/UI/Select/MapSelectFrame.png", "MapSelectFrame");
	lpImageMng.GetID("Resource/image/UI/Select/MapSelectFrameText.png", "MapSelectFrameText");
	lpImageMng.GetID("Resource/image/UI/Select/MapSelectFrameLight.png", "MapSelectFrameLight");
	lpImageMng.GetID("Resource/image/UI/Select/Triangle.png", "Triangle");
	lpImageMng.GetID("Resource/image/UI/Select/ToTitle.png", "ToTitle");
	lpImageMng.GetID("Resource/image/UI/Select/ToTitleText.png", "ToTitleText");
	lpImageMng.GetID("Resource/image/UI/Select/FrameLight.png", "FrameLight");
	lpImageMng.GetID("Resource/image/UI/Select/Stage0.png", "Stage0");
	lpImageMng.GetID("Resource/image/UI/Select/Stage1.png", "Stage1");
	lpImageMng.GetID("Resource/image/UI/Select/Stage2.png", "Stage2");
	lpImageMng.GetID("Resource/image/UI/Select/Stage5.png", "Stage5");
	lpImageMng.GetID("Resource/image/UI/Select/New.png", "New");
	lpImageMng.GetID("Resource/image/UI/Select/map0Text.png", "map0Text");
	lpImageMng.GetID("Resource/image/UI/Select/map1Text.png", "map1Text");
	lpImageMng.GetID("Resource/image/UI/Select/map2Text.png", "map2Text");
	lpImageMng.GetID("Resource/image/UI/Select/map4Text.png", "map4Text");
	
	lpAudioMng.GetID("Resource/Audio/BGM/select.mp3", "select");

	SetUseASyncLoadFlag(true);

	lpImageMng.GetID("Resource/image/Plant/knightplant.png", "Knightplant");
	lpImageMng.GetID("Resource/image/Plant/archerplant.png", "Archerplant");
	lpImageMng.GetID("Resource/image/Plant/warriorplant.png", "Warriorplant");

	lpImageMng.GetID("Resource/image/UI/Status/KnightStatus.png", "KnightStatus");
	lpImageMng.GetID("Resource/image/UI/Status/ArcherStatus.png", "ArcherStatus");
	lpImageMng.GetID("Resource/image/UI/Status/WarriorStatus.png", "WarriorStatus");
	lpImageMng.GetID("Resource/image/UI/Status/HurricaneStatus.png", "HurricaneStatus");
	lpImageMng.GetID("Resource/image/effect/wind/wind.png", "wind", { GIMMICK_SIZE_X,GIMMICK_SIZE_Y }, { WIND_DIVCNT_X,WIND_DIVCNT_Y });

	//背景
	lpImageMng.GetID("Resource/image/Background/map0.jpg", "map0");
	lpImageMng.GetID("Resource/image/Background/map2.jpg", "map2");
	lpImageMng.GetID("Resource/image/Background/map4.jpg", "map4");

	//UI
	lpImageMng.GetID("Resource/image/UI/window1.png", "window1");
	lpImageMng.GetID("Resource/image/UI/window2.jpg", "window2");
	lpImageMng.GetID("Resource/image/UI/window3.jpg", "window3");

	lpImageMng.GetID("Resource/image/UI/caption.png", "caption");
	lpImageMng.GetID("Resource/image/UI/mapUI.png", "mapUI");
	lpImageMng.GetID("Resource/image/UI/cap_plant.png", "cap_plant");
	lpImageMng.GetID("Resource/image/UI/cap_gim.png", "cap_gim");

	lpImageMng.GetID("Resource/image/UI/pattern.png", "pattern");
	lpImageMng.GetID("Resource/image/UI/woodframe.png", "woodframe");
	lpImageMng.GetID("Resource/image/UI/woodframe0.jpg", "woodframe0");
	lpImageMng.GetID("Resource/image/UI/woodframe01.png", "woodframe01");
	lpImageMng.GetID("Resource/image/UI/woodframe02.png", "woodframe02");
	lpImageMng.GetID("Resource/image/UI/woodframe03.png", "woodframe03");
	lpImageMng.GetID("Resource/image/UI/woodframe04.png", "woodframe04");

	//プラント
	lpImageMng.GetID("Resource/image/Plant/knightplantenemy.png", "Knightplantenemy");
	lpImageMng.GetID("Resource/image/Plant/crack/kpcrack1.png", "KPcrack1");
	lpImageMng.GetID("Resource/image/Plant/crack/kpcrack2.png", "KPcrack2");

	lpImageMng.GetID("Resource/image/Plant/archerplantenemy.png", "Archerplantenemy");
	lpImageMng.GetID("Resource/image/Plant/crack/apcrack1.png", "APcrack1");
	lpImageMng.GetID("Resource/image/Plant/crack/apcrack2.png", "APcrack2");

	lpImageMng.GetID("Resource/image/Plant/warriorplantenemy.png", "Warriorplantenemy");
	lpImageMng.GetID("Resource/image/Plant/crack/wpcrack1.png", "WPcrack1");
	lpImageMng.GetID("Resource/image/Plant/crack/wpcrack2.png", "WPcrack2");

	//コア
	lpImageMng.GetID("Resource/image/core/coreally.png", "Coreally", { CORE_SIZE_X,CORE_SIZE_Y }, { 3,4 });
	lpImageMng.GetID("Resource/image/core/coreenemy.png", "Coreenemy", { CORE_SIZE_X,CORE_SIZE_Y }, { 3,4 });

	//ギミック
	lpImageMng.GetID("Resource/image/effect/wind/wind.png", "wind", { GIMMICK_SIZE_X,GIMMICK_SIZE_Y }, { WIND_DIVCNT_X,WIND_DIVCNT_Y });

	//シグナル
	lpImageMng.GetID("Resource/image/UI/Signal/SigTriAlly.png", "SigTriAlly");
	lpImageMng.GetID("Resource/image/UI/Signal/SigTriEnemy.png", "SigTriEnemy");
	lpImageMng.GetID("Resource/image/UI/Signal/SigTriAllyPlant.png", "SigTriAllyPlant");
	lpImageMng.GetID("Resource/image/UI/Signal/SigTriEnemyPlant.png", "SigTriEnemyPlant");

	//チップス画面
	lpImageMng.GetID("Resource/image/UI/NowLoading.png", "NowLoading");
	lpImageMng.GetID("Resource/image/UI/PushTheScreen.png", "PushTheScreen");
	lpImageMng.GetID("Resource/image/UI/dot.png", "dot");
	for (int i = 1; i <= BACKLIGHT_SUM; i++)
	{
		lpImageMng.SetID("Resource/image/effect/BackLight/" + std::to_string(i) + ".png", "BackLight");
	}
	for (int i = 1; i <= ONE_LIGHT_SUM; i++)
	{
		lpImageMng.SetID("Resource/image/effect/OneBackLight/" + std::to_string(i) + ".png", "OneBackLight");
	}

	lpAudioMng.GetID("Resource/Audio/SE/ClickStar.mp3", "ClickStar");
	lpAudioMng.GetID("Resource/Audio/SE/EndLoading.mp3", "EndLoading");

	//カウントダウン
	lpImageMng.GetID("Resource/image/UI/CountDown/1.png", "Count1");
	lpImageMng.GetID("Resource/image/UI/CountDown/2.png", "Count2");
	lpImageMng.GetID("Resource/image/UI/CountDown/3.png", "Count3");
	lpImageMng.GetID("Resource/image/UI/CountDown/go.png", "CountGo");
	lpAudioMng.GetID("Resource/Audio/SE/drum0.mp3", "Drum0");
	lpAudioMng.GetID("Resource/Audio/SE/drum1.mp3", "Drum1");

	//ゲームシーン
	lpImageMng.GetID("Resource/image/Unit/shadow.png", "Shadow");
	lpImageMng.GetID("Resource/image/UI/WarSituation.png", "WarSituation");
	lpImageMng.GetID("Resource/image/UI/ally_text.png", "AllyText");
	lpImageMng.GetID("Resource/image/UI/enemy_text.png", "EnemyText");
	for (int i = 1; i <= BLINKLIGHT_SUM; i++)
	{
		lpImageMng.SetID("Resource/image/effect/BlinkLight/" + std::to_string(i) + ".png", "BlinkLight");
	}
	for (int i = 1; i <= CONVERGENCE_SUM; i++)
	{
		lpImageMng.SetID("Resource/image/effect/Convergence/" + std::to_string(i) + ".png", "Convergence");
	}
	for (int i = 1; i <= EXPLOSION_SUM; i++)
	{
		lpImageMng.SetID("Resource/image/effect/Explosion/" + std::to_string(i) + ".png", "Explosion");
	}

	lpAudioMng.GetID("Resource/Audio/SE/Unit/KnightAtk.mp3", "KnightAtk");
	lpAudioMng.GetID("Resource/Audio/SE/Unit/ArcherAtk.mp3", "ArcherAtk");
	lpAudioMng.GetID("Resource/Audio/SE/Unit/WarriorAtk.mp3", "WarriorAtk");
	lpAudioMng.GetID("Resource/Audio/SE/Unit/AttackedCore.mp3", "AttackedCore");
	lpAudioMng.GetID("Resource/Audio/SE/Unit/AttackedPlant.mp3", "AttackedPlant");
	lpAudioMng.GetID("Resource/Audio/SE/Unit/BrokenCore.mp3", "BrokenCore");
	lpAudioMng.GetID("Resource/Audio/SE/Unit/BrokenPlant.mp3", "BrokenPlant");
	lpAudioMng.GetID("Resource/Audio/SE/Unit/RepelArrow.mp3", "RepelArrow");
	lpAudioMng.GetID("Resource/Audio/SE/DeployPlant.mp3", "DeployPlant");

	lpAudioMng.GetID("Resource/Audio/SE/Explosion.mp3", "Explosion");
	lpAudioMng.GetID("Resource/Audio/SE/Convergence.mp3", "Convergence");


	SetUseASyncLoadFlag(false);
}

void SelectScene::DrawMap(void)
{
	//マップを描画
	SetDrawScreen(mapGh_);
	ClsDrawScreen();
	lptmxobj.DrawMap();

	//施設の描画
	//味方
	for (auto& plant : lptmxobj.GetAllyPlantPos())
	{
		DrawGraph(plant.second.x_ * BLOCK_SIZE_X, plant.second.y_ * BLOCK_SIZE_Y, lpImageMng.GetID(lpImageMng.ConvertObjTypeToKey(plant.first,ArmyType::ALLY))[0], true);
	}
	//敵
	for (auto& plant : lptmxobj.GetEnemyPlantPos())
	{
		DrawGraph(plant.second.x_ * BLOCK_SIZE_X, plant.second.y_ * BLOCK_SIZE_Y, lpImageMng.GetID(lpImageMng.ConvertObjTypeToKey(plant.first , ArmyType::ENEMY))[0], true);
	}

	//コアの描画
	//味方
	DrawGraph(lptmxobj.GetAllyCorePos().x_ * BLOCK_SIZE_X, lptmxobj.GetAllyCorePos().y_ * BLOCK_SIZE_Y - BLOCK_SIZE_Y, lpImageMng.GetID("Coreally")[0], true);
	//敵
	DrawGraph(lptmxobj.GetEnemyCorePos().x_ * BLOCK_SIZE_X, lptmxobj.GetEnemyCorePos().y_ * BLOCK_SIZE_Y - BLOCK_SIZE_Y, lpImageMng.GetID("Coreenemy")[0], true);
}

void SelectScene::CreatePlantMenu(void)
{
	//使用可能施設のカウント
	int plantCnt = 0;

	auto fontHandle = lpFontMng.GetFontSizeHandle(FontType::DEFAULT, SELECT_MAP_FONT_SIZE);
	//フォントサイズの取得
	int fontSize = 0;
	GetFontStateToHandle(NULL, &fontSize, NULL, fontHandle);

	if (mapObj_.kpNum_)
	{
		auto ramda = [&](int Cnt)
		{
			DrawGraph(DEPLOY_POS_X + Cnt * (BLOCK_SIZE_X + DEPLOY_INTERVAL) - STATUS_SIZE_X / 2, DEPLOY_POS_Y - STATUS_SIZE_Y, lpImageMng.GetID("KnightStatus")[0], true);
			return false;
		};
		plantList_.push_back(std::make_unique<Box>(
			Vector2(DEPLOY_POS_X + plantCnt * (BLOCK_SIZE_X + DEPLOY_INTERVAL), DEPLOY_POS_Y + fontSize),
			Vector2(BLOCK_SIZE_X, BLOCK_SIZE_Y),
			MouseInputID::LEFT,
			ramda,
			ObjType::KNIGHT));
		plantList_.back()->SetModelID(lpImageMng.GetID("Knightplant")[0]);
		plantCnt++;
	}

	if (mapObj_.wpNum_)
	{
		auto ramda = [&](int Cnt)
		{
			DrawGraph(DEPLOY_POS_X + Cnt * (BLOCK_SIZE_X + DEPLOY_INTERVAL) - STATUS_SIZE_X / 2, DEPLOY_POS_Y - STATUS_SIZE_Y, lpImageMng.GetID("WarriorStatus")[0], true);
			return false;
		};
		plantList_.push_back(std::make_unique<Box>(
			Vector2(DEPLOY_POS_X + plantCnt * (BLOCK_SIZE_X + DEPLOY_INTERVAL), DEPLOY_POS_Y + fontSize),
			Vector2(BLOCK_SIZE_X, BLOCK_SIZE_Y),
			MouseInputID::LEFT,
			ramda,
			ObjType::WARRIOR));
		plantList_.back()->SetModelID(lpImageMng.GetID("Warriorplant")[0]);
		plantCnt++;
	}

	if (mapObj_.apNum_)
	{
		auto ramda = [&](int Cnt)
		{
			DrawGraph(DEPLOY_POS_X + Cnt * (BLOCK_SIZE_X + DEPLOY_INTERVAL) - STATUS_SIZE_X / 2, DEPLOY_POS_Y - STATUS_SIZE_Y, lpImageMng.GetID("ArcherStatus")[0], true);
			return false;
		};
		plantList_.push_back(std::make_unique<Box>(
			Vector2(DEPLOY_POS_X + plantCnt * (BLOCK_SIZE_X + DEPLOY_INTERVAL), DEPLOY_POS_Y + fontSize),
			Vector2(BLOCK_SIZE_X, BLOCK_SIZE_Y),
			MouseInputID::LEFT,
			ramda,
			ObjType::ARCHER));
		plantList_.back()->SetModelID(lpImageMng.GetID("Archerplant")[0]);
		plantCnt++;
	}

	//所持ギミック
	//使用可能ギミックのカウント
	int gimCnt = 0;

	if (mapObj_.huricaneNum_)
	{
		auto ramda = [&](int Cnt)
		{
			DrawGraph(DEPLOY_POS_X + (Cnt + 1) * (BLOCK_SIZE_X + DEPLOY_INTERVAL) - STATUS_SIZE_X / 2, DEPLOY_POS_Y - HUR_STATUS_SIZE_Y, lpImageMng.GetID("HurricaneStatus")[0], true);
			return false;
		};
		plantList_.push_back(std::make_unique<Box>(
			Vector2(DEPLOY_POS_X + (plantCnt + 1 + gimCnt) * (BLOCK_SIZE_X + DEPLOY_INTERVAL), DEPLOY_POS_Y + fontSize),
			Vector2(BLOCK_SIZE_X, BLOCK_SIZE_Y),
			MouseInputID::LEFT,
			ramda,
			ObjType::MAX));
		plantList_.back()->SetModelID(lpImageMng.GetID("wind")[5]);
		gimCnt++;
	}
	plantSum_ = plantCnt;
}

void SelectScene::DrawStatus(void)
{
	//通常描画
	auto curPos = lpSceneMng.cursol_->GetPos();

	//カーソルがあったさい能力表示
	bool Flag = false;
	int cnt = 2;
	for (const auto& plant : plantList_)
	{
		if (plant->CheckColition(curPos))
		{
			plant->GetFunction2()(cnt);
			Flag = true;
			DrawByObjType(Vector2(DEPLOY_POS_X + cnt * (BLOCK_SIZE_X + DEPLOY_INTERVAL) - STATUS_SIZE_X / 2,
				DEPLOY_POS_Y - STATUS_SIZE_Y), plant->GetObjType());
		}
		cnt++;
	}

	if (Flag)
	{
		animTime_ += lpSceneMng.GetDeltaTime();
	}
	else
	{
		animTime_ = 0.0;
	}
}

void SelectScene::DrawByObjType(Vector2 pos, ObjType objType)
{
	int id = CaliculateId(objType);
	switch (objType)
	{
	case ObjType::KNIGHT:
		DrawRotaGraph(pos.x_ + STATUS_RELATION_POS_X, pos.y_ + STATUS_RELATION_POS_Y,
			3.0,0.0,lpImageMng.GetDotID("Knight")[id], true);
		break;
	case ObjType::WARRIOR:
		DrawRotaGraph(pos.x_ + STATUS_RELATION_POS_X, pos.y_ + STATUS_RELATION_POS_Y,
			3.0, 0.0, lpImageMng.GetDotID("Warrior")[id], true);
		break;
	case ObjType::ARCHER:
		DrawRotaGraph(pos.x_ + STATUS_RELATION_POS_X, pos.y_ + STATUS_RELATION_POS_Y,
			3.0, 0.0, lpImageMng.GetDotID("Archer")[id], true);
		break;
	case ObjType::MAX:
		DrawExtendGraph(pos.x_ + HUR_STATUS_RELATION_POS_X, pos.y_ + HUR_STATUS_RELATION_POS_Y,
			pos.x_ + HUR_STATUS_RELATION_POS_X + BLOCK_SIZE_X, pos.y_ + HUR_STATUS_RELATION_POS_Y + BLOCK_SIZE_Y,
			lpImageMng.GetID("wind")[id], true);
		break;
	default:
		//何もしない
		break;
	}
}

void SelectScene::SetUnitActiveID(void)
{
	//ユニット別にのすべての有効IDをセット
	SetID(ObjType::KNIGHT, KNIGHT_MOVE_ID_X, KNIGHT_MOVE_ID_Y, KNIGHTDIVCNT_X, KNIGHT_MOVE_FRAMESUM);
	SetID(ObjType::KNIGHT, KNIGHT_ATTACK_ID_X, KNIGHT_ATTACK_ID_Y, KNIGHTDIVCNT_X, KNIGHT_ATTACK_FRAMESUM);
	SetID(ObjType::KNIGHT, KNIGHT_SHIELD_ID_X, KNIGHT_SHIELD_ID_Y, KNIGHTDIVCNT_X, KNIGHT_SHIELD_FRAMESUM);

	SetID(ObjType::ARCHER, ARCHER_MOVE_ID_X, ARCHER_MOVE_ID_Y, ARCHERDIVCNT_X, ARCHER_MOVE_FRAMESUM);
	SetID(ObjType::ARCHER, ARCHER_ATTACK_ID_X, ARCHER_ATTACK_ID_Y, ARCHERDIVCNT_X, ARCHER_ATTACK_FRAMESUM);

	SetID(ObjType::WARRIOR, WARRIOR_MOVE_ID_X, WARRIOR_MOVE_ID_Y, WARRIORDIVCNT_X, WARRIOR_MOVE_FRAMESUM);
	SetID(ObjType::WARRIOR, WARRIOR_ATTACK_ID_X, WARRIOR_ATTACK_ID_Y, WARRIORDIVCNT_X, WARRIOR_ATTACK_FRAMESUM);

	SetID(ObjType::MAX, 0, 0, WIND_DIVCNT_X, WIND_DIVCNT_X * WIND_DIVCNT_Y);
}

void SelectScene::SetID(ObjType objType, int startIdX, int startIdY, int divX, int frameSum)
{
	for (int i = startIdX + startIdY * divX; i < startIdX + startIdY * divX + frameSum; i++)
	{
		unitIdMap_[objType].push_back(i);
	}
}

int SelectScene::CaliculateId(ObjType objType)
{
	int id = 0;
	if (unitIdMap_.count(objType) == 1)
	{
		id = unitIdMap_[objType].front();
		id = unitIdMap_[objType][static_cast<int>(fmod((animTime_ / STATUS_INV_TIME), static_cast<int>(unitIdMap_[objType].size())))];
	}
	return id;
}

bool SelectScene::MapDownUI(void)
{
	mapMagniUpFlag_ = true;
	mapMagniTime_ = MAX_MAGNI_TIME;

	if (mapMagniDownFlag_)
	{
		mapMagniDownFlag_ = false;
	}

	//0割り防止のため
	mapMagniTime_ -= 0.00001;

	return false;
}

bool SelectScene::CancelUI(void)
{
	defScrEndFlag_ = false;
	return false;
}

bool SelectScene::SetoffUI(void)
{
	return true;
}

bool SelectScene::CancelUI2(void)
{
	comScrFlag_ = false;
	return false;
}

bool SelectScene::GoUI(void)
{
	return true;
}


