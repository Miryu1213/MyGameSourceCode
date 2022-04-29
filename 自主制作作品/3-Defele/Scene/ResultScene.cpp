#include <memory>
#include "ResultScene.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "Transition/FadeInOut.h"
#include "SceneMng.h"
#include "../common/ImageMng.h"
#include "../common/AudioMng.h"
#include "../common/FontMng.h"
#include "UI/Box.h"

//�{�b�N�X�T�C�Y
constexpr int BUTTON_SIZE_X = 192;
constexpr int BUTTON_SIZE_Y = 96;

constexpr int BUTTON_OFFSET_Y = 312;

constexpr int BUTTON_INTERVAL = 4;

constexpr int RESULT_FONT_SIZE = 70;

constexpr double TEXT_TOTAL_TIME = 2.0;
constexpr int TEXT_OFFSET_Y = 60;
constexpr int BUTTON_MOVE_OFFSET_Y = 120;

ResultScene::ResultScene(ArmyType breakarmycore)
{
	destroyedCore_ = breakarmycore;

	lpAudioMng.GetID("Resource/Audio/BGM/lose.mp3", "lose");
	lpAudioMng.GetID("Resource/Audio/BGM/win.mp3", "win");

	//���y���쐬���Ă��ꍇ�~�߂�
	lpAudioMng.StopMusicAll();

	//���y�Đ�
	if (destroyedCore_ == ArmyType::ALLY)
	{
		PlaySoundMem(lpAudioMng.GetID("lose"), DX_PLAYTYPE_BACK);
	}
	else
	{
		PlaySoundMem(lpAudioMng.GetID("win"), DX_PLAYTYPE_BACK);
	}

	SetVolumeMusic(200);

	Init();
}

ResultScene::~ResultScene()
{
	lpImageMng.DeleteID("TitleFromRes");
	lpImageMng.DeleteID("SelFromRes");

	lpAudioMng.DeleteID("lose");
	lpAudioMng.DeleteID("win");
}

bool ResultScene::Init(void)
{
	//�����o������
	textScrEndFlag_ = false;
	textTime_ = 0.0;
	textDefPos_ = { 0,-TEXT_OFFSET_Y };
	textPos_ = textDefPos_;

	lpImageMng.GetID("Resource/image/Background/0.jpg", "0");
	lpImageMng.GetID("Resource/image/Background/ResultWin.jpg", "ResultWin");

	lpImageMng.GetID("Resource/image/UI/Result/TitleFromRes.png", "TitleFromRes");
	lpImageMng.GetID("Resource/image/UI/Result/SelFromRes.png", "SelFromRes");

	lpImageMng.GetID("Resource/image/UI/Result/WinText.png", "WinText");
	lpImageMng.GetID("Resource/image/UI/Result/LoseText.png", "LoseText");
	
	btnList_.push_back(std::make_unique<Box>(Vector2((lpSceneMng.GetViewArea().x_ - BUTTON_SIZE_X) / 2, lpSceneMng.GetViewArea().y_ - BUTTON_OFFSET_Y),
		Vector2(BUTTON_SIZE_X, BUTTON_SIZE_Y),
		std::bind(&ResultScene::ToTitle,this),
		MouseInputID::LEFT));
	btnList_.back()->SetModelID(lpImageMng.GetID("TitleFromRes")[0]);

	btnList_.push_back(std::make_unique<Box>(Vector2((lpSceneMng.GetViewArea().x_ - BUTTON_SIZE_X) / 2, lpSceneMng.GetViewArea().y_ - BUTTON_OFFSET_Y + BUTTON_SIZE_Y + BUTTON_INTERVAL),
		Vector2(BUTTON_SIZE_X, BUTTON_SIZE_Y),
		std::bind(&ResultScene::ToSelect, this),
		MouseInputID::LEFT));
	btnList_.back()->SetModelID(lpImageMng.GetID("SelFromRes")[0]);

	btnPos_ = btnList_.front()->GetPos() + Vector2(0, BUTTON_MOVE_OFFSET_Y);
	btnDefPos_ = btnPos_;

	return true;
}

