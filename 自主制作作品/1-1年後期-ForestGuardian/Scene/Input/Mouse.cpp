#include<DxLib.h>
#include "Mouse.h"

#include"../../_debug/_DebugConOut.h"

Mouse::Mouse()
{
	TRACE("Mouse�̍쐬\n");
	Init();
	offset_.x = 0;
	offset_.y = 0;
}

Mouse::~Mouse()
{
	TRACE("Mouse�̍폜\n");
}

bool Mouse::Update(obj& Obj, ObjList& objlist)
{
	for (auto& data : data_)
	{
		olddata_[data.first] = data.second;
	}
	GetMousePoint(&data_["X���W"], &data_["Y���W"]);
	data_["X���W"] -= offset_.x;
	data_["Y���W"] -= offset_.y;
	//TRACE("X���W�F%d,Y���W�F%d", data_["X���W"], data_["Y���W"]);
	data_["�{�^��"] = GetMouseInput();
	data_["�z�C�[��"] = GetMouseWheelRotVol();
	data_["�Z�[�u"] = CheckHitKey(KEY_INPUT_F8);
	data_["���[�h"] = CheckHitKey(KEY_INPUT_F7);
	data_["�v���C"] = CheckHitKey(KEY_INPUT_F5);
	data_["X"] = CheckHitKey(KEY_INPUT_X);
	return true;
}

bool Mouse::Init()
{
	data_["X���W"] = 0;
	data_["Y���W"] = 0;
	data_["�{�^��"] = 0;
	data_["�z�C�[��"] = 0;
	data_["�Z�[�u"] = 0;//F8
	data_["���[�h"] = 0;//F7
	data_["�v���C"] = 0;//F5
	data_["X"] = 0;
	return true;
}