#pragma once
#include "Transition.h"

class FadeOut :
	public Transition
{
public:
    FadeOut(UniqueScene BeforeScene, UniqueScene AfterScene, double Limittime);
    ~FadeOut();
private:
    bool UpDateTransition(void) override;
    void Draw(void) override;
};

