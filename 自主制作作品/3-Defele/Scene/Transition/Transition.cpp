#include "Transition.h"
#include "../SceneMng.h"

Transition::Transition(UniqueScene BeforeScene, UniqueScene AfterScene)
{
	//二つのシーンの情報を貰う
	beforeScene_ = std::move(BeforeScene);
	afterScene_ = std::move(AfterScene);

	//初期化
	totalDelta_ = 0;

	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	//最後のDrawの状態を一瞬描画
	beforeScene_->Draw();
}

Transition::~Transition()
{
}

bool Transition::Init(void)
{
	return false;
}

UniqueScene Transition::UpDate(UniqueScene ownScene)
{
	totalDelta_ += lpSceneMng.GetDeltaTime();
	if (UpDateTransition())
	{
		return std::move(afterScene_);
	}
	//自身の描画をセット
	return ownScene;
}
