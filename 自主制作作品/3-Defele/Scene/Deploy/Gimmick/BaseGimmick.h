#pragma once
#include "GimmickType.h"
#include "../../../common/Collision.h"
#include "../../../common/Vector2.h"

class BaseGimmick
{
public:
	BaseGimmick();
	virtual ~BaseGimmick() {};

	virtual void Init(void) = 0;
	virtual bool UpDate(void) = 0;
	virtual void Draw(void) = 0;

	virtual GimmickType GetGimmickType(void) = 0;

	Vector2 GetPos(void)
	{
		return pos_;
	}

	Vector2 GetSize(void)
	{
		return size_;
	}

	bool aliveFlag_;

protected:
	Vector2 pos_;
	Vector2 size_;

	double elapTime_;
};

