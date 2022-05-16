#include <sstream>
#include <string>
#include "TutorialScene.h"
#include "CountDownScene.h"
#include "SceneMng.h"
#include "SystemObj/InputString.h"
#include "Deploy/DeployMng.h"

#include "../_debug/_DebugConOut.h"

constexpr int TUTORIAL_BGM_VOLUME = 120;

TutorialScene::TutorialScene(const char* mapname, UniqueScene gameScene)
{
	nextScene_ = std::move(gameScene);

	camera_->InitArea();

	//ゲームシーンのカメラをチュートリアルのカメラにする
	nextScene_->SetCamera(camera_);

	Init();
}

TutorialScene::~TutorialScene()
{
}

bool TutorialScene::Init(void)
{
	//メッセージの記載

	//チュートリアル表示順
	orderList_.push_back(PLACE::FIRST);
	orderList_.push_back(PLACE::PLANT);
	orderList_.push_back(PLACE::UNIT);
	orderList_.push_back(PLACE::CORE);
	orderList_.push_back(PLACE::BUTTON);
	orderList_.push_back(PLACE::WARSITUATION);
	orderList_.push_back(PLACE::DEPLOY);
	orderList_.push_back(PLACE::ENEMYPLANT);
	orderList_.push_back(PLACE::MAX);

	for (auto place : PLACE())
	{
		//プレイスを格納
		stringMap_.try_emplace(place);
	}

	//それぞれのプレイスに文字列を格納
	stringMap_[PLACE::FIRST] = "Welcome To My Game !,ここではチュートリアルを行います";
	stringMap_[PLACE::PLANT] = "施設はユニットの生成を行います";
	stringMap_[PLACE::UNIT] = "ユニットは施設に与えられた|ルート上を動きます,敵や相手の施設が近くにいると攻撃します";
	stringMap_[PLACE::CORE] = "コアは破壊すれば勝利、|破壊されれば敗北です";
	stringMap_[PLACE::BUTTON] = "上部ボタンで施設のルート変更が可能です,x1で倍速の切り替えができます";
	stringMap_[PLACE::WARSITUATION] = "上部バーで現在の戦況を確認できます";
	stringMap_[PLACE::DEPLOY] = "下の丸から施設の配置|ギミックの発動が可能です";
	stringMap_[PLACE::ENEMYPLANT] = "今回の相手は東にいるようです";
	stringMap_[PLACE::MAX] = "Have a Good Time !";

	//現在の文字列
	std::string nowString = stringMap_[orderList_.front()];

	//,が見つかると分割する
	int laststr =
		static_cast<int>(nowString.find_first_of(",")) != nowString.npos ?
		static_cast<int>(nowString.find_first_of(",")) : static_cast<int>(nowString.size());

	if (static_cast<int>(nowString.size()) != laststr)
	{
		//分割する場合
		nowString.erase(laststr, static_cast<int>(nowString.size()));

		//現在の文字列まで削除
		stringMap_[orderList_.front()].erase(0, laststr + 1);

		//次のメッセージへ
		lpInputString.CreateString(orderList_.front(), nowString, true);
	}
	else
	{
		//現在の文字列が最後まで進んでいる場合

	    //次のメッセージへ
		lpInputString.CreateString(orderList_.front(), nowString, true);

		//先頭を削除
		orderList_.pop_front();
	}

	return true;
}

void TutorialScene::InitMusic(void)
{
	//音楽を作成してた場合止める
	lpAudioMng.StopMusicAll();
	//音楽再生
	PlaySoundMem(lpAudioMng.GetID(nextScene_->GetMusicName()), DX_PLAYTYPE_LOOP);
	SetVolumeMusicMem(TUTORIAL_BGM_VOLUME, lpAudioMng.GetID(nextScene_->GetMusicName()));
}

UniqueScene TutorialScene::UpDate(UniqueScene ownScene)
{
	//カーソルの更新
	lpSceneMng.cursol_->Update();

	//チュートリアル用のカメラの更新
	camera_->UpDateTutorial();

	//シグナル更新
	for (const auto& signal : nextScene_->GetSigNalList())
	{
		signal->Update(camera_->GetPos(), camera_->GetDrawOffset(), true);
	}

	//マウスの値の取得
	auto mouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::NOW);
	auto oldmouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::OLD);

	if (mouseInput[MouseInputID::LEFT] && !oldmouseInput[MouseInputID::LEFT])
	{
		if (!orderList_.empty())
		{
			//現在の文字列
			auto nowString = stringMap_[orderList_.front()];

			//,が見つかると分割する
			int laststr =
				static_cast<int>(nowString.find_first_of(",")) != nowString.npos ?
				static_cast<int>(nowString.find_first_of(",")) : static_cast<int>(nowString.size());

			if (static_cast<int>(nowString.size()) != laststr)
			{
				//分割する場合
				nowString.erase(laststr, static_cast<int>(nowString.size()));

				//現在の文字列まで削除
				stringMap_[orderList_.front()].erase(0, laststr + 1);

				//次のメッセージへ
				lpInputString.CreateString(orderList_.front(), nowString, true);
			}
			else
			{
				//現在の文字列が最後まで進んでいる場合

				//次のメッセージへ
				lpInputString.CreateString(orderList_.front(), nowString, true);

				//先頭を削除
				orderList_.pop_front();
			}
		}
		else
		{
			//次のメッセージがない場合
			lpInputString.CreateString(PLACE::MAX, "", true);

			//ゲームシーンの初期化処理
			nextScene_->InitFromTutorial();

			//ゲームシーンへ
			return std::make_unique<CountDownScene>(std::move(nextScene_));
		}
	}

	//表示しているものが手持ちのとき
	if (!orderList_.empty())
	{
		if (orderList_.front() == PLACE::ENEMYPLANT)
		{
			lpDeployMng.CircleMoveRight();

			lpDeployMng.SetShowBoxFlag(true);
		}
		if (orderList_.front() == PLACE::MAX)
		{
			lpDeployMng.CircleMoveLeft();

			lpDeployMng.SetShowBoxFlag(false);
		}
	}

	return std::move(ownScene);
}

void TutorialScene::Draw(void)
{
	if (!orderList_.empty())
	{
		//画面を薄く
		lpSceneMng.alpha_ = 128;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, lpSceneMng.alpha_);
	}
	
	nextScene_->Draw();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//文字列格納時
	if (lpInputString.GetString() != "")
	{
		//マウスの値の取得
		auto mouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::NOW);
		auto oldmouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::OLD);

		//マウス左クリックで消滅
		lpInputString.DissapperInputDraw();
	}
}