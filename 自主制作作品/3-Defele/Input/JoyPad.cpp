#include<DxLib.h>
#include "Joypad.h"

JoyPad::JoyPad()
{
	Init();
}

JoyPad::~JoyPad()
{
}

bool JoyPad::Update(void)
{
	//old�փR�s�[
	for (auto& data : Inputdata_)
	{
		oldInputdata_[data.first] = data.second;
	}

	//�}�E�X�̍X�V
	mouse_.Update();

	//�p�b�h�̏��̎擾
	auto PadData_ = GetJoypadInputState(DX_INPUT_PAD1);

	//�i�[�������̓f�[�^���O���A�N�Z�X�\�ʒu�ֈړ�
	for (auto& tableData : PadTbl_)
	{
		Inputdata_[tableData.first] = (PadData_ & tableData.second);
	}
	return true;
}

bool JoyPad::Init()
{
	//�L�[�o�^
	PadTbl_[InputID::UP] = PAD_INPUT_UP;
	PadTbl_[InputID::DOWN] = PAD_INPUT_DOWN;
	PadTbl_[InputID::LEFT] = PAD_INPUT_LEFT;
	PadTbl_[InputID::RIGHT] = PAD_INPUT_RIGHT;
	PadTbl_[InputID::A] = PAD_INPUT_2;
	PadTbl_[InputID::B] = PAD_INPUT_1;
	PadTbl_[InputID::X] = PAD_INPUT_4;
	PadTbl_[InputID::Y] = PAD_INPUT_3;
	PadTbl_[InputID::START] = PAD_INPUT_8;
	PadTbl_[InputID::RB] = PAD_INPUT_6;
	PadTbl_[InputID::LB] = PAD_INPUT_5;

	//�O���A�N�Z�X�p�̃L�[�f�[�^�̏�����
	for (auto& tableData : PadTbl_)
	{
		Inputdata_.emplace(tableData.first, 0);
	}

	//�}�E�X�̏�����
	mouse_.Init();

	return true;
}
