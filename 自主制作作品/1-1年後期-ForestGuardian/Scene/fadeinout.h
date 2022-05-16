#pragma once
#include "Trandition.h"

class fadeinout :
	public Trandition
{
public:
	fadeinout(uniqueBaseScene beforeScene, uniqueBaseScene nextScene);
	~fadeinout();
private:
	int Count_;
	bool ChangeFlag;
	void DrawOwnScreen(void) override;
	bool UpdateTrandition() override;
};


