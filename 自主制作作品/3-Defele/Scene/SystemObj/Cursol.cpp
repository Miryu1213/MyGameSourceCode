#include "Cursol.h"
#include "../../Input/KeyBoard.h"
#include "../../Input/JoyPad.h"
#include "../../common/ImageMng.h"
#include "../SceneMng.h"
#include "../SystemObj/InputString.h"
#include "../../common/Easing.h"

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

	//�g���C���̏�����
	TrailClear();

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

	//�g���C�����X�g�ֈʒu��}��
	Vector2Flt FltPos;
	FltPos.x_ = static_cast<float>(pos_.x_);
	FltPos.y_ = static_cast<float>(pos_.y_);

	trailList_.push_front(FltPos);

	//�g���C���������E�𒴂��Ă���ΌÂ��ق�����j��
	if (trailList_.size() > trailLimit)
	{
		trailList_.pop_back();
	}
}

void Cursol::Draw(bool isTutorial)
{
	//DrawCircle(pos_.x_, pos_.y_, 5, 0xff5555);

	//�g���C���̕`��
	Vector2Flt lastpos;
	//�ЂƂO�̍��W
	lastpos.x_ = static_cast<float>(pos_.x_);
	lastpos.y_ = static_cast<float>(pos_.y_);

	//Vert�J�E���g�p
	int vertCnt = 0;

	//�A���t�@�l�ύX�p
	byte alpha = 255;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	for (auto& pos : trailList_)
	{
		//��
		Vert[vertCnt].pos = { pos.x_ - TrailSizeH ,pos.y_,0 };
		Vert[vertCnt].rhw = 1.0f;
		Vert[vertCnt].dif = GetColorU8(255, 255, 255, alpha);
		Vert[vertCnt].u = 0.0f;
		Vert[vertCnt].v = 0.0f;

		vertCnt++;

		Vert[vertCnt].pos = { lastpos.x_ + TrailSizeH,lastpos.y_ ,0 };
		Vert[vertCnt].rhw = 1.0f;
		Vert[vertCnt].dif = GetColorU8(255, 255, 255, alpha);
		Vert[vertCnt].u = 1.0f;
		Vert[vertCnt].v = 1.0f;

		vertCnt++;

		Vert[vertCnt].pos = { lastpos.x_ - TrailSizeH,lastpos.y_ ,0 };
		Vert[vertCnt].rhw = 1.0f;
		Vert[vertCnt].dif = GetColorU8(255, 255, 255, alpha);
		Vert[vertCnt].u = 0.0f;
		Vert[vertCnt].v = 1.0f;

		vertCnt++;

		//��
		Vert[vertCnt].pos = { pos.x_ - TrailSizeH,pos.y_,0 };
		Vert[vertCnt].rhw = 1.0f;
		Vert[vertCnt].dif = GetColorU8(255, 255, 255, alpha);
		Vert[vertCnt].u = 0.0f;
		Vert[vertCnt].v = 0.0f;

		vertCnt++;

		Vert[vertCnt].pos = { pos.x_ + TrailSizeH,pos.y_,0 };
		Vert[vertCnt].rhw = 1.0f;
		Vert[vertCnt].dif = GetColorU8(255, 255, 255, alpha);
		Vert[vertCnt].u = 1.0f;
		Vert[vertCnt].v = 0.0f;

		vertCnt++;

		Vert[vertCnt].pos = { lastpos.x_ + TrailSizeH,lastpos.y_,0 };
		Vert[vertCnt].rhw = 1.0f;
		Vert[vertCnt].dif = GetColorU8(255, 255, 255, alpha);
		Vert[vertCnt].u = 1.0f;
		Vert[vertCnt].v = 1.0f;

		vertCnt++;

		//DrawLineAA(lastpos.x_, lastpos.y_, pos_.x_, pos_.y_, 0xffffff, 10);
		//thickness *=0.95f;
		lastpos = pos;
		alpha -= 18;
	}

	//DrawPolygon2D(Vert, trailLimit, DX_NONE_GRAPH, true);

	DrawGraph(pos_.x_, pos_.y_, lpImageMng.GetID("Cursol")[0], true);

	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

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
	alpha = 255;
	if (isTutorial)
	{
		//�`���[�g���A�����̃��l��ς���
		alpha = lpSceneMng.alpha_;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
}

void Cursol::TrailClear(void)
{
	trailList_.clear();
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
