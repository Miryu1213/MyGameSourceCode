#pragma once
#include <memory>
#include <list>
#include "BaseScene.h"

class UI;
class Circle;

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();

	bool Init(void) override;
	UniqueScene UpDate(UniqueScene ownScene) override;
	void Draw(void) override;

	Scene GetSceneID(void) override
	{
		return Scene::TITLE;
	};

private:
	bool UpdateDefault(void);
	void UpdateCredit(void);

	void DrawFirstImage(void);
	void DrawFadeImage(void);
	void DrawBurn(void);
	void DrawFadeUI(void);
	void DrawDefault(void);
	void DrawCredit(void);

	std::list<std::unique_ptr<UI>> btnList_;

	bool loadEndFlag_;

	double time_;
	bool firstImageEndFlag_;
	bool fadeEndFlag_;
	bool burnEndFlag_;
	bool uiFadeEndFlag_;

	double burnAnimTime_;

	bool musicFlag_;

	bool creditFlag_;
	bool creditOffFlag_;
	Vector2 creditSize_;
	double creditTime_;
	std::unique_ptr<UI> closingCredit_;
	
	Vector2 buttonPos_;
	Vector2 buttonDefPos_;

	Vector2 logoPos_;
	Vector2 logoDefPos_;
};

