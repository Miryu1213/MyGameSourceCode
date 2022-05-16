#include<DxLib.h>
#include"JoyPad.h"

#include"../../_debug/_DebugConOut.h"

JoyPad::JoyPad()
{
	Init();
}

JoyPad::~JoyPad()
{
}

bool JoyPad::Update(obj& Obj, ObjList& objlist)
{
	for (auto& data : data_)
	{
		olddata_[data.first] = data.second;
	}
	auto PadData_ = GetJoypadInputState(DX_INPUT_PAD1);
	for (auto tableData : PadTbl_)
	{
		//map�^��first��z��̈ʒu�Ƃ��ăA�N�Z�X���A���̒��g��second�̓��e������B
		data_[tableData.first] = (PadData_ & tableData.second);
	}
	data_["�^�C�g��"] = CheckHitKey(PadTbl_["�^�C�g��"]);
	//TRACE("%d\n", data_["A"]);
	return true;
}

bool JoyPad::Init()
{
	PadTbl_["��"] = PAD_INPUT_UP;
	PadTbl_["��"] = PAD_INPUT_DOWN;
	PadTbl_["��"] = PAD_INPUT_LEFT;
	PadTbl_["�E"] = PAD_INPUT_RIGHT;
	PadTbl_["A"] = PAD_INPUT_2;
	PadTbl_["B"] = PAD_INPUT_1;
	PadTbl_["C"] = PAD_INPUT_4;
	PadTbl_["�^�C�g��"] = KEY_INPUT_F6;

	for (auto tableData : PadTbl_)
	{
		data_.emplace(tableData.first, 0);
	}

	return true;
}