#include <string>
#include "CountDownScene.h"
#include "SceneMng.h"
#include "../common/FontMng.h"
#include "../common/AudioMng.h"

const char* ONE = "1";
const char* TWO = "2";
const char* THREE = "3";
const char* GO = "GO!";

constexpr double STR_INTERVAL = 1.0;
constexpr double STR_SOUND_START_TIME = 0.3;

constexpr int STR_SIZE_X = 200;
constexpr int STR_SIZE_Y = 180;

constexpr int STR_GO_SIZE_X = 360;
constexpr int STR_GO_SIZE_Y = 180;

constexpr double STR_MOVE_TIME = 0.4;

constexpr int OLDPEPAR_OFFSET_X = 50;
constexpr int OLDPEPAR_OFFSET_Y = 50;

CountDownScene::CountDownScene(UniqueScene gamescene)
{
	nextScene_ = std::move(gamescene);
	cntString_ = const_cast<char*>(THREE);
	totalTimeCnt_ = 0.0;

	Init();
}

CountDownScene::~CountDownScene()
{
	if (initThread_.joinable())
	{
		initThread_.join();
	}
	delete downObj_;
	
	lpImageMng.DeleteID("Count1");
	lpImageMng.DeleteID("Count2");
	lpImageMng.DeleteID("Count3");
	lpImageMng.DeleteID("CountGo");
	lpImageMng.DeleteID("Drum0");
	lpImageMng.DeleteID("Drum1");
}

bool CountDownScene::Init(void)
{
	//音楽を作成してた場合止める
	lpAudioMng.StopMusicAll();

	downObj_ = new MoveObjectBoolVoid(
		Vector2{ (lpSceneMng.GetViewArea().x_ - STR_SIZE_X) / 2, -STR_SIZE_Y },
		Vector2{ (lpSceneMng.GetViewArea().x_ - STR_SIZE_X) / 2, (lpSceneMng.GetViewArea().y_ - STR_SIZE_Y) / 2 },
		MoveType::OUTEXP,
		STR_INTERVAL,
		"Count3",
		"Drum0",
		STR_SOUND_START_TIME,
		[]{ return true; });
	
	//非同期で読み込み
	SetUseASyncLoadFlag(true);
	lpAudioMng.GetID(nextScene_->GetMusicName().c_str(), nextScene_->GetMusicName().c_str());
	SetUseASyncLoadFlag(false);

	return true;
}

UniqueScene CountDownScene::UpDate(UniqueScene ownScene)
{
	//カーソルの更新
	lpSceneMng.cursol_->Update();

	downObj_->Update()();

	totalTimeCnt_ += lpSceneMng.GetDeltaTime();

	if (totalTimeCnt_ > STR_INTERVAL)
	{
		totalTimeCnt_ = 0.0;
		if (cntString_ == GO)
		{
			//音楽の再生
			nextScene_->InitMusic();
			return std::move(nextScene_);
		}
		if (cntString_ == ONE)
		{
			cntString_ = const_cast<char*>(GO);
			
			downObj_ = nullptr;
			downObj_ = new MoveObjectBoolVoid(
				Vector2{ (lpSceneMng.GetViewArea().x_ - STR_GO_SIZE_X) / 2, -STR_SIZE_Y },
				Vector2{ (lpSceneMng.GetViewArea().x_ - STR_GO_SIZE_X) / 2, (lpSceneMng.GetViewArea().y_ - STR_GO_SIZE_Y) / 2 },
				MoveType::OUTEXP,
				STR_INTERVAL,
				"CountGo",
				"Drum1",
				STR_SOUND_START_TIME,
				[&] { return true; });
		}
		if (cntString_ == TWO)
		{
			cntString_ = const_cast<char*>(ONE);
		
			downObj_ = nullptr;
			downObj_ = new MoveObjectBoolVoid(
				Vector2{ (lpSceneMng.GetViewArea().x_ - STR_SIZE_X) / 2, -STR_SIZE_Y },
				Vector2{ (lpSceneMng.GetViewArea().x_ - STR_SIZE_X) / 2, (lpSceneMng.GetViewArea().y_ - STR_SIZE_Y) / 2 },
				MoveType::OUTEXP,
				STR_INTERVAL,
				"Count1",
				"Drum0",
				STR_SOUND_START_TIME,
				[&] { return true; });
		}
		if (cntString_ == THREE)
		{
			cntString_ = const_cast<char*>(TWO);

			downObj_ = nullptr;
			downObj_ = new MoveObjectBoolVoid(
				Vector2{ (lpSceneMng.GetViewArea().x_ - STR_SIZE_X) / 2, -STR_SIZE_Y },
				Vector2{ (lpSceneMng.GetViewArea().x_ - STR_SIZE_X) / 2, (lpSceneMng.GetViewArea().y_ - STR_SIZE_Y) / 2 },
				MoveType::OUTEXP,
				STR_INTERVAL,
				"Count2",
				"Drum0",
				STR_SOUND_START_TIME,
				[&] {return true; });
		}
	}
	return ownScene;
}

void CountDownScene::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	//ゲームシーンの描画
	nextScene_->Draw();

	downObj_->Draw();
}
