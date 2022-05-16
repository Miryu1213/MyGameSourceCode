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

	std::map<InputID, int> keyTbl_;		//キーテーブル
	char KeyData_[(int)InputID::MAX];	//キー情報まとめ
};

