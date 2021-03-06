#include<DxLib.h>
#include "Trandition.h"

#include"../_debug/_DebugConOut.h"

Trandition::Trandition(uniqueBaseScene beforeScene, uniqueBaseScene nextScene)
{
	beforeScene_ = std::move(beforeScene);
	nextScene_ = std::move(nextScene);
	TRACE("TranditionSceneの生成\n");
}

Trandition::~Trandition()
{
}

uniqueBaseScene Trandition::UpDate(uniqueBaseScene own)
{
	DrawOwnScreen();
	if (UpdateTrandition() == true)
	{
		TRACE("TranditionSceneの終了\n");
		return std::move(nextScene_);
	}
	return own;
}


SceneID Trandition::GetSceneID()
{
	return SceneID::TRANDITION;
}

bool Trandition::Init(void)
{
	return false;
}
