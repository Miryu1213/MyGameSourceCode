#include<DxLib.h>
#include"MapCtl.h"
#include "EditScene.h"
#include"GameScene.h"
#include"SceneMng.h"

#include"../_debug/_DebugConOut.h"

EditScene::EditScene()
{
	TRACE("EditSceneの生成\n");
	Init();
}
EditScene::~EditScene()
{
	TRACE("EditSceneの削除\n");
}

uniqueBaseScene EditScene::UpDate(uniqueBaseScene own)
{
	//TRACE("EditSceneのUpDate\n");
	if (UpDateObj() == true)
	{
		MapCtl::Destroy();
		return std::make_unique<GameScene>();
	}
	camera_->UpDate();
	DrawOwnScreen();
	return std::move(own);
}

SceneID EditScene::GetSceneID()
{
	DeleteGraph(BGimage_);
	return SceneID::EDIT;
}


bool EditScene::Init()
{
	MapCtl::Create(SceneID::EDIT);

	if ((BGimage_ = LoadGraph("image/Forest.png")) == -1)
	{
		TRACE("背景の読み込み失敗\n");
	}

	auto GameScreenSize = lpSceneMng.viewArea_;
	//+1は右端と下端の罫線描画のため
	gameScrID_ = MakeScreen(GameScreenSize.x + 1, GameScreenSize.y + 1, 255);

	auto itrObj = AddobjList(std::make_shared<EditCar>(camera_));

	//camera_->SetTarget((*itrObj));
	camera_->InitArea();
	camera_->SetPos((*itrObj)->GetPos());
	lpMapCtl.SetCamera(camera_);

	DrawOwnScreen();
	return false;
}

void EditScene::DrawOwnScreen(void)
{
	SetDrawScreen(gameScrID_);
	ClsDrawScreen();
	//シーンの内容の描画
	auto camoffset = camera_->GetDrawOffset();
	DrawModiGraph(
		camoffset.x, 0,														//左上
		lpSceneMng.viewArea_.x + camoffset.x, 0,							//右上
		lpSceneMng.viewArea_.x + camoffset.x, lpSceneMng.viewArea_.y,		//右下
		camoffset.x, lpSceneMng.viewArea_.y,								//左下
		BGimage_, false);
	DrawModiGraph(
		camoffset.x + lpSceneMng.viewArea_.x, 0,													//左上
		lpSceneMng.viewArea_.x + camoffset.x + lpSceneMng.viewArea_.x, 0,							//右上
		lpSceneMng.viewArea_.x + camoffset.x + lpSceneMng.viewArea_.x, lpSceneMng.viewArea_.y,		//右下
		camoffset.x + lpSceneMng.viewArea_.x, lpSceneMng.viewArea_.y,								//左下
		BGimage_, false);
	lpMapCtl.DrawFunc();
	DrawObj();
	camera_->Draw();

	SetDrawScreen(screenScrID_);
	ClsDrawScreen();

	//文字
	
	//エディット系全般の描画
	DrawGraph(0, 0, gameScrID_, true);
	
	/*DrawBox(harfmap.x - returnPoint + lpSceneMng.drawOffset_.x,
		harfmap.y - returnPoint + lpSceneMng.drawOffset_.y,
		harfmap.x + returnPoint + lpSceneMng.drawOffset_.x,
		harfmap.y + returnPoint + lpSceneMng.drawOffset_.y,
		GetColor(255, 255, 255), true);*/
}