#pragma once
#include "BaseScene.h"
#include "Obj/Type/ArmyType.h"

class UI;

class ResultScene :
	public BaseScene
{
public:
	ResultScene(ArmyType breakarmycore);
	~ResultScene();

	bool Init(void) override;
	UniqueScene UpDate(UniqueScene ownScene) override;
	void Draw(void) override;

	Scene GetSceneID(void) override
	{
		return Scene::RESULT;
	};

private:
	//破壊されたコア
	ArmyType destroyedCore_;

	std::list<std::unique_ptr<UI>> btnList_;

	//表示テキスト
	bool textScrEndFlag_;
	double textTime_;
	Vector2 textPos_;
	Vector2 textDefPos_;

	Vector2 btnPos_;
	Vector2 btnDefPos_;
};

