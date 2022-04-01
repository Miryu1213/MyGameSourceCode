#include <DxLib.h>
#include "TitleScene.h"
#include "SelectScene.h"
#include "Transition/FadeInOut.h"
#include "SceneMng.h"
#include "../common/ImageMng.h"
#include "../common/AudioMng.h"
#include "../common/FontMng.h"
#include "UI/Box.h"
#include "UI/Circle.h"

//ロゴ
constexpr int LOGO_POS_X = 138;
constexpr int LOGO_POS_Y = 40;

constexpr int LOGO_SIZE_X = 708;
constexpr int LOGO_SIZE_Y = 384;

//ボタン
constexpr int BUTTON_DEF_POS_Y = 450;

constexpr int BUTTON_OFFSET_Y = 350;

constexpr int BUTTON_SIZE_X = 192;
constexpr int BUTTON_SIZE_Y = 96;

constexpr int BUTTON_SPACE = 6;

constexpr int TITLE_FONT_SIZE = 40;

constexpr int FLAG_SIZE_X = 64;
constexpr int FLAG_SIZE_Y = 64;
constexpr int FLAG_DIVCNT_X = 4;
constexpr int FLAG_DIVCNT_Y = 1;

constexpr double FIRST_IMAGE_TIME = 2.2;
constexpr double FADE_TIME = 1.6;
constexpr double BURN_TIME = 1.2;
constexpr double UI_FADE_TIME = 1.4;

constexpr double BURN_ONE_FRAME_TIME = 0.1;
constexpr int BURN_SUM_FRAME = 41;
constexpr int BURN_OFFSET_Y = 150;

constexpr int CREDIT_MAX_SIZE = 298;
constexpr double CREDIT_SUM_TIME = 0.8;
//constexpr double CREDIT_SUM_CLOSE_TIME = 0.7;

constexpr int CANCEL_SIZE_X = 66;
constexpr int CANCEL_SIZE_Y = 66;

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
	lpImageMng.DeleteID("logo");
	
	lpAudioMng.DeleteID("title");
}

