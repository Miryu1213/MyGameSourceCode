#pragma once
#include "Input.h"
class KeyBoard :
	public Input
{
public:
	KeyBoard();
	~KeyBoard();
private:
	bool Update(void) override;
	bool Init(void) override;

	InputClass GetInputClass(void) override
	{
		return InputClass::KETBOARD;
	}

	std::map<InputID, int> keyTbl_;		//�L�[�e�[�u��
	char KeyData_[(int)InputID::MAX];	//�L�[���܂Ƃ�
};

