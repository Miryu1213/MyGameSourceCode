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
	void DrawOwnScreen(void) override;	//���g�̃V�[���̕`��
	int gameScrID_;		//�I�t�Z�b�g�����̉��
	int BGimage_;
};

