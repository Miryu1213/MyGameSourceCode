#pragma once
#include "Input.h"

class JoyPad :
    public Input
{
public:
	JoyPad();
	~JoyPad();
	bool Update(obj& obj, ObjList& objlist) override;
private:
	bool Init();
	std::map<std::string, int> PadTbl_;		//パッドテーブル
};

