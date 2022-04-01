#pragma once
#include <type_traits>

enum class InputType
{
	OLD,
	NOW
};

enum class MouseInputID
{
	POSX,
	POSY,
	LEFT,
	RIGHT,
	MIDDLE,
	BUTTON_X,
	BUTTON_Y,
	MAX
};

static MouseInputID begin(MouseInputID)
{
	return MouseInputID::POSX;
}

static MouseInputID end(MouseInputID)
{
	return MouseInputID::MAX;
}

static MouseInputID operator++(MouseInputID& input)
{
	//return static_cast<InputID>(static_cast<int>(input) + 1);
	return (input = static_cast<MouseInputID>(std::underlying_type<MouseInputID>::type(input) + 1));
}

static MouseInputID operator*(MouseInputID& input)
{
	return input;
}