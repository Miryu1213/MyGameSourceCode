#include <DxLib.h>
#include "Hurricane.h"
#include "../DeployMng.h"
#include "../../SceneMng.h"

constexpr double FRAME_SUM = 7;
constexpr double ONE_FRAME_TIME = 0.12;

//�_���[�W��
constexpr int AMO_DANEGE = 70;

//�n���P�[�����ݎ���
constexpr double HARRICANE_TIME = FRAME_SUM * ONE_FRAME_TIME;

Hurricane::Hurricane(Vector2 pos, Vector2 size)
{
	pos_ = pos;
	size_ = size;
}

Hurricane::~Hurricane()
{
}

void Hurricane::Init(void)
{
}

bool Hurricane::UpDate(void)
{
	//���Ԃ��J�E���g
	elapTime_ += lpSceneMng.GetDeltaTime();
	if (elapTime_ > HARRICANE_TIME)
	{
		for (auto objitr = lpDeployMng.objList_->begin(); objitr != lpDeployMng.objList_->end(); objitr++)
		{
			if ((*objitr)->GetGimmickFlag(GimmickType::HURRICANE))
			{
				//�M�~�b�N�t���O���I�t
				(*objitr)->SetGimmickFlag(GimmickType::HURRICANE, false);
			}
		}
		return false;
	}

	for (auto objitr = lpDeployMng.objList_->begin(); objitr != lpDeployMng.objList_->end(); objitr++)
	{
		//�M�~�b�N�͖������ɂ͋@�\���Ȃ�
		if ((*objitr)->GetArmyType() == ArmyType::ALLY)
		{
			continue;
		}

		//���j�b�g�ȊO�͏Ȃ�
		if ((*objitr)->GetGenerateType() != GenerateType::UNIT)
		{
			continue;
		}

		if (Collision::CheckSquare(pos_ - size_ / 2, size_, (*objitr)->GetPos(), (*objitr)->GetSize()))
		{
			if (!(*objitr)->GetGimmickFlag(GimmickType::HURRICANE))
			{
				//HP�ቺ
				(*objitr)->DamegeHP(AMO_DANEGE, nullptr);
				
				//�M�~�b�N�t���O���I��
				(*objitr)->SetGimmickFlag(GimmickType::HURRICANE, true);
			}
		}
		else
		{
			if ((*objitr)->GetGimmickFlag(GimmickType::HURRICANE))
			{
				//�M�~�b�N�t���O���I�t
				(*objitr)->SetGimmickFlag(GimmickType::HURRICANE, false);
			}
		}
	}
	return true;
}

void Hurricane::Draw(void)
{
	int id = static_cast<int>(fmod(elapTime_ / ONE_FRAME_TIME, FRAME_SUM));
	
	DrawGraph(pos_.x_ - size_.x_ / 2, pos_.y_ - size_.y_ / 2, lpDeployMng.gimmickGhMap_[GimmickType::HURRICANE][id], true);
}
