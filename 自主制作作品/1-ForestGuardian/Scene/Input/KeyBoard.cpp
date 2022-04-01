#include<DxLib.h>
#include "KeyBoard.h"

KeyBoard::KeyBoard()
{
	Init();
}

KeyBoard::~KeyBoard()
{
}

bool KeyBoard::Update(obj& Obj, ObjList& objlist)
{
	for (auto& data : data_)
	{
		olddata_[data.first] = data.second;
	}
	//keydataの情報をdata_にコピー
	GetHitKeyStateAll(KeyData_);
	for (auto tableData : keyTbl_)
	{
		//map型のfirstを配列の位置としてアクセスし、その中身にsecondの内容を入れる。
		data_[tableData.first] = KeyData_[tableData.second];
	}

	return false;
}

bool KeyBoard::Init()
{
	keyTbl_["上"] = KEY_INPUT_UP;
	keyTbl_["下"] = KEY_INPUT_DOWN;
	keyTbl_["左"] = KEY_INPUT_LEFT;
	keyTbl_["右"] = KEY_INPUT_RIGHT;
	keyTbl_["A"] = KEY_INPUT_A;
	keyTbl_["B"] = KEY_INPUT_S;
	keyTbl_["C"] = KEY_INPUT_D;
	keyTbl_["タイトル"] = KEY_INPUT_F6;

	for (auto tableData : keyTbl_)
	{
		data_.emplace(tableData.first, 0);
	}
	return true;
}
