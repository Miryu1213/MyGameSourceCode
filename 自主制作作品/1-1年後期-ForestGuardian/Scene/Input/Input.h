#pragma once
#include<map>
#include<string>
#include"../BaseScene.h"

enum class Inputtype
{
	OLD,
	NOW
};

using InputData = std::map<std::string, int>;

class obj;

class Input
{
public:
	Input();
	virtual ~Input();
	virtual bool Update(obj& Obj, ObjList& objlist) = 0;
	//���͋@��̊m�F
	const InputData& GetData(Inputtype type);
private:
protected:
	InputData data_;		//���̓L�[
	InputData olddata_;		//�ЂƂO�̓��̓L�[
};


