#pragma once
#include <cmath>
#include "Vector2.h"

struct Collision
{
public:
	//��`�̓����蔻��
	//��`���m
	[[nodiscard]] static bool CheckSquare(Vector2 pos, Vector2 size, Vector2 otherpos, Vector2 othersize)
	{
		if (pos.x_ <= otherpos.x_ + othersize.x_ && pos.x_ + size.x_ >= otherpos.x_)
		{
			if (pos.y_ <= otherpos.y_ + othersize.y_ && pos.y_ + size.y_ >= otherpos.y_)
			{
				return true;
			}
		}
		return false;
	}
	//��`�Ɠ_
	[[nodiscard]] static bool CheckSquare(Vector2 pos, Vector2 size, Vector2 otherpos)
	{
		if (pos <= otherpos && otherpos <= pos + size)
		{
			return true;
		}
		return false;
	}

	//�~�Ɠ_
	[[nodiscard]] static bool CheckCircle(Vector2 pos, int radius, Vector2 otherpos)
	{
		Vector2 distance = otherpos - pos;
		if (radius > sqrt(distance.x_ * distance.x_ + distance.y_ * distance.y_))
		{
			return true;
		}
		return false;
	}

	//�͈͊O
	[[nodiscard]] static bool CheckOutOfRange(Vector2 MinPos, Vector2 MaxPos, Vector2 otherpos)
	{
		if (MinPos <= otherpos && otherpos < MaxPos)
		{
			return true;
		}
		return false;
	}

	//�㉺���E�̓����蔻��
	[[nodiscard]] static bool CheckUDLR(Vector2 pos, Vector2 size, Vector2 otherpos, Vector2 othersize)
	{
		if (pos - size < otherpos + othersize && pos + size > otherpos - othersize)
		{
			return true;
		}
		return false;
	}

	//�㉺���E�̓����蔻��(����͋�`)
	[[nodiscard]] static bool CheckUDLRtoSquare(Vector2 pos, Vector2 size, Vector2 otherpos, Vector2 othersize)
	{
		if (pos - size < otherpos + othersize && pos + size > otherpos)
		{
			return true;
		}
		return false;
	}
};