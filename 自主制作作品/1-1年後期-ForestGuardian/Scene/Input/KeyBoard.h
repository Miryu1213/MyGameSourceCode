#pragma once
#include"Input.h"

class KeyBoard:
	public Input
{
public:
	KeyBoard();
	~KeyBoard();
private:
	bool Update(obj& Obj, ObjList& objlist) override;
	bool Init();

	std::map<std::string, int> keyTbl_;		//�L�[�e�[�u��
	char KeyData_[256];
};

