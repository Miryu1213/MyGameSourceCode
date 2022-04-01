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
	//oldへコピー
	for (auto& data : Inputdata_)
	{
		oldInputdata_[data.first] = data.second;
	}

	//マウスの更新
	mouse_.Update();

	//キーデータに入力情報の格納
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

	//格納した入力データを外部アクセス可能位置へ移動
	for (auto& tableData : keyTbl_)
	{
		Inputdata_[tableData.first] = KeyData_[static_cast<int>(tableData.first)];
	}

	return true;
}

bool KeyBoard::Init()
{
	//キー登録
	keyTbl_[InputID::UP] = KEY_INPUT_UP;
	keyTbl_[InputID::DOWN] = KEY_INPUT_DOWN;
	keyTbl_[InputID::LEFT] = KEY_INPUT_LEFT;
	keyTbl_[InputID::RIGHT] = KEY_INPUT_RIGHT;
	keyTbl_[InputID::A] = KEY_INPUT_A;

	//外部アクセス用のキーデータの初期化
	for (auto& tableData : keyTbl_)
	{
		Inputdata_.emplace(tableData.first, 0);
	}

	//マウスの初期化
	mouse_.Init();

	return true;
}