bool TitleScene::Init(void)
{
	//メンバ初期化
	loadEndFlag_ = false;
	time_ = 0.0;
	firstImageEndFlag_ = false;
	fadeEndFlag_ = false;
	burnEndFlag_ = false;
	uiFadeEndFlag_ = false;
	burnAnimTime_ = 0.0;
	musicFlag_ = false;

	creditFlag_ = false;
	creditOffFlag_ = false;
	creditTime_ = 0.0;
	
	buttonDefPos_ = { (lpSceneMng.GetViewArea().x_ - BUTTON_SIZE_X) / 2, BUTTON_DEF_POS_Y };
	buttonPos_ = buttonDefPos_;

	logoDefPos_ = { (lpSceneMng.GetViewArea().x_ - LOGO_SIZE_X) / 2, 0 };
	logoPos_ = logoDefPos_;
	
	//画像の読み込み
	lpImageMng.GetID("Resource/image/Background/Title0.jpg", "map1");
	lpImageMng.GetID("Resource/image/Background/Title1.jpg", "Title1");
	lpImageMng.GetID("Resource/image/UI/logo.png", "logo");
	lpImageMng.GetID("Resource/image/UI/button_back.png", "ButtonBack");
	lpImageMng.GetID("Resource/image/UI/Title/Start.png", "StartUI");
	lpImageMng.GetID("Resource/image/UI/Title/Credit.png", "CreditUI");
	lpImageMng.GetID("Resource/image/UI/Title/End.png", "EndUI");

	SetUseASyncLoadFlag(true);
	for (int i = 1; i <= BURN_SUM_FRAME; i++)
	{
		lpImageMng.SetID("Resource/image/effect/Fire/" + std::to_string(i) + ".png", "Fire");
	}
	lpImageMng.GetID("Resource/image/UI/textbox.png", "textbox");
	lpImageMng.GetID("Resource/image/UI/Credit.png", "Credit");
	SetUseASyncLoadFlag(false);

	closingCredit_ = std::make_unique<Circle>(
		Vector2(lpSceneMng.GetViewArea().x_ / 2 - CREDIT_MAX_SIZE, lpSceneMng.GetViewArea().y_ / 2 - CREDIT_MAX_SIZE),
		Vector2(CANCEL_SIZE_X, CANCEL_SIZE_Y),
		[&] {
			creditOffFlag_ = true;
			creditTime_ = 0.0;
			return false; },
		MouseInputID::LEFT);
	closingCredit_->SetModelID(lpImageMng.GetID("ButtonBack")[0]);
	
	btnList_.push_back(std::make_unique<Box>(
		Vector2((lpSceneMng.GetViewArea().x_ - BUTTON_SIZE_X) / 2, lpSceneMng.GetViewArea().y_ - BUTTON_OFFSET_Y),
		Vector2(BUTTON_SIZE_X, BUTTON_SIZE_Y),
		[] { return true; },
		MouseInputID::LEFT));
	btnList_.back()->SetModelID(lpImageMng.GetID("StartUI")[0]);

	btnList_.push_back(std::make_unique<Box>(
		Vector2((lpSceneMng.GetViewArea().x_ - BUTTON_SIZE_X) / 2, lpSceneMng.GetViewArea().y_ - BUTTON_OFFSET_Y + (BUTTON_SIZE_Y + BUTTON_SPACE)),
		Vector2(BUTTON_SIZE_X, BUTTON_SIZE_Y), 
		[&] { 
			creditFlag_ = true; 
			return false; },
		MouseInputID::LEFT));
	btnList_.back()->SetModelID(lpImageMng.GetID("CreditUI")[0]);

	btnList_.push_back(std::make_unique<Box>(
		Vector2((lpSceneMng.GetViewArea().x_ - BUTTON_SIZE_X) / 2, lpSceneMng.GetViewArea().y_ - BUTTON_OFFSET_Y + (BUTTON_SIZE_Y + BUTTON_SPACE) * 2),
		Vector2(BUTTON_SIZE_X, BUTTON_SIZE_Y),
		[] {
			lpSceneMng.FinishGame();
			return false; },
		MouseInputID::LEFT));
	btnList_.back()->SetModelID(lpImageMng.GetID("EndUI")[0]);

	//ユニット
	if (!lpImageMng.IsDotKey("Archer"))
	{
		lpImageMng.SetColorAndID(0x005d00, TOCOLOR::BLUE, "Resource/image/Unit/archer.png", "Archer", { 64,64 }, { 6,5 });
		lpImageMng.SetColorAndID(0x005d00, TOCOLOR::RED, "Resource/image/Unit/archer.png", "Archerenemy", { 64,64 }, { 6,5 });
		lpImageMng.SetAllColorAndID(TOCOLOR::RED, "Resource/image/Unit/archer.png", "Archerenemydamage", { 64,64 }, { 6,5 });
		lpImageMng.SetAllColorAndID(TOCOLOR::BLUE, "Resource/image/Unit/archer.png", "Archerallydamage", { 64,64 }, { 6,5 });
		lpImageMng.GetID("Resource/image/Unit/arrow.png", "Archerarrow");

		int warriorchangecolor = 0x000088 + 0x0000b4 - 0x000088;
		lpImageMng.SetColorAndID(warriorchangecolor, TOCOLOR::BLUE, "Resource/image/Unit/warrior.png", "Warrior", { 64,64 }, { 6,5 });
		lpImageMng.SetColorAndID(warriorchangecolor, TOCOLOR::RED, "Resource/image/Unit/warrior.png", "Warriorenemy", { 64,64 }, { 6,5 });
		lpImageMng.SetAllColorAndID(TOCOLOR::RED, "Resource/image/Unit/warrior.png", "Warriorenemydamage", { 64,64 }, { 6,5 });
		lpImageMng.SetAllColorAndID(TOCOLOR::BLUE, "Resource/image/Unit/warrior.png", "Warriorallydamage", { 64,64 }, { 6,5 });

		int knightchangecolor = 0x000088 + 0x0000b4 - 0x000088;
		lpImageMng.SetColorAndID(knightchangecolor, TOCOLOR::BLUE, "Resource/image/Unit/knight.png", "Knight", { 64,64 }, { 8,8 });
		lpImageMng.SetColorAndID(knightchangecolor, TOCOLOR::RED, "Resource/image/Unit/knight.png", "Knightenemy", { 64,64 }, { 8,8 });
		lpImageMng.SetAllColorAndID(TOCOLOR::RED, "Resource/image/Unit/knight.png", "Knightenemydamage", { 64,64 }, { 8,8 });
		lpImageMng.SetAllColorAndID(TOCOLOR::BLUE, "Resource/image/Unit/knight.png", "Knightallydamage", { 64,64 }, { 8,8 });

		lpImageMng.GetDotID("Resource/image/Plant/Flag.png", "AllyFlag", { FLAG_SIZE_X,FLAG_SIZE_Y }, { FLAG_DIVCNT_X,FLAG_DIVCNT_Y });
		lpImageMng.SetColorAndID(0x0000ff, TOCOLOR::RED, "Resource/image/Plant/Flag.png", "EnemyFlag", { FLAG_SIZE_X,FLAG_SIZE_Y }, { FLAG_DIVCNT_X,FLAG_DIVCNT_Y });
	}

	lpAudioMng.GetID("Resource/Audio/BGM/title.mp3", "title");

	//音楽を作成してた場合止める
	lpAudioMng.StopMusicAll();

	SetWindowText("Defele");

	return true;
}

