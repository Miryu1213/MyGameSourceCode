#pragma once
#include <DxLib.h>
#include "../../common/Vector2.h"

enum class SignalType
{
	ALLYPLANT,
	ENEMYPLANT,
	ALLYCOREDISPLAY,
	ENEMYCOREDISPLAY,
	ALLYCOREATTACKED,
	ENEMYCOREATTACK
};

class Signal
{
public:
	Signal(Vector2 camPos, Vector2 targetPos, Vector2 camOffset, SignalType signalType);

	bool Update(Vector2 camPos ,Vector2 camOffset, bool IsTutorial = false);
	void Draw(bool isTutorial);
private:
	Vector2Flt pos_;
	
	int gh_;
	double extRate_;

	//�p�x
	float angleRad_;

	//�o�ߎ���
	double loadingTime_;
	double maxTime_;

	//�^�[�Q�b�g�̍��W
	Vector2 targetPos_;
};

