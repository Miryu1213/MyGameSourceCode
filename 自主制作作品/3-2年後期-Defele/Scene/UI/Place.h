#pragma once
#include <type_traits>

enum class PLACE
{
	FIRST,
	PLANT,
	UNIT,
	CORE,
    BUTTON,
    WARSITUATION,
    DEPLOY,
	ENEMYPLANT,
    MAX
};

static PLACE begin(PLACE)
{
	return PLACE::FIRST;
}

static PLACE end(PLACE)
{
	return PLACE::MAX;
}

static PLACE operator++(PLACE& input)
{
	//return static_cast<InputID>(static_cast<int>(input) + 1);
	return (input = static_cast<PLACE>(std::underlying_type<PLACE>::type(input) + 1));
}

static PLACE operator*(PLACE& input)
{
	return input;
}