UniqueScene TitleScene::UpDate(UniqueScene ownScene)
{
	lpSceneMng.cursol_->Update();

	if (GetASyncLoadNum() == 0)
	{
		//ロード終了
		loadEndFlag_ = true;
	}

	if (!uiFadeEndFlag_)
	{
		//クリックされた際演出スキップ
		if (lpSceneMng.cursol_->GetMouseInput(InputType::NOW)[MouseInputID::LEFT])
		{
			uiFadeEndFlag_ = true;
		}
		return ownScene;
	}

	//クレジット
	UpdateCredit();

	//通常のアップデート
	if (UpdateDefault())
	{
		return std::make_unique<FadeInOut>(std::move(ownScene), std::make_unique<SelectScene>(), 1.0);
	}

	return ownScene;
}

void TitleScene::Draw(void)
{
	DrawFirstImage();
	DrawFadeImage();
	DrawBurn();
	DrawFadeUI();
	DrawDefault();

	DrawCredit();

	//カーソルの描画
	lpSceneMng.cursol_->Draw(false);
}

bool TitleScene::UpdateDefault(void)
{
	if (creditFlag_)
	{
		return false;
	}

	if (!musicFlag_)
	{
		//音楽再生
		PlaySoundMem(lpAudioMng.GetID("title"), DX_PLAYTYPE_LOOP);
		SetVolumeMusic(180);
		musicFlag_ = true;
	}

	//マウスの値の取得
	auto mouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::NOW);
	auto oldmouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::OLD);

	for (auto& button : btnList_)
	{
		if (button->CheckColition(lpSceneMng.cursol_->GetPos()))
		{
			if ((button->CheckButtonInput(mouseInput[button->GetCheckId()], oldmouseInput[button->GetCheckId()])))
			{
				if (button->GetFunction()())
				{
					return true;
				}
			}
		}
	}
	return false;
}

void TitleScene::UpdateCredit(void)
{
	if (!creditFlag_)
	{
		return;
	}

	if (!creditOffFlag_)
	{
		//拡大
		creditSize_.x_ = static_cast<int>(Easing::OutQuart(creditTime_, CREDIT_SUM_TIME) * CREDIT_MAX_SIZE);
		creditSize_.y_ = static_cast<int>(Easing::OutQuart(creditTime_, CREDIT_SUM_TIME) * CREDIT_MAX_SIZE);

		creditTime_ = min(creditTime_ + lpSceneMng.GetDeltaTime(), CREDIT_SUM_TIME);

		//マウスの値の取得
		auto mouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::NOW);
		auto oldmouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::OLD);

		if (closingCredit_->CheckColition(lpSceneMng.cursol_->GetPos()))
		{
			if ((closingCredit_->CheckButtonInput(mouseInput[closingCredit_->GetCheckId()], oldmouseInput[closingCredit_->GetCheckId()])))
			{
				closingCredit_->GetFunction()();
			}
		}
	}
	else
	{
		//縮小
		/*creditSize_.x_ = CREDIT_MAX_SIZE - static_cast<int>(Easing::InCubic(creditTime_, CREDIT_SUM_CLOSE_TIME) * CREDIT_MAX_SIZE);
		creditSize_.y_ = CREDIT_MAX_SIZE - static_cast<int>(Easing::InCubic(creditTime_, CREDIT_SUM_CLOSE_TIME) * CREDIT_MAX_SIZE);

		creditTime_ = min(creditTime_ + lpSceneMng.GetDeltaTime(), CREDIT_SUM_CLOSE_TIME);

		if (creditTime_ == CREDIT_SUM_CLOSE_TIME)
		{
			creditFlag_ = false;
			creditOffFlag_ = false;
			creditTime_ = 0.0;
		}*/
		creditFlag_ = false;
		creditOffFlag_ = false;
		creditTime_ = 0.0;
		creditSize_ = Vector2();
	}
}

