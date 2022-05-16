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

	//角度
	float angleRad_;

	//経過時間
	double loadingTime_;
	double maxTime_;

	//ターゲットの座標
	Vector2 targetPos_;
};

