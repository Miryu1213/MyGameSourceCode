#include <DxLib.h>
#include "BaseScene.h"
#include "SceneMng.h"

BaseScene::BaseScene()
{
	//カメラの作成
	camera_ = std::make_shared<Camera>();
	camera_->SetTarget(std::move(lpSceneMng.cursol_));
}
