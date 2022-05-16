#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

const InputData& Input::GetData(Inputtype type)
{
	if (type == Inputtype::NOW)
	{
		return data_;
	}
	else
	{
		return olddata_;
	}
}
