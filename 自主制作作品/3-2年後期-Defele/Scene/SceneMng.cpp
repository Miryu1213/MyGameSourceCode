#include <DxLib.h>
#include <chrono>
#include "SceneMng.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "ResultScene.h"

#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"

//アイコンID
#define ICON_ID 100

//初期全体サイズ
constexpr int Def_World_Size_X = 1024;
constexpr int Def_World_Size_Y = 768;

//初期表示エリア
constexpr int Def_View_Area_X = 1024;
constexpr int Def_View_Area_Y = 768;

//画面オフセット
constexpr int GAME_OFFSET = 60;

using namespace std::chrono;

void SceneMng::Run(void)
{
	if (!Init())
	{
		return;
	}
	//初期シーン
	scene_ = std::make_unique<TitleScene>();
	//scene_ = std::make_unique<ResultScene>(ArmyType::ALLY);

	//時間保存系
	system_clock::time_point StartTime = system_clock::now();									//開始時の日時
	microseconds microstarttime = duration_cast<milliseconds>(StartTime.time_since_epoch());	//現在のシステム時間
	system_clock::time_point nowTime;															//現在の日時
	microseconds elaptime = microstarttime;
	microseconds oldelaptime;																	//一つ前の時間

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && !gameEndFlag_)
	{
		_dbgStartDraw();
		oldelaptime = elaptime;																	//一つ前の時間の更新
		nowTime = system_clock::now();															//現在の日時
		elaptime = duration_cast<microseconds>(nowTime.time_since_epoch());
		delta_ = static_cast<double>((elaptime - oldelaptime).count() * 0.000001f);

		UpDate();
		Draw();

		totaldelta_ += delta_;
	}
	DxLib_End();
}

bool SceneMng::Init(void)
{
	if (!SysInit())
	{
		//二重チェック
		if (!SysInit())
		{
			TRACE("SysInit失敗\n");
			return false;
		}
	}

	//カーソルの作成
	cursol_ = std::make_shared<Cursol>();

	//カーソルの初期化
	cursol_->Init();

	return true;
}

void SceneMng::UpDate(void)
{
	auto oldScene = scene_->GetSceneID();
	scene_ = scene_->UpDate(std::move(scene_));
	if (oldScene != scene_->GetSceneID())
	{
		//シーンが変わったときのリセット
		//経過時間
 		totaldelta_ = 0.0;
		//倍速
		doubleValue_ = 1.0;
		//カーソル
		cursol_->Init();

		lpSceneMng.alpha_ = 255;
	}
}

void SceneMng::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	scene_->Draw();

#ifdef _DEBUG
	_dbgDraw();
#endif

	/*auto fps = GetFPS();
	auto drawcall = GetDrawCallCount();
	DrawFormatString(10, 10, 0xffffff, "FPS : %f", fps);
	DrawFormatString(10, 30, 0xffffff, "DrawCallCount : %d", drawcall);*/

	ScreenFlip();
}

void SceneMng::SetWorldSize(Vector2 worldsize)
{
	worldSize_ = worldsize;
}

Vector2 SceneMng::GetCurPosAddOffset(void)
{
	return cursol_->GetPos() + Vector2{ 0,GetOffset() };
}

bool SceneMng::SysInit(void)
{
	SetWindowText("Loading...");
	SetWindowIconID(ICON_ID);
	//初期サイズの設定
	worldSize_ = { Def_World_Size_X,Def_World_Size_Y };
	viewArea_ = { Def_View_Area_X,Def_View_Area_Y };

	//オフセット設定
	offset_ = GAME_OFFSET;

	//倍速の値
	doubleValue_ = 1.0;

	//DxLib関連
	SetGraphMode(Def_View_Area_X, Def_View_Area_Y, 32);		//画面のサイズ、色深度(表現数)
	ChangeWindowMode(true);									//trueでwindow falseでフルスクリーン
	if (DxLib_Init() == -1)
	{
		return false;
	}

	//ストリーム再生
	//SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS_PLUS);

#ifdef _DEBUG
	_dbgSetup(Def_View_Area_X, Def_View_Area_Y, 255);
#endif // DEBUG

	return true;
}

SceneMng::SceneMng()
{
	delta_ = 0.0;
	totaldelta_ = 0.0;
	doubleValue_ = 1.0;

	alpha_ = 255;

	gameEndFlag_ = false;
}

SceneMng::~SceneMng()
{
	scene_.release();
}
