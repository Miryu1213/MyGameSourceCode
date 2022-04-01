#pragma once
#include "Input.h"
#include"../../common/vector2.h"

class Mouse :
	public Input
{
public:
	Mouse();
	~Mouse();
private:
	bool Update(obj& Obj, ObjList& objlist) override;
	bool Init();
	Vector2 offset_;
protected:

};
