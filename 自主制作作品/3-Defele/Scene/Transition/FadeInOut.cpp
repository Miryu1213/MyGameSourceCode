#include <DxLib.h>
#include "FadeInOut.h"
#include "../SceneMng.h"

FadeInOut::FadeInOut(UniqueScene BeforeScene, UniqueScene AfterScene, double Limittime) :
	Transition::Transition(std::move(BeforeScene), std::move(AfterScene))
{
	limitTime_ = Limittime;
	ChangeFlag = false;
}

FadeInOut::~FadeInOut()
{
}

bool FadeInOut::UpDateTransition(void)
{
	if (totalDelta_ > limitTime_)
	{
		//Limittime‰z‚¦‚½‚çƒV[ƒ“ˆÚs
		return true;
	}
	return false;
}

void FadeInOut::Draw(void)
{
	double tmpcnt = 255.0 / (limitTime_ / 2.0) * totalDelta_;
	if (ChangeFlag == false)
	{
		//limittime‚Ì”¼•ª‚ÌŠÔ
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - static_cast<int>(tmpcnt));
		beforeScene_->Draw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		if ((limitTime_ / 2) < totalDelta_)
		{
			totalDelta_ = 0;
			ChangeFlag = true;
		}
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(tmpcnt));
		afterScene_->Draw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		if ((limitTime_ / 2) < totalDelta_)
		{
			totalDelta_ *= 2.0;
		}
	}
	return;
}