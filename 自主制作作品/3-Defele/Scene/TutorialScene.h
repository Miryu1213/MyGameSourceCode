#pragma once
#include <DxLib.h>
#include "BaseScene.h"
#include "GameScene.h"
#include "UI/Place.h"

class TutorialScene :
	public BaseScene
{
public:
	TutorialScene(const char* mapname, UniqueScene gameScene);
    ~TutorialScene() override;

private:
    bool Init(void) override;
    void InitMusic(void) override;
    UniqueScene UpDate(UniqueScene ownScene) override;
 
    void Draw(void) override;

    Scene GetSceneID(void) override
    {
        return Scene::TUTORIAL;
    };

    //�`���[�g���A���\����
    std::list<PLACE> orderList_;

    //�ꏊ���Ƃ̃`���[�g���A��������
    std::map<PLACE, std::string> stringMap_;

    //�S�Ẵe�L�X�g
    //std::string allMessage_;
    //���ݏo�̓e�L�X�g
    //std::string message_;

    UniqueScene nextScene_;
};

