#include <cmath>
#include "Signal.h"
#include "../SceneMng.h"
#include "../Obj/RouteCtl.h"
#include "../../common/Easing.h"

//�J���[
constexpr UINT32 WHITE = 0xffffff;
constexpr UINT32 LIGHTBLUE = 0x00c9e8;
constexpr UINT32 BLUE = 0x0000e8;
constexpr UINT32 ORANGE = 0xf86400;
constexpr UINT32 RED = 0xe80000;

//�O�p�`�p�x
constexpr float TRIANGLE_DEGREE = 80.0f;

//�V�O�i���\������
constexpr double PLANT_SIGNAL_TIME = 6.0;
constexpr double CORE_SIGNAL_TIME = 8.0;

//�V�O�i���T�C�Y
constexpr float PLANT_SIGNAL_SIZE = 30.0f;
constexpr float CORE_SIGNAL_SIZE = 50.0f;

constexpr double PLANT_RATE = 0.3;
constexpr double CORE_RATE = 0.65;

Signal::Signal(Vector2 camPos, Vector2 targetPos, Vector2 camOffset, SignalType signalType)
{
	switch (signalType)
	{
	case SignalType::ALLYPLANT:
		gh_ = lpImageMng.GetID("SigTriAllyPlant")[0];
		extRate_ = PLANT_RATE;
		loadingTime_ = PLANT_SIGNAL_TIME;
		maxTime_ = PLANT_SIGNAL_TIME;
		break;
	case SignalType::ENEMYPLANT:
		gh_ = lpImageMng.GetID("SigTriEnemyPlant")[0];
		extRate_ = PLANT_RATE;
		loadingTime_ = PLANT_SIGNAL_TIME;
		maxTime_ = PLANT_SIGNAL_TIME;
		break;
	case SignalType::ALLYCOREDISPLAY:
		gh_ = lpImageMng.GetID("SigTriAlly")[0];
		extRate_ = CORE_RATE;
		loadingTime_ = CORE_SIGNAL_TIME;
		maxTime_ = CORE_SIGNAL_TIME;
		break;
	case SignalType::ENEMYCOREDISPLAY:
		gh_ = lpImageMng.GetID("SigTriEnemy")[0];
		extRate_ = CORE_RATE;
		loadingTime_ = CORE_SIGNAL_TIME;
		maxTime_ = CORE_SIGNAL_TIME;
		break;
	case SignalType::ALLYCOREATTACKED:
		gh_ = lpImageMng.GetID("SigTriEnemy")[0];
		extRate_ = CORE_RATE;
		loadingTime_ = CORE_SIGNAL_TIME;
		maxTime_ = CORE_SIGNAL_TIME;
		break;
	case SignalType::ENEMYCOREATTACK:
		gh_ = lpImageMng.GetID("SigTriAlly")[0];
		extRate_ = CORE_RATE;
		loadingTime_ = CORE_SIGNAL_TIME;
		maxTime_ = CORE_SIGNAL_TIME;
		break;
	default:
		break;
	}
	
	//�^�[�Q�b�g�̊i�[
	targetPos_ = targetPos;

	//��_����p�x���Z�o
	angleRad_ = static_cast<float>(Easing::RadIn2PI(atan2(targetPos.y_ - camPos.y_, targetPos.x_ - camPos.x_)));

	//�J�����̒��S����^�[�Q�b�g�܂ł̃x�N�g���̐��K��
	Vector2Flt targetPosFloat = { static_cast<float>(targetPos.x_), static_cast<float>(targetPos.y_) };
	Vector2Flt camPosFloat = { static_cast<float>(camPos.x_), static_cast<float>(camPos.y_) };
	Vector2Flt vec = Vector2Flt(targetPosFloat.x_, targetPosFloat.y_) - Vector2Flt(camPosFloat.x_, camPosFloat.y_);
	vec = vec.Normalized();

	//���E����
	if (angleRad_ > (DX_TWO_PI_F ) / 4 && angleRad_ < DX_TWO_PI_F * 3 / 4)
	{
		//�^�[�Q�b�g����
	
		//�x�N�g���𔽓]
		vec.x_ *= -1;
		vec.y_ *= -1;

		//�{��
		float magni = 0.0f;

		//��ʊO�̏ꍇ�͉�ʒ[
		auto target = (targetPos_.x_ - camPos.x_) < -lpSceneMng.GetViewArea().x_ / 2 ? -camOffset.x_ : targetPos_.x_;

		//�^�[�Q�b�g�܂ł̔{��
		magni = (target - camPos.x_) / vec.x_;

		//�ړ���
		auto moveVec = (vec * magni);

		pos_ = Vector2Flt(static_cast<float>(lpSceneMng.GetViewArea().x_ / 2), static_cast<float>(lpSceneMng.GetViewArea().y_ / 2)) + moveVec;

		//���݂̃^�[�Q�b�g���{�݂̏ꍇ
		if (target == targetPos_.x_)
		{
			//���W��1�u���b�N���E�ɂ��炷
			pos_.x_ += lpRouteCtl.GetBlockSize();
		}
	}
	else
	{
		//�^�[�Q�b�g���E
	
		//�{��
		float magni = 0.0f;

		//��ʊO�̏ꍇ�͉�ʒ[
		auto target = (targetPos.x_ - camPos.x_) < lpSceneMng.GetViewArea().x_ / 2 ? targetPos.x_ : lpSceneMng.GetViewArea().x_ - camOffset.x_;

		//�^�[�Q�b�g�܂ł̔{��
		magni = (target - camPos.x_) / vec.x_;

		//�ړ���
		auto moveVec = (vec * magni);

		pos_ = Vector2Flt(static_cast<float>(lpSceneMng.GetViewArea().x_ / 2), static_cast<float>(lpSceneMng.GetViewArea().y_ / 2)) + moveVec;
	}

	//�^�[�Q�b�g����ɂ���ꍇ�̓u���b�N�T�C�Y���J�E���g
	if (targetPos.y_ < camPos.y_)
	{
		pos_.y_ += lpRouteCtl.GetBlockSize();
	}

	//�I�t�Z�b�g�����炷
	pos_.y_ += lpSceneMng.GetOffset();
}