void TitleScene::DrawFirstImage(void)
{
	if (firstImageEndFlag_ || !loadEndFlag_)
	{
		return;
	}

	int alpha = min((time_ / (FIRST_IMAGE_TIME / 2)) * 255, 255);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(0, 0, lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_, lpImageMng.GetID("map1")[0], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	time_ += lpSceneMng.GetDeltaTime();
	if (FIRST_IMAGE_TIME < time_)
	{
		firstImageEndFlag_ = true;
		time_ = 0.0;
	}
}

void TitleScene::DrawFadeImage(void)
{
	if (!firstImageEndFlag_ || fadeEndFlag_)
	{
		return;
	}
	int alpha = static_cast<int>(min((time_ / (FADE_TIME * 2 / 3)) * 255, 255));

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	DrawExtendGraph(0, 0, lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_, lpImageMng.GetID("map1")[0], true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(0, 0, lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_, lpImageMng.GetID("Title1")[0], true);

	time_ += lpSceneMng.GetDeltaTime();
	if (FADE_TIME < time_)
	{
		fadeEndFlag_= true;
		time_ = 0.0;
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void TitleScene::DrawBurn(void)
{
	if (!fadeEndFlag_ || burnEndFlag_)
	{
		return;
	}

	DrawExtendGraph(0, 0, lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_, lpImageMng.GetID("Title1")[0], true);
	
	int alpha = static_cast<int>(min((time_ / (BURN_TIME * 2 / 3)) * 220, 220));
	
	SetDrawBlendMode(DX_BLENDMODE_ADD, alpha);
	int id = static_cast<int>(fmod((burnAnimTime_ / BURN_ONE_FRAME_TIME), BURN_SUM_FRAME));
	DrawExtendGraph(0, lpSceneMng.GetViewArea().y_ / 2 - BURN_OFFSET_Y,
		lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_,
		lpImageMng.GetID("Fire")[id], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	burnAnimTime_ += lpSceneMng.GetDeltaTime();
	
	time_ += lpSceneMng.GetDeltaTime();
	if (BURN_TIME < time_)
	{
		burnEndFlag_ = true;
		time_ = 0.0;
	}
}

void TitleScene::DrawFadeUI(void)
{
	if (!burnEndFlag_ || uiFadeEndFlag_ )
	{
		return;
	}

	//背景描画
	DrawModiGraph(0, 0,
		lpSceneMng.GetViewArea().x_, 0,
		lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_,
		0, lpSceneMng.GetViewArea().y_, lpImageMng.GetID("Title1")[0], true);

	SetDrawBlendMode(DX_BLENDMODE_ADD, 220);
	//火エフェクト
	int id = static_cast<int>(fmod((burnAnimTime_ / BURN_ONE_FRAME_TIME), BURN_SUM_FRAME));
	DrawExtendGraph(0, lpSceneMng.GetViewArea().y_ / 2 - BURN_OFFSET_Y,
		lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_,
		lpImageMng.GetID("Fire")[id], true);
	burnAnimTime_ += lpSceneMng.GetDeltaTime();
	
	//ボタン
	int alpha = min((time_ / UI_FADE_TIME) * 255, 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	DrawExtendGraph(buttonPos_.x_, buttonPos_.y_,
		buttonPos_.x_ + BUTTON_SIZE_X, buttonPos_.y_ + BUTTON_SIZE_Y,
		lpImageMng.GetID("StartUI")[0], true);

	DrawExtendGraph(buttonPos_.x_, buttonPos_.y_ + BUTTON_SIZE_Y + BUTTON_SPACE,
		buttonPos_.x_ + BUTTON_SIZE_X, buttonPos_.y_ + BUTTON_SIZE_Y + (BUTTON_SIZE_Y + BUTTON_SPACE) * 1,
		lpImageMng.GetID("CreditUI")[0], true);

	DrawExtendGraph(buttonPos_.x_, buttonPos_.y_ + BUTTON_SIZE_Y + (BUTTON_SIZE_Y + BUTTON_SPACE) * 1 + BUTTON_SPACE,
		buttonPos_.x_ + BUTTON_SIZE_X, buttonPos_.y_ + BUTTON_SIZE_Y + (BUTTON_SIZE_Y + BUTTON_SPACE) * 2,
		lpImageMng.GetID("EndUI")[0], true);

	int distance = buttonDefPos_.y_ - (lpSceneMng.GetViewArea().y_ - BUTTON_OFFSET_Y);
	buttonPos_.y_ = buttonDefPos_.y_ - min((time_ / (UI_FADE_TIME / 2)) * distance, distance);

	//ロゴ
	alpha = static_cast<int>(min((time_ / (UI_FADE_TIME * 2 / 3)) * 255, 255));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawGraph(logoPos_.x_, logoPos_.y_, lpImageMng.GetID("logo")[0], true);
	int logoDistance = LOGO_POS_Y - logoDefPos_.y_;
	logoPos_.y_ = logoDefPos_.y_ + static_cast<int>(min((time_ / (UI_FADE_TIME / 2)) * logoDistance, logoDistance));

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	time_ += lpSceneMng.GetDeltaTime();
	if (UI_FADE_TIME < time_)
	{
		uiFadeEndFlag_ = true;
		time_ = 0.0;
	}
}

void TitleScene::DrawDefault(void)
{
	if (!uiFadeEndFlag_)
	{
		return;
	}

	//背景描画
	DrawModiGraph(0, 0,
		lpSceneMng.GetViewArea().x_, 0,
		lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_,
		0, lpSceneMng.GetViewArea().y_, lpImageMng.GetID("Title1")[0], true);

	DrawGraph((lpSceneMng.GetViewArea().x_ - LOGO_SIZE_X) / 2, LOGO_POS_Y, lpImageMng.GetID("logo")[0], true);

	SetDrawBlendMode(DX_BLENDMODE_ADD, 220);
	int id = static_cast<int>(fmod((burnAnimTime_ / BURN_ONE_FRAME_TIME), BURN_SUM_FRAME));
	DrawExtendGraph(0, lpSceneMng.GetViewArea().y_ / 2 - BURN_OFFSET_Y,
		lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_,
		lpImageMng.GetID("Fire")[id], true);
	burnAnimTime_ += lpSceneMng.GetDeltaTime();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	for (auto& button : btnList_)
	{
		bool Flag = false;
		if (!creditFlag_)
		{
			if (button->CheckColition(lpSceneMng.cursol_->GetPos()))
			{
				Flag = true;
			}
		}
		button->Draw(Flag, TITLE_FONT_SIZE);
	}
}

void TitleScene::DrawCredit(void)
{
	if (!creditFlag_)
	{
		return;
	}

	DrawExtendGraph(lpSceneMng.GetViewArea().x_ / 2 - creditSize_.x_, lpSceneMng.GetViewArea().y_ / 2 - creditSize_.y_,
		lpSceneMng.GetViewArea().x_ / 2 + creditSize_.x_, lpSceneMng.GetViewArea().y_ / 2 + creditSize_.x_,
		lpImageMng.GetID("Credit")[0], true);

	if (creditTime_ == CREDIT_SUM_TIME)
	{
		bool Flag = false;
		if (closingCredit_->CheckColition(lpSceneMng.cursol_->GetPos()))
		{
			Flag = true;
		}
		closingCredit_->Draw(Flag);
	}
}
