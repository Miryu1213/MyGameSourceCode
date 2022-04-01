#pragma once
#include "Transition.h"
class FadeInOut :
	public Transition
{
public:
    FadeInOut(UniqueScene BeforeScene, UniqueScene AfterScene, double Limittime);
    ~FadeInOut();
private:
    bool UpDateTransition(void) override;
    void Draw(void) override;

    bool ChangeFlag;
};

