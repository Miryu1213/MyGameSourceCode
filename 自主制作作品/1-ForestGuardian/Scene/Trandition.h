#pragma once
#include<memory>
#include "BaseScene.h"

enum class Tdtion
{
	CrossOver,
	fadeinout,
	Split,
	max
};

class Trandition :
	public BaseScene
{
public:
	Trandition(uniqueBaseScene beforeScene, uniqueBaseScene nextScene);
	~Trandition();
private:
	virtual bool UpdateTrandition() = 0;

	uniqueBaseScene UpDate(uniqueBaseScene own) override;
	SceneID GetSceneID() override;
	bool Init(void) override;

protected:
	uniqueBaseScene beforeScene_;
	uniqueBaseScene nextScene_;
};