bool Signal::Update(Vector2 camPos, Vector2 camOffset, bool IsTutorial)
{
	if (!IsTutorial)
	{
		if (loadingTime_ <= 0)
		{
			return true;
		}
	}

	//�Čv�Z
	//��_����p�x���Z�o
	angleRad_ = static_cast<float>(Easing::RadIn2PI(atan2(targetPos_.y_ - camPos.y_, targetPos_.x_ - camPos.x_)));

	//�J�����̒��S����^�[�Q�b�g�܂ł̃x�N�g���̐��K��
	Vector2Flt targetPosFloat = { static_cast<float>(targetPos_.x_), static_cast<float>(targetPos_.y_) };
	Vector2Flt camPosFloat = { static_cast<float>(camPos.x_), static_cast<float>(camPos.y_) };
	Vector2Flt vec = Vector2Flt(targetPosFloat.x_, targetPosFloat.y_) - Vector2Flt(camPosFloat.x_, camPosFloat.y_);
	if (vec.x_ != 0)
	{
		vec = vec.Normalized();
	}

	//���E����
	if (angleRad_ > (DX_TWO_PI_F) / 4 && angleRad_ < DX_TWO_PI_F * 3 / 4)
	{
		//�^�[�Q�b�g����

		//�x�N�g���𔽓]
		vec.x_ *= -1;
		vec.y_ *= -1;

		//�{��
		float magni = 0.0f;

		//��ʊO�̏ꍇ�͉�ʒ[
		auto target = (targetPos_.x_ - camPos.x_) < -lpSceneMng.GetViewArea().x_ / 2 ? -camOffset.x_ : targetPos_.x_;

		//�^�[�Q�b�g�܂ł̔{��
		if (vec.x_ == 0)
		{
			magni = 1;
		}
		else
		{
			magni = (target - camPos.x_) / vec.x_;
		}

		//�ړ���
		auto moveVec = (vec * magni);

		pos_ = Vector2Flt(static_cast<float>(lpSceneMng.GetViewArea().x_ / 2), static_cast<float>(lpSceneMng.GetViewArea().y_ / 2)) + moveVec;

		//���݂̃^�[�Q�b�g���{�݂̏ꍇ
		if (target == targetPos_.x_)
		{
			//���W��1�u���b�N���E�ɂ��炷
			pos_.x_ += lpRouteCtl.GetBlockSize();
		}
	}
	else
	{
		//�^�[�Q�b�g���E

		//�{��
		float magni = 0.0f;

		//��ʊO�̏ꍇ�͉�ʒ[
		auto target = (targetPos_.x_ - camPos.x_) < lpSceneMng.GetViewArea().x_ / 2 ? targetPos_.x_ : lpSceneMng.GetViewArea().x_ - camOffset.x_;

		//�^�[�Q�b�g�܂ł̔{��
		if (vec.x_ == 0)
		{
			magni = 1;
		}
		else
		{
			magni = (target - camPos.x_) / vec.x_;
		}

		//�ړ���
		auto moveVec = (vec * magni);

		pos_ = Vector2Flt(static_cast<float>(lpSceneMng.GetViewArea().x_ / 2), static_cast<float>(lpSceneMng.GetViewArea().y_ / 2)) + moveVec;
	}

	//�^�[�Q�b�g����ɂ���ꍇ�̓u���b�N�T�C�Y���J�E���g
	if (targetPos_.y_ < camPos.y_)
	{
		pos_.y_ += lpRouteCtl.GetBlockSize();
	}

	//�I�t�Z�b�g�����炷
	pos_.y_ += lpSceneMng.GetOffset();

	loadingTime_ -= lpSceneMng.GetDeltaTime();
	return false;
}

void Signal::Draw(bool isTutorial)
{
	if (loadingTime_ > maxTime_ * 3 / 4 && fmod(loadingTime_, 0.2) <= 0.095)
	{
		//�o�������͓_��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
	}

	DrawRotaGraphF(pos_.x_, pos_.y_, extRate_, angleRad_, gh_, true);
	
	//���l
	int alpha = 255;
	if (isTutorial)
	{
		//�`���[�g���A�����̃��l��ς���
		alpha = lpSceneMng.alpha_;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
}
