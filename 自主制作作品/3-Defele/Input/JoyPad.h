#pragma once
#include "Input.h"

class JoyPad :
	public Input
{
public:
	JoyPad();
	~JoyPad();
	
private:
	bool Update(void) override;
	bool Init(void) override;

	InputClass GetInputClass(void) override
	{
		return InputClass::PAD;
	}

	std::map<InputID, int> PadTbl_;		//パッドテーブル
};