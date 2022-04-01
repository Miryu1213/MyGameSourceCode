#pragma once
#include "../../common/Vector2.h"

constexpr int MINIMAP_SIZE_X = 120;
constexpr int MINIMAP_SIZE_Y = 80;

class MiniMap
{
public:
	MiniMap(Vector2 pos);
	~MiniMap();

	void Draw(void);
private:
	Vector2 pos_;
	Vector2 size_;

	//ŽB‰eˆÊ’u
	Vector2 shotingLocation_;
};

