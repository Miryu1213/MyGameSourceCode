#include "SceneMng.h"
#include<Dxlib.h>
#include<time.h>
#include"GameScene.h"
#include"TitleScene.h"
#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"

void SceneMng::Init()
{
	scene_ = std::make_unique<TitleScene>();
}

void SceneMng::Run()
{
	Init();
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && outkey == false)
	{
		_dbgStartDraw();
		scene_ = scene_->UpDate(std::move(scene_));

		Draw();
	}
}

void SceneMng::Draw()
{
	SetDrawScreen(DX_SCREEN_BACK);			// バックバッファに設定
	ClsDrawScreen();						// 画面消去

	scene_->Draw();
	_dbgAddDraw();			//画面追加処理（スローなど）
	ScreenFlip();			 // 裏画面を表画面に瞬間コピー
}

bool SceneMng::SysInit(void)
{
	// システム処理
	SetWindowText("ForestGuardian");
	// システム処理
	SetGraphMode(scnArea_.x, scnArea_.y, 16);		// 800×600ドット65536色モードに設定
	ChangeWindowMode(true);								// true:window false:フルスクリーン
	if (DxLib_Init() == -1)
	{
		return false;									// DXライブラリの初期化処理
	}
	_dbgSetup(scnArea_.x, scnArea_.y, 200);			//デバッグライブラリの初期化

	srand((unsigned int)time(NULL));		//コンピュータの時間をsrandに入れ、疑似的にランダムに

	return true;
}

#define VIEW_AREA_X (836)
#define VIEW_AREA_Y (480)

SceneMng::SceneMng() :
	scnArea_{ 836,480 },
	viewArea_{ VIEW_AREA_X,VIEW_AREA_Y },
	worldArea_{ VIEW_AREA_X * 2,VIEW_AREA_Y },
	PlayerSize_{ 138,96 }
{
	outkey = false;
	SysInit();
}

SceneMng::~SceneMng()
{
}
