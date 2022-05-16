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
	double doublecnt;	//�_�u���^�̃J�E���g�B��t���[��0.05����
	float maxtime;		//�����̎��Ԃ�������̎l���̈�B�܂�ő�U�����ړ��I�����ԂƂȂ�
	double returnPoint;	//��������߂��Ă���l

	bool WideFlag;
	bool NarrowFlag;

	void SizeUpUI(double time, double totaltime);	//���X�ɍL�����Ă���UI
};

