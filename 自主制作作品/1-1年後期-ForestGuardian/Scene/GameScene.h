#pragma once
#include<string>
#include "BaseScene.h"

#define MAX_TIME 60
#define POP_COUNT 5
#define EFF_MAXTIME 180

class GameScene :
    public BaseScene
{
public:
	GameScene();
	~GameScene();

private:
	uniqueBaseScene UpDate(uniqueBaseScene own) override;
	SceneID GetSceneID() override;
	bool Init() override;
	void DrawOwnScreen(void) override;
	void DrawBlackHall();
	int gameScrID_;
	int BGimage_;		//背景のグラフィックハンドル

	int count_;

	int filterimg_;

	//int wakuimg_;
	int chapimg_;		//キャプチャ用
	bool dethFlag;
	int gameoverimg_;
	bool thankflag;
	int clearimg_;
	bool clearflag;
	int thankimg_;

	bool doFlag;
	bool dodoflag;

	int descimg_;
	bool descflag;

	int animcnt;
	int blackhallimg_[20];
	int lighteffimg_[15];
	int effcnt;
	bool effFlag;

	void enemypopfre();	//敵出現
	void enemypop(int poptype);
};

