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
	SetDrawScreen(DX_SCREEN_BACK);			// �o�b�N�o�b�t�@�ɐݒ�
	ClsDrawScreen();						// ��ʏ���

	scene_->Draw();
	_dbgAddDraw();			//��ʒǉ������i�X���[�Ȃǁj
	ScreenFlip();			 // ����ʂ�\��ʂɏu�ԃR�s�[
}

bool SceneMng::SysInit(void)
{
	// �V�X�e������
	SetWindowText("ForestGuardian");
	// �V�X�e������
	SetGraphMode(scnArea_.x, scnArea_.y, 16);		// 800�~600�h�b�g65536�F���[�h�ɐݒ�
	ChangeWindowMode(true);								// true:window false:�t���X�N���[��
	if (DxLib_Init() == -1)
	{
		return false;									// DX���C�u�����̏���������
	}
	_dbgSetup(scnArea_.x, scnArea_.y, 200);			//�f�o�b�O���C�u�����̏�����

	srand((unsigned int)time(NULL));		//�R���s���[�^�̎��Ԃ�srand�ɓ���A�^���I�Ƀ����_����

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
