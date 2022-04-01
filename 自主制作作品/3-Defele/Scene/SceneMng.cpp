#include <DxLib.h>
#include <chrono>
#include "SceneMng.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "ResultScene.h"

#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"

//�A�C�R��ID
#define ICON_ID 100

//�����S�̃T�C�Y
constexpr int Def_World_Size_X = 1024;
constexpr int Def_World_Size_Y = 768;

//�����\���G���A
constexpr int Def_View_Area_X = 1024;
constexpr int Def_View_Area_Y = 768;

//��ʃI�t�Z�b�g
constexpr int GAME_OFFSET = 60;

using namespace std::chrono;

void SceneMng::Run(void)
{
	if (!Init())
	{
		return;
	}
	//�����V�[��
	scene_ = std::make_unique<TitleScene>();
	//scene_ = std::make_unique<ResultScene>(ArmyType::ALLY);

	//���ԕۑ��n
	system_clock::time_point StartTime = system_clock::now();									//�J�n���̓���
	microseconds microstarttime = duration_cast<milliseconds>(StartTime.time_since_epoch());	//���݂̃V�X�e������
	system_clock::time_point nowTime;															//���݂̓���
	microseconds elaptime = microstarttime;
	microseconds oldelaptime;																	//��O�̎���

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && !gameEndFlag_)
	{
		_dbgStartDraw();
		oldelaptime = elaptime;																	//��O�̎��Ԃ̍X�V
		nowTime = system_clock::now();															//���݂̓���
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
		//��d�`�F�b�N
		if (!SysInit())
		{
			TRACE("SysInit���s\n");
			return false;
		}
	}

	//�J�[�\���̍쐬
	cursol_ = std::make_shared<Cursol>();

	//�J�[�\���̏�����
	cursol_->Init();

	return true;
}

void SceneMng::UpDate(void)
{
	auto oldScene = scene_->GetSceneID();
	scene_ = scene_->UpDate(std::move(scene_));
	if (oldScene != scene_->GetSceneID())
	{
		//�V�[�����ς�����Ƃ��̃��Z�b�g
		//�o�ߎ���
 		totaldelta_ = 0.0;
		//�{��
		doubleValue_ = 1.0;
		//�J�[�\��
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
	//�����T�C�Y�̐ݒ�
	worldSize_ = { Def_World_Size_X,Def_World_Size_Y };
	viewArea_ = { Def_View_Area_X,Def_View_Area_Y };

	//�I�t�Z�b�g�ݒ�
	offset_ = GAME_OFFSET;

	//�{���̒l
	doubleValue_ = 1.0;

	//DxLib�֘A
	SetGraphMode(Def_View_Area_X, Def_View_Area_Y, 32);		//��ʂ̃T�C�Y�A�F�[�x(�\����)
	ChangeWindowMode(true);									//true��window false�Ńt���X�N���[��
	if (DxLib_Init() == -1)
	{
		return false;
	}

	//�X�g���[���Đ�
	SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS_PLUS);

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
