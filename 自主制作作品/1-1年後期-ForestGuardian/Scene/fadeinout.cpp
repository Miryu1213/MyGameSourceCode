#include "fadeinout.h"

fadeinout::fadeinout(uniqueBaseScene beforeScene, uniqueBaseScene nextScene)
	:Trandition::Trandition(std::move(beforeScene), std::move(nextScene))
{
	Count_ = 0;
	ChangeFlag = false;
	camera_->InitArea();
	DrawOwnScreen();
}

fadeinout::~fadeinout()
{
}

void fadeinout::DrawOwnScreen(void)
{
	SetDrawScreen(screenScrID_);
	ClsDrawScreen();
	if (ChangeFlag == false)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - Count_);
		beforeScene_->Draw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		if (Count_ == 255)
		{
			Count_ = 0;
			ChangeFlag = true;
		}
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, Count_);
		nextScene_->Draw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

bool fadeinout::UpdateTrandition()
{
	Count_ += 5;
	if ((Count_ > 255) && ChangeFlag == true)
	{
		return true;
	}
	return false;
}
