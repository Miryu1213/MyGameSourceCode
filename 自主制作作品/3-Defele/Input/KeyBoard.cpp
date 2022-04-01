#include<DxLib.h>
#include "KeyBoard.h"

KeyBoard::KeyBoard()
{
	Init();
}

KeyBoard::~KeyBoard()
{
}

bool KeyBoard::Update(void)
{
	//old�փR�s�[
	for (auto& data : Inputdata_)
	{
		oldInputdata_[data.first] = data.second;
	}

	//�}�E�X�̍X�V
	mouse_.Update();

	//�L�[�f�[�^�ɓ��͏��̊i�[
	for (auto inputid = InputID::UP; inputid != InputID::MAX; (++inputid))
	{
		if (CheckHitKey(keyTbl_[inputid]))
		{
			KeyData_[static_cast<int>(inputid)] = 1;
		}
		else
		{
			KeyData_[static_cast<int>(inputid)] = 0;
		}
	}

	//�i�[�������̓f�[�^���O���A�N�Z�X�\�ʒu�ֈړ�
	for (auto& tableData : keyTbl_)
	{
		Inputdata_[tableData.first] = KeyData_[static_cast<int>(tableData.first)];
	}

	return true;
}

bool KeyBoard::Init()
{
	//�L�[�o�^
	keyTbl_[InputID::UP] = KEY_INPUT_UP;
	keyTbl_[InputID::DOWN] = KEY_INPUT_DOWN;
	keyTbl_[InputID::LEFT] = KEY_INPUT_LEFT;
	keyTbl_[InputID::RIGHT] = KEY_INPUT_RIGHT;
	keyTbl_[InputID::A] = KEY_INPUT_A;

	//�O���A�N�Z�X�p�̃L�[�f�[�^�̏�����
	for (auto& tableData : keyTbl_)
	{
		Inputdata_.emplace(tableData.first, 0);
	}

	//�}�E�X�̏�����
	mouse_.Init();

	return true;
}
