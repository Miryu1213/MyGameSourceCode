#include "Transition.h"
#include "../SceneMng.h"

Transition::Transition(UniqueScene BeforeScene, UniqueScene AfterScene)
{
	//��̃V�[���̏���Ⴄ
	beforeScene_ = std::move(BeforeScene);
	afterScene_ = std::move(AfterScene);

	//������
	totalDelta_ = 0;

	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	//�Ō��Draw�̏�Ԃ���u�`��
	beforeScene_->Draw();
}

Transition::~Transition()
{
}

bool Transition::Init(void)
{
	return false;
}

UniqueScene Transition::UpDate(UniqueScene ownScene)
{
	totalDelta_ += lpSceneMng.GetDeltaTime();
	if (UpDateTransition())
	{
		return std::move(afterScene_);
	}
	//���g�̕`����Z�b�g
	return ownScene;
}
