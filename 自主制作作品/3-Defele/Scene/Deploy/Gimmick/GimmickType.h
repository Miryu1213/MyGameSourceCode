#pragma once
#include <type_traits>

enum class GimmickType
{
	HURRICANE,
	MAX
};

static GimmickType begin(GimmickType)
{
	return GimmickType::HURRICANE;
}

static GimmickType end(GimmickType)
{
	return GimmickType::MAX;
}

static GimmickType operator++(GimmickType& input)
{
	//return static_cast<InputID>(static_cast<int>(input) + 1);
	return (input = static_cast<GimmickType>(std::underlying_type<GimmickType>::type(input) + 1));
}

static GimmickType operator*(GimmickType& input)
{
	return input;
}
