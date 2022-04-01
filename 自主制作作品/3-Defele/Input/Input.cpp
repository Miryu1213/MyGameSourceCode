#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

const InputData& Input::GetInputData(InputType type)
{
	if (type == InputType::NOW)
	{
		return Inputdata_;
	}
	else
	{
		return oldInputdata_;
	}
}

const MouseInputData Input::GetMouseData(InputType type)
{
	return mouse_.GetMouseData(type);
}

void Input::SetMousePos(Vector2 pos)
{
	mouse_.SetMouseInputPos(pos);
}