UniqueScene ResultScene::UpDate(UniqueScene ownScene)
{
	lpSceneMng.cursol_->Update();

	if (!textScrEndFlag_)
	{
		return ownScene;
	}

	//�}�E�X�̒l�̎擾
	auto mouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::NOW);
	auto oldmouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::OLD);

	for (auto& box : btnList_)
	{
		if (box->CheckColition(lpSceneMng.cursol_->GetPos()))
		{
			//�N���b�N���ꂽ�Ƃ�
			if ((box->CheckButtonInput(mouseInput[box->GetCheckId()], oldmouseInput[box->GetCheckId()])))
			{
				if (box->GetFunction()())
				{
					return std::make_unique<FadeInOut>(std::move(ownScene), std::make_unique<TitleScene>(), 1.0);
				}
				else
				{
					return std::make_unique<FadeInOut>(std::move(ownScene), std::make_unique<SelectScene>(), 1.0);
				}
			}
		}
	}

	return ownScene;
}

void ResultScene::Draw(void)
{
	const char* textKeyName = "";
	const char* bgKeyName = "";

	if (destroyedCore_ == ArmyType::ALLY)
	{
		textKeyName = "LoseText";
		bgKeyName = "0";
	}
	else
	{
		textKeyName = "WinText";
		bgKeyName = "ResultWin";
	}

	//�w�i�`��
	DrawModiGraph(0, 0,
		lpSceneMng.GetViewArea().x_, 0,
		lpSceneMng.GetViewArea().x_, lpSceneMng.GetViewArea().y_,
		0, lpSceneMng.GetViewArea().y_, lpImageMng.GetID(bgKeyName)[0], true);

	//�����̕`��
	int alpha = min(static_cast<int>((textTime_ / TEXT_TOTAL_TIME) * 255), 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawGraph(textPos_.x_, textPos_.y_, lpImageMng.GetID(textKeyName)[0], true);

	int textDistance = -textDefPos_.y_;
	//textPos_.y_ = textDefPos_.y_ + min((textTime_ / (TEXT_TOTAL_TIME)) * textDistance, textDistance);
	textPos_.y_ = textDefPos_.y_ + static_cast<int>(Easing::OutCubic(textTime_, TEXT_TOTAL_TIME) * textDistance);

	//�{�^��
	if (!textScrEndFlag_)
	{
		DrawGraph(btnPos_.x_, btnPos_.y_, lpImageMng.GetID("TitleFromRes")[0], true);
		DrawGraph(btnPos_.x_, btnPos_.y_ + BUTTON_SIZE_Y + BUTTON_INTERVAL, lpImageMng.GetID("SelFromRes")[0], true);
	}
	else
	{
		for (const auto& btn : btnList_)
		{
			bool Flag = false;
			if (btn->CheckColition(lpSceneMng.cursol_->GetPos()))
			{
				Flag = true;
			}
			btn->Draw(Flag, 32);
		}
	}

	int btnDistance = btnDefPos_.y_ + BUTTON_MOVE_OFFSET_Y - btnDefPos_.y_;
	//btnPos_.y_ = btnDefPos_.y_ - min((textTime_ / (TEXT_TOTAL_TIME)) * btnDistance, btnDistance);
	btnPos_.y_ = btnDefPos_.y_ - static_cast<int>(Easing::OutCubic(textTime_, TEXT_TOTAL_TIME) * btnDistance);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	textTime_ = min(textTime_ + lpSceneMng.GetDeltaTime(), TEXT_TOTAL_TIME);
	if (TEXT_TOTAL_TIME <= textTime_)
	{
		textScrEndFlag_ = true;
	}

	//�J�[�\���̕`��
	lpSceneMng.cursol_->Draw(false);
}

bool ResultScene::ToTitle()
{
	return true;
}

bool ResultScene::ToSelect()
{
	return false;
}
