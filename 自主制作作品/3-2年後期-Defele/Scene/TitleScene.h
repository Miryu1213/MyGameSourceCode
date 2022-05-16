#pragma once
#include <functional>
#include <memory>
#include <map>
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

	//現在の画面
	enum class CURSCREEN
	{
		NONE,
		START,
		FADE,
		BURN,
		FADEUI,
		DEFAULT,
		CREDIT
	};

	void UpdateDefault(void);
	void UpdateCredit(void);

	void DrawFirstImage(void);	//初期画面（1）
	void DrawFadeImage(void);	//フェード画面（2）
	void DrawBurn(void);		//炎上画面（3）
	void DrawFadeUI(void);		//フェードUI画面（4）
	void DrawDefault(void);		//通常メニュー画面（5）
	void DrawCredit(void);		//クレジット画面

	CURSCREEN curScreen_;		//現在のスクリーン

	//各画面の更新
	std::map<CURSCREEN, std::function<void(void)>> update_;
	std::map<CURSCREEN, std::function<void(void)>> draw_;

	//メニュー画面でのボタンリスト
	std::list<std::unique_ptr<UI>> btnList_;

	double time_;		//時間管理用タイム
	bool musicFlag_;	//音楽再生
	bool toSelectFlag_;	//セレクトシーンへ

	//クレジット
	std::unique_ptr<UI> closingCredit_;		//クレジットUI
	
	//メニューボタン
	Vector2 buttonPos_;
	Vector2 buttonDefPos_;

	//ロゴ
	Vector2 logoPos_;
	Vector2 logoDefPos_;
};

