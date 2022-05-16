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

    //チュートリアル表示順
    std::list<PLACE> orderList_;

    //場所ごとのチュートリアル文字列
    std::map<PLACE, std::string> stringMap_;

    UniqueScene nextScene_;
};

