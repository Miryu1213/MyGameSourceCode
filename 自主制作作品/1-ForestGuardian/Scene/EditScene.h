#pragma once
#include "BaseScene.h"
#include"Obj/EditCar.h"

class EditScene :
	public BaseScene
{
public:
	EditScene();
	~EditScene();

private:

	uniqueBaseScene UpDate(uniqueBaseScene own) override;
	SceneID GetSceneID() override;

	bool Init() override;
	void DrawOwnScreen(void) override;	//自身のシーンの描画
	int gameScrID_;		//オフセット抜きの画面
	int BGimage_;
};

