#include<DxLib.h>
#include"MapCtl.h"
#include "EditScene.h"
#include"GameScene.h"
#include"SceneMng.h"

#include"../_debug/_DebugConOut.h"

EditScene::EditScene()
{
	TRACE("EditScene�̐���\n");
	Init();
}
EditScene::~EditScene()
{
	TRACE("EditScene�̍폜\n");
}

uniqueBaseScene EditScene::UpDate(uniqueBaseScene own)
{
	//TRACE("EditScene��UpDate\n");
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
		TRACE("�w�i�̓ǂݍ��ݎ��s\n");
	}

	auto GameScreenSize = lpSceneMng.viewArea_;
	//+1�͉E�[�Ɖ��[�̌r���`��̂���
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
	//�V�[���̓��e�̕`��
	auto camoffset = camera_->GetDrawOffset();
	DrawModiGraph(
		camoffset.x, 0,														//����
		lpSceneMng.viewArea_.x + camoffset.x, 0,							//�E��
		lpSceneMng.viewArea_.x + camoffset.x, lpSceneMng.viewArea_.y,		//�E��
		camoffset.x, lpSceneMng.viewArea_.y,								//����
		BGimage_, false);
	DrawModiGraph(
		camoffset.x + lpSceneMng.viewArea_.x, 0,													//����
		lpSceneMng.viewArea_.x + camoffset.x + lpSceneMng.viewArea_.x, 0,							//�E��
		lpSceneMng.viewArea_.x + camoffset.x + lpSceneMng.viewArea_.x, lpSceneMng.viewArea_.y,		//�E��
		camoffset.x + lpSceneMng.viewArea_.x, lpSceneMng.viewArea_.y,								//����
		BGimage_, false);
	lpMapCtl.DrawFunc();
	DrawObj();
	camera_->Draw();

	SetDrawScreen(screenScrID_);
	ClsDrawScreen();

	//����
	
	//�G�f�B�b�g�n�S�ʂ̕`��
	DrawGraph(0, 0, gameScrID_, true);
	
	/*DrawBox(harfmap.x - returnPoint + lpSceneMng.drawOffset_.x,
		harfmap.y - returnPoint + lpSceneMng.drawOffset_.y,
		harfmap.x + returnPoint + lpSceneMng.drawOffset_.x,
		harfmap.y + returnPoint + lpSceneMng.drawOffset_.y,
		GetColor(255, 255, 255), true);*/
}