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
	//oldへコピー
	for (auto& data : Inputdata_)
	{
		oldInputdata_[data.first] = data.second;
	}

	//マウスの更新
	mouse_.Update();

	//パッドの情報の取得
	auto PadData_ = GetJoypadInputState(DX_INPUT_PAD1);

	//格納した入力データを外部アクセス可能位置へ移動
	for (auto& tableData : PadTbl_)
	{
		Inputdata_[tableData.first] = (PadData_ & tableData.second);
	}
	return true;
}

bool JoyPad::Init()
{
	//キー登録
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

	//外部アクセス用のキーデータの初期化
	for (auto& tableData : PadTbl_)
	{
		Inputdata_.emplace(tableData.first, 0);
	}

	//マウスの初期化
	mouse_.Init();

	return true;
}
