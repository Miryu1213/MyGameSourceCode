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

    double totalDelta_;          //���vdelta�l
    double limitTime_;          //�V�[���ڍs����

    UniqueScene beforeScene_;   //�O�̃V�[��
    UniqueScene afterScene_;    //���̃V�[��
private:
    bool Init(void) override;
    UniqueScene UpDate(UniqueScene ownScene) override;

    virtual bool UpDateTransition(void) = 0;
    virtual void Draw(void) { return; };   //�����̓_�~�[
};

