#pragma once
#include "../BaseScene.h"

enum class TransType
{
    FadeInOut,
    Max
};

class Transition :
	public BaseScene
{
public:
    Transition(UniqueScene BeforeScene, UniqueScene AfterScene);
    ~Transition();

protected:
    Scene GetSceneID(void) override final
    {
        return Scene::TRANSITION;
    }

    double totalDelta_;          //合計delta値
    double limitTime_;          //シーン移行時間

    UniqueScene beforeScene_;   //前のシーン
    UniqueScene afterScene_;    //次のシーン
private:
    bool Init(void) override;
    UniqueScene UpDate(UniqueScene ownScene) override;

    virtual bool UpDateTransition(void) = 0;
    virtual void Draw(void) { return; };   //ここはダミー
};

