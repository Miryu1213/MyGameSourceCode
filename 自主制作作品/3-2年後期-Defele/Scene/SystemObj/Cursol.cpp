#include "Cursol.h"
#include "../../Input/KeyBoard.h"
#include "../../Input/JoyPad.h"
#include "../../common/ImageMng.h"
#include "../SceneMng.h"
#include "../SystemObj/InputString.h"
#include "../../common/Easing.h"

//�s���������e�͈�
constexpr int Tolerance = 20;

//���ʍő�\������
constexpr double WaterSurfaceTime = 0.6;

Cursol::Cursol()
{
}

Cursol::~Cursol()
{
}

void Cursol::Init(void)
{
	//���͏��̎擾
	if (GetJoypadNum())
	{
		input_ = std::make_unique<JoyPad>();
	}
	else
	{
		input_ = std::make_unique<KeyBoard>();
	}

	input_->Init();

	//������
	speed_ = 8;

	//�}�E�X�J�[�\���̍폜
#ifndef _DEBUG
	SetMouseDispFlag(false);
#endif

	//�J�[�\���̃T�C�Y�̎擾
	GetGraphSize(lpImageMng.GetID("Resource/image/UI/cursol.png", "Cursol")[0], &size_.x_, &size_.y_);

	//��ʃT�C�Y�̎擾
	int tmpcolor = 0;
	GetScreenState(&Screensize_.x_, &Screensize_.y_, &tmpcolor);
}

void Cursol::Update(void)
{
	input_->Update();
	//�L�[���̎擾
	auto inputdata = input_->GetInputData(InputType::NOW);
	auto oldinputdata = input_->GetInputData(InputType::OLD);

	//�L�[�{�[�h����`�F�b�N
	for (auto& data : inputdata)
	{
		if (data.second != 0)
		{
			keyChecker = true;
			break;
		}
	}

	//�}�E�X���̎擾
	auto m_data = GetMouseInput(InputType::NOW);
	auto m_dataold = GetMouseInput(InputType::OLD);

	Vector2 mousepos = { m_data[MouseInputID::POSX] ,m_data[MouseInputID::POSY] };
	Vector2 oldmousepos = { m_dataold[MouseInputID::POSX] ,m_dataold[MouseInputID::POSY] };

	//�}�E�X�̍��W���O��ƈႦ�΃}�E�X���͂ɂ���
	if (mousepos != oldmousepos)
	{
		keyChecker = false;
		pos_ = mousepos;
	}

	int add =  speed_ + Tolerance;

	//���W�̍X�V
	if (0 <= pos_.y_ - speed_)
	{
		if (inputdata[InputID::UP])
		{
			pos_.y_ -= speed_;
		}
	}
	if (pos_.y_ + add <= Screensize_.y_)
	{
		if (inputdata[InputID::DOWN])
		{
			pos_.y_ += speed_;
		}
	}
	if (pos_.x_ + add <= Screensize_.x_)
	{
		if (inputdata[InputID::RIGHT])
		{
			pos_.x_ += speed_;
		}
	}
	if (0 <= pos_.x_ - speed_)
	{
		if (inputdata[InputID::LEFT])
		{
			pos_.x_ -= speed_;
		}
	}

	//�L�[�{�[�h�������Ȃ��ꍇ�A�}�E�X���W�̍X�V
	if (!keyChecker)
	{
		input_->SetMousePos(pos_);
	}

	//���N���b�N���ꂽ�Ƃ��̐���
	if (m_data[MouseInputID::LEFT] && !m_dataold[MouseInputID::LEFT])
	{
		waterTimeVec_.push_back(WaterSurfaceTime);
		waterPosVec_.push_back(pos_);
	}

	for (auto& waterTime : waterTimeVec_)
	{
		if (0 < waterTime)
		{
			//�b���J�E���g
			waterTime -= lpSceneMng.GetDeltaTime();
		}
	}

	for (auto waterTimeItr = waterTimeVec_.begin(); waterTimeItr != waterTimeVec_.end();)
	{
		if ((*waterTimeItr) <= 0.0)
		{
			waterTimeItr = waterTimeVec_.erase(waterTimeItr);

			//���W�̍폜
			waterPosVec_.erase(waterPosVec_.begin());
			continue;
		}
		waterTimeItr++;
	}
}

void Cursol::Draw(bool isTutorial)
{
	DrawGraph(pos_.x_, pos_.y_, lpImageMng.GetID("Cursol")[0], true);

	//���ʂ̕`��
	for (int i = 0; i < static_cast<int>(waterTimeVec_.size()); i++)
	{
		if (0 < waterTimeVec_[i])
		{
			//�{��
			int doublepoint = 18;

			auto amountofMove = Easing::OutCirc(waterTimeVec_[i], WaterSurfaceTime);

			//���l����	
			int alpha = 0;

			if (waterTimeVec_[i] < WaterSurfaceTime / 4)
			{
				//�c�莞�Ԃ�1/4�̂Ƃ��t�F�[�h�A�E�g
				alpha = static_cast<int>((WaterSurfaceTime - waterTimeVec_[i]) * 400);
			}
			else
			{
				alpha = static_cast<int>((WaterSurfaceTime - waterTimeVec_[i]) * 120);
			}

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);

			DrawCircle(waterPosVec_[i].x_, waterPosVec_[i].y_, static_cast<int>(amountofMove * doublepoint), 0xffffff, false);

			//�����܂ŕ`��
			if (WaterSurfaceTime / 2 < waterTimeVec_[i])
			{
				DrawCircle(waterPosVec_[i].x_, waterPosVec_[i].y_, static_cast<int>(amountofMove * doublepoint) + 1, 0xffffff, false);
			}
		}
	}

	//���l
	int alpha = 255;
	if (isTutorial)
	{
		//�`���[�g���A�����̃��l��ς���
		alpha = lpSceneMng.alpha_;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
}

MouseInputData Cursol::GetMouseInput(InputType inputtype)
{
	auto mouse = input_->GetMouseData(inputtype);

	//�p�b�h�̏ꍇ�̓p�b�h�̓��͂��g�p����
	if (input_->GetInputClass() == InputClass::PAD)
	{
		auto pad = input_->GetInputData(inputtype);
		mouse[MouseInputID::LEFT] |= pad[InputID::A];
		mouse[MouseInputID::RIGHT] |= pad[InputID::B];
		mouse[MouseInputID::MIDDLE] += pad[InputID::RB];
		mouse[MouseInputID::MIDDLE] -= pad[InputID::LB];

		//�{�^������
		mouse[MouseInputID::BUTTON_X] |= pad[InputID::X];
		mouse[MouseInputID::BUTTON_Y] |= pad[InputID::Y];
	}

	return mouse;
}
