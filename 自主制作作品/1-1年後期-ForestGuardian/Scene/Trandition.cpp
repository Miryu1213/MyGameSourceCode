#include<DxLib.h>
#include "Trandition.h"

#include"../_debug/_DebugConOut.h"

Trandition::Trandition(uniqueBaseScene beforeScene, uniqueBaseScene nextScene)
{
	beforeScene_ = std::move(beforeScene);
	nextScene_ = std::move(nextScene);
	TRACE("TranditionSceneÇÃê∂ê¨\n");
}

Trandition::~Trandition()
{
}

uniqueBaseScene Trandition::UpDate(uniqueBaseScene own)
{
	DrawOwnScreen();
	if (UpdateTrandition() == true)
	{
		TRACE("TranditionSceneÇÃèIóπ\n");
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
