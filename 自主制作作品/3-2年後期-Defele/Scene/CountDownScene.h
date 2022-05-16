#pragma once
#include <thread>
#include "BaseScene.h"
#include "../common/Directing/MoveObject.h"

class CountDownScene :
	public BaseScene
{
public:
	CountDownScene(UniqueScene gameScene);
	~CountDownScene();

	bool Init(void) override;
	UniqueScene UpDate(UniqueScene ownScene) override;
	void Draw(void) override;

	Scene GetSceneID(void) override
	{
		return Scene::COUNTDOWN;
	};

private:
	UniqueScene nextScene_;
	std::thread initThread_;		//�X���b�h
	double totalTimeCnt_;			//���ԃJ�E���g

	//����
	char* cntString_;

	MoveObjectBoolVoid* downObj_;	//�����Ă���Object
};
