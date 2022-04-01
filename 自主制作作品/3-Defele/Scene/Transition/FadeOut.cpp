#include "FadeOut.h"

#include "../../_debug/_DebugConOut.h"

FadeOut::FadeOut(UniqueScene BeforeScene, UniqueScene AfterScene, double Limittime) :
	Transition::Transition(std::move(BeforeScene), std::move(AfterScene))
{
	limitTime_ = Limittime;
	beforeScene_->SetIsFade(true);
}

FadeOut::~FadeOut()
{
}

bool FadeOut::UpDateTransition(void)
{
	if (totalDelta_ > limitTime_)
	{
		//Limittime‰z‚¦‚½‚çƒV[ƒ“ˆÚs
		return true;
	}
	return false;
}

void FadeOut::Draw(void)
{
	double tmpcnt = (255.0 / limitTime_)  * totalDelta_;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - static_cast<int>(tmpcnt));
	beforeScene_->SetAlpha(255 - static_cast<int>(tmpcnt));
	beforeScene_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
