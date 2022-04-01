#pragma once
#include<DxLib.h>
#include<memory>
#include"JoyPad.h"
#include"KeyBoard.h"

struct InputSelecter		//joypad‚©keyboard‚©”»’f‚·‚é
{
	std::unique_ptr<Input> operator()()
	{
		std::unique_ptr<Input> Input_;
		if (GetJoypadNum() != 0)
		{
			Input_ = std::make_unique<JoyPad>();
		}
		else
		{
			Input_ = std::make_unique<KeyBoard>();
		}
		return Input_;
	}
};