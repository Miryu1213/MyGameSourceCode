#pragma once
#include "BaseScene.h"
#include"../common/Easing.h"

class TitleScene :
    public BaseScene
{
public:
	TitleScene();
	~TitleScene();

private:
	uniqueBaseScene UpDate(uniqueBaseScene own) override;
	SceneID GetSceneID() override;
	bool Init() override;
	void DrawOwnScreen(void) override;
	int titleScrID_;

	int titleimg;
	int startimg;
	int creditimg;
	int exitimg;
	int wakuimg;

	Vector2 buttonpos;
	int selectbutton;

	bool oldflag;
	bool upflag;
	bool startFlag;
	bool exitFlag;

	bool Drawflag;

	Easing easing_;
	double doublecnt;	//ダブル型のカウント。一フレーム0.05増加
	float maxtime;		//ここの時間が一周期の四分の一。つまり最大振幅かつ移動終了時間となる
	double returnPoint;	//引数から戻ってくる値

	bool WideFlag;
	bool NarrowFlag;

	void SizeUpUI(double time, double totaltime);	//徐々に広がっていくUI
};

