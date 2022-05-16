#pragma once
#include "BaseGimmick.h"

class Hurricane :
	public BaseGimmick
{
public:
	Hurricane(Vector2 pos,Vector2 size);
	~Hurricane() override;

private:
	void Init(void) override;
	bool UpDate(void) override;
	void Draw(void) override;

	GimmickType GetGimmickType(void) override
	{
		return GimmickType::HURRICANE;
	}
};

