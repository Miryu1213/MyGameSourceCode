#include<DxLib.h>
#include "Mouse.h"

#include"../../_debug/_DebugConOut.h"

Mouse::Mouse()
{
	TRACE("Mouseの作成\n");
	Init();
	offset_.x = 0;
	offset_.y = 0;
}

Mouse::~Mouse()
{
	TRACE("Mouseの削除\n");
}

bool Mouse::Update(obj& Obj, ObjList& objlist)
{
	for (auto& data : data_)
	{
		olddata_[data.first] = data.second;
	}
	GetMousePoint(&data_["X座標"], &data_["Y座標"]);
	data_["X座標"] -= offset_.x;
	data_["Y座標"] -= offset_.y;
	//TRACE("X座標：%d,Y座標：%d", data_["X座標"], data_["Y座標"]);
	data_["ボタン"] = GetMouseInput();
	data_["ホイール"] = GetMouseWheelRotVol();
	data_["セーブ"] = CheckHitKey(KEY_INPUT_F8);
	data_["ロード"] = CheckHitKey(KEY_INPUT_F7);
	data_["プレイ"] = CheckHitKey(KEY_INPUT_F5);
	data_["X"] = CheckHitKey(KEY_INPUT_X);
	return true;
}

bool Mouse::Init()
{
	data_["X座標"] = 0;
	data_["Y座標"] = 0;
	data_["ボタン"] = 0;
	data_["ホイール"] = 0;
	data_["セーブ"] = 0;//F8
	data_["ロード"] = 0;//F7
	data_["プレイ"] = 0;//F5
	data_["X"] = 0;
	return true;
}