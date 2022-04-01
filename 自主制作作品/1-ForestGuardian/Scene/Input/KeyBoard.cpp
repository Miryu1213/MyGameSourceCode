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
	//keydata�̏���data_�ɃR�s�[
	GetHitKeyStateAll(KeyData_);
	for (auto tableData : keyTbl_)
	{
		//map�^��first��z��̈ʒu�Ƃ��ăA�N�Z�X���A���̒��g��second�̓��e������B
		data_[tableData.first] = KeyData_[tableData.second];
	}

	return false;
}

bool KeyBoard::Init()
{
	keyTbl_["��"] = KEY_INPUT_UP;
	keyTbl_["��"] = KEY_INPUT_DOWN;
	keyTbl_["��"] = KEY_INPUT_LEFT;
	keyTbl_["�E"] = KEY_INPUT_RIGHT;
	keyTbl_["A"] = KEY_INPUT_A;
	keyTbl_["B"] = KEY_INPUT_S;
	keyTbl_["C"] = KEY_INPUT_D;
	keyTbl_["�^�C�g��"] = KEY_INPUT_F6;

	for (auto tableData : keyTbl_)
	{
		data_.emplace(tableData.first, 0);
	}
	return true;
}
