#include <DxLib.h>
#include "BaseScene.h"
#include "SceneMng.h"

BaseScene::BaseScene()
{
	//�J�����̍쐬
	camera_ = std::make_shared<Camera>();
	camera_->SetTarget(std::move(lpSceneMng.cursol_));
}
