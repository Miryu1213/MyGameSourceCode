#pragma once
#include <type_traits>

enum class InputID
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	A,
	B,
	X,
	Y,
	START,
	RB,
	LB,
	MAX
};

static InputID begin(InputID)
{
	return InputID::UP;
}

static InputID end(InputID)
{
	return InputID::MAX;
}

static InputID operator++(InputID& input)
{
	//return static_cast<InputID>(static_cast<int>(input) + 1);
	return (input = static_cast<InputID>(std::underlying_type<InputID>::type(input) + 1));
}

static InputID operator*(InputID& input)
{
	return input;
}