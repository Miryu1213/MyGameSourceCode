#include "DeployMng.h"
#include "Gimmick/Hurricane.h"
#include "../tmx/MapObject.h"
#include "../SceneMng.h"
#include "../../common/FontMng.h"
#include "../../common/ImageMng.h"
#include "../../common/Collision.h"
#include "../../common/Easing.h"
#include "../tmx/TmxObj.h"
#include "../Obj/RouteCtl.h"
#include "../Obj/Plant/KnightPlant.h"
#include "../Obj/Plant/WarriorPlant.h"
#include "../Obj/Plant/ArcherPlant.h"
#include "../SystemObj/WarSituationGraph.h"
#include "../SystemObj/InputString.h"
#include "../SystemObj/Camera.h"
#include "../UI/Circle.h"
#include "../UI/Box.h"

#include "../../_debug/_DebugConOut.h"

//�M�~�b�N�T�C�Y
constexpr int GIMMICK_SIZE_X = 120;
constexpr int GIMMICK_SIZE_Y = 120;

//���̕����T�C�Y
constexpr int WIND_DIVCNT_X = 8;
constexpr int WIND_DIVCNT_Y = 1;

//�~�ړ���
constexpr int CIRCLEMOVE = 100;
//�~�����ʒu
constexpr int DEF_CIRCLE_POS_X = 20 - CIRCLEMOVE;
//�~�摜�T�C�Y
constexpr int CIRCLE_SIZE_X = 180;
constexpr int CIRCLE_SIZE_Y = 180;
//�~���������W
constexpr int CIRCLE_RANGE_X = CIRCLE_SIZE_X + 50;
constexpr int CIRCLE_RANGE_Y = CIRCLE_SIZE_Y + 50;
//�~�ړ�����
constexpr double CIRCLEMOVETIME = 0.8;

//�{�b�N�X�T�C�Y
constexpr int BUTTON_SIZE_X = 64;
constexpr int BUTTON_SIZE_Y = 64;

//�莝���\���n
constexpr int DEPLOY_BETWEEN_WIDTH = 100;

//�莝���̕\���I�t�Z�b�g
//�v�����g
constexpr int DEPLOY_PLANT_OFFSET_X = 20;
constexpr int DEPLOY_PLANT_OFFSET_Y = 26;

//�M�~�b�N
constexpr int DEPLOY_GIMMICK_OFFSET_X = 20;
constexpr int DEPLOY_GIMMICK_OFFSET_Y = 16;

//�t�H���g
constexpr int DEPLOY_FONT_SIZE = 20;

//���Ԃ̐錾
DeployMng* DeployMng::s_Instance = nullptr;

DeployMng::DeployMng(std::list<std::unique_ptr<Obj>>* objlist, int offset)
{
	objList_ = objlist;

	circle_ = new Circle({ DEF_CIRCLE_POS_X ,lpSceneMng.GetViewArea().y_ - offset }, { CIRCLE_SIZE_X,CIRCLE_SIZE_Y });
	box_ = new Box({ 0,lpSceneMng.GetViewArea().y_ - BUTTON_SIZE_Y - offset }, { lpSceneMng.GetViewArea().x_,BUTTON_SIZE_Y + offset });
	box_->SetModelID(lpImageMng.GetID("woodframe02")[0]);

	showboxFlag_ = false;

	gimmickGhMap_[GimmickType::HURRICANE] = lpImageMng.GetID("wind");

	plantGhMap_[ObjType::KNIGHTPLANT] = lpImageMng.GetID("Knightplant")[0];
	plantGhMap_[ObjType::WARRIORPLANT] = lpImageMng.GetID("Warriorplant")[0];
	plantGhMap_[ObjType::ARCHERPLANT] = lpImageMng.GetID("Archerplant")[0];

	grabGimmickFlag_ = false;
	gimType_ = GimmickType::MAX;
	plantType_ = ObjType::MAX;

	nowTime_ = 0.0;
	moveFinishFlag_ = false;
	delayFlag_ = false;

	entryPlantFlag_ = false;
}

DeployMng::~DeployMng()
{
	delete circle_;
	delete box_;
}

void DeployMng::DeployUIFunction(void)
{
	//�J�[�\�����͈͓��ɗ������o�Ă���
	auto curPos = lpSceneMng.cursol_->GetPos();
	if (Collision::CheckCircle({ DEF_CIRCLE_POS_X + CIRCLEMOVE,lpSceneMng.GetViewArea().y_ - lpSceneMng.GetOffset() }, CIRCLE_RANGE_X, curPos) &&
		delayFlag_ == false && lpRouteCtl.routeChangeFlag_ == false)
	{
		//�͈͓�
		if (nowTime_ <= 0)
		{
			nowTime_ = CIRCLEMOVETIME;
		}
	}
	else
	{
		//�͈͊O
		if (moveFinishFlag_ && !showboxFlag_)
		{
			//�߂�
			CircleMoveLeft();
		}
	}

	if (nowTime_ > 0.0 && moveFinishFlag_ == false && delayFlag_ == false)
	{
		//�i��
		CircleMoveRight();
	}

	//�A�����ďo�Ă���̂�h�~
	if (delayFlag_)
	{
		nowTime_ -= lpSceneMng.GetDeltaTime();
		if (nowTime_ <= 0)
		{
			delayFlag_ = false;
		}
	}

}

void DeployMng::DrawindentGraph(Vector2 pos, Vector2 size, int gh)
{
	DrawExtendGraph(pos.x_, pos.y_, pos.x_ + size.x_, pos.y_ + size.y_, gh, true);
}

void DeployMng::SetEntryFlag(bool flag)
{
	entryPlantFlag_ = flag;
}

bool DeployMng::Init(std::string mapname)
{
	//������
	gimList_.clear();

	//�}�b�v�l�[����map+�����ɉ��H
	mapname.erase(0, mapname.find_last_of("/") + 1);
	mapname.erase(mapname.find_last_of("."), static_cast<int>(mapname.size()));

	auto mapObj = MapObject::GetMapObject(mapname);

	plantSum_[ObjType::ARCHERPLANT] = mapObj.apNum_;
	plantSum_[ObjType::KNIGHTPLANT] = mapObj.kpNum_;
	plantSum_[ObjType::WARRIORPLANT] = mapObj.wpNum_;

	gimmickSum_[GimmickType::HURRICANE] = mapObj.huricaneNum_;

	//�{�݂̏ꏊ���܂Ƃ߂Ċi�[
	for (auto& plantpos : lptmxobj.GetAllyPlantPos())
	{
		plantPosList.push_back(plantpos.second);
	}
	for (auto& plantpos : lptmxobj.GetEnemyPlantPos())
	{
		plantPosList.push_back(plantpos.second);
	}

	return true;
}

void DeployMng::UpDate(Vector2 camoffset)
{
	if (lpRouteCtl.routeChangeFlag_)
	{
		return;
	}

	//�J�[�\���̍��W�̓I�t�Z�b�g�����Z����
	auto curPos = lpSceneMng.cursol_->GetPos();

	if (showboxFlag_ == true)
	{
		//���̃{�b�N�X����J�[�\�����O���Ώ�����
		if (!box_->CheckColition(curPos))
		{
			showboxFlag_ = false;
		}
	}

	if (circle_->CheckColition(curPos))
	{
		//�T�[�N���ɓ�����Ή��̃{�b�N�X�\��
		showboxFlag_ = true;
	}

	//�e�M�~�b�N�̍X�V
	for (auto& gimmick : gimList_)
	{
		if (!gimmick->UpDate())
		{
			//�M�~�b�N�̐����t���O��false��
			gimmick->aliveFlag_ = false;
		}
	}

	//�����A�N�V����
	GrabFunction(camoffset);

	//��UI�̏���
	DeployUIFunction();

	//�M�~�b�N�̍폜
	gimList_.remove_if([](std::unique_ptr<BaseGimmick>& gim) { return gim->aliveFlag_ == false; });
}

void DeployMng::Draw()
{
	for (auto& gimmick : gimList_)
	{
		gimmick->Draw();
	}
}

void DeployMng::DrawDeploy(SharedCamera& camera, bool isTutorial)
{
	//���l
	int alpha = 255;

	//���݂̃`���[�g���A���������ȊO ���I�t��ԂłȂ���Δ�������
	if (lpInputString.GetNowPlace() != PLACE::DEPLOY && lpInputString.GetNowPlace() != PLACE::MAX)
	{
		alpha = lpSceneMng.alpha_;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	//�~�̕`��
	circle_->Draw();

	if (showboxFlag_ == true)
	{
		//�{�b�N�X�\����
		box_->Draw();
		//�{�b�N�X���̕`��
		DrawinBox();
	}

	//�����Ă�����̂�`��
	DrawWhatPlayerHas(camera->GetDrawOffset(), isTutorial);
}

void DeployMng::DrawWhatPlayerHas(Vector2 camOffset, bool isTutorial)
{
	if (grabGimmickFlag_)
	{
		auto blocksize = lpRouteCtl.GetBlockSize();
		auto armyMap = lpWarSituationGraph.GetArmyMap();

		//�J�[�\�����̂̍��W
		auto curPos = lpSceneMng.cursol_->GetPos();
		//�}�b�v�S�̂��猩���J�[�\���̍��W
		auto curMapPos = lpSceneMng.cursol_->GetPos() - Vector2(0, lpSceneMng.GetOffset()) - camOffset;

		//�u���b�N�l�֕ϊ�
		Vector2 blockPos = (curMapPos / blocksize) * blocksize;

		//���݂̑I���u���b�N�𔒂�
		DrawBox(blockPos.x_ + camOffset.x_, blockPos.y_ + lpSceneMng.GetOffset(), blockPos.x_ + camOffset.x_ + blocksize, blockPos.y_ + blocksize + +lpSceneMng.GetOffset(), 0xeeeeee, true);

		if (armyMap[(blockPos.y_) / blocksize][blockPos.x_ / blocksize] != SceneMng::Blue)
		{
			//�ݒu�͈͊O
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 158);
		}

		if (gimType_ == GimmickType::MAX)
		{
			//�{�݂̕`��
			DrawGraph(curPos.x_ - blocksize / 2, curPos.y_ - blocksize / 2, plantGhMap_[plantType_], true);
		}
		else
		{
			//�M�~�b�N�̕`��
			DrawGraph(curPos.x_ - GIMMICK_SIZE_X / 2, curPos.y_ - GIMMICK_SIZE_Y / 2, gimmickGhMap_[gimType_][5], true);
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
}

void DeployMng::DrawinBox(void)
{
	//���̃M�~�b�N�ꗗ�̕\��
	auto boxpos = box_->GetPos();
	int i = 0;

	auto fontHandle = lpFontMng.GetFontSizeHandle(FontType::DEFAULT, DEPLOY_FONT_SIZE);

	//�{�ݕ\��
	DrawFormatStringToHandle(20, boxpos.y_ + 5, 0xffffff, fontHandle, "[�{��]");
	for (auto& plant : plantGhMap_)
	{
		DrawGraph(i * DEPLOY_BETWEEN_WIDTH + DEPLOY_PLANT_OFFSET_X, boxpos.y_ + DEPLOY_PLANT_OFFSET_Y, plant.second, true);
		DrawFormatStringToHandle(i * DEPLOY_BETWEEN_WIDTH + lpRouteCtl.GetBlockSize() - 10, boxpos.y_ + DEPLOY_PLANT_OFFSET_Y + lpRouteCtl.GetBlockSize(), 0xffffff, fontHandle, "x %d", plantSum_[plant.first]);
		i++;
	}

	//�M�~�b�N�\��
	DrawFormatStringToHandle(static_cast<int>(plantGhMap_.size())* DEPLOY_BETWEEN_WIDTH + 26, boxpos.y_ + 5, 0xffffff, fontHandle, "[�M�~�b�N]");
	for(auto gimmick : gimmickGhMap_)
	{
		DrawModiGraph(i * DEPLOY_BETWEEN_WIDTH + DEPLOY_GIMMICK_OFFSET_X + 5, boxpos.y_ + DEPLOY_GIMMICK_OFFSET_Y + 10,
			i * DEPLOY_BETWEEN_WIDTH + DEPLOY_GIMMICK_OFFSET_X + 5 + 84, boxpos.y_ + DEPLOY_GIMMICK_OFFSET_Y + 10,
			i * DEPLOY_BETWEEN_WIDTH + DEPLOY_GIMMICK_OFFSET_X + 5 + 84, boxpos.y_ + DEPLOY_GIMMICK_OFFSET_Y + 10 + 64,
			i * DEPLOY_BETWEEN_WIDTH + DEPLOY_GIMMICK_OFFSET_X + 5, boxpos.y_ + DEPLOY_GIMMICK_OFFSET_Y + 10 + 64, gimmick.second[5], true);
		
		DrawFormatStringToHandle(i * DEPLOY_BETWEEN_WIDTH + 5 + 74, boxpos.y_ + DEPLOY_GIMMICK_OFFSET_Y + 64 + 8, 0xffffff,fontHandle, "x %d", gimmickSum_[gimmick.first]);
		i++;
	}
}

void DeployMng::SetGimmick(GimmickType gimmicktype, Vector2 pos, Vector2 size)
{
	switch (gimmicktype)
	{
	case GimmickType::HURRICANE :
		gimList_.push_back(std::make_unique<Hurricane>(pos,size));
		break;
	default:
		//�������Ȃ�
		break;
	}
}

void DeployMng::GrabFunction(Vector2 camoffset)
{
	if (lpRouteCtl.routeChangeFlag_)
	{
		//���[�g�`�F���W���͏���
		return;
	}

	//�����A�N�V����
	auto boxpos = box_->GetPos();
	auto blocksize = lpRouteCtl.GetBlockSize();
	
	if (lpSceneMng.cursol_->GetMouseInput(InputType::NOW)[MouseInputID::LEFT])
	{
		//�}�E�X���N���b�N��
		if (grabGimmickFlag_)
		{
			//������Ԃ̏ꍇ
			return;
		}
		if (!showboxFlag_)
		{
			//�莝�����\������Ă��Ȃ��ꍇ
			return;
		}

		int i = 0;

		//���̃}�X�̃I�u�W�F�N�g�����ݏ�Ԃ�
		if (Collision::CheckSquare({ i * DEPLOY_BETWEEN_WIDTH + DEPLOY_PLANT_OFFSET_X,boxpos.y_ + DEPLOY_PLANT_OFFSET_Y }, { blocksize,blocksize }, lpSceneMng.cursol_->GetPos()))
		{
			gimType_ = GimmickType::MAX;
			plantType_ = ObjType::KNIGHTPLANT;
			grabGimmickFlag_ = true;
		}

		i++;

		if (Collision::CheckSquare({ i * DEPLOY_BETWEEN_WIDTH + DEPLOY_PLANT_OFFSET_X,boxpos.y_ + DEPLOY_PLANT_OFFSET_Y }, { blocksize,blocksize }, lpSceneMng.cursol_->GetPos()))
		{
			gimType_ = GimmickType::MAX;
			plantType_ = ObjType::WARRIORPLANT;
			grabGimmickFlag_ = true;
		}

		i++;

		if (Collision::CheckSquare({ i * DEPLOY_BETWEEN_WIDTH + DEPLOY_PLANT_OFFSET_X,boxpos.y_ + DEPLOY_PLANT_OFFSET_Y }, { blocksize,blocksize }, lpSceneMng.cursol_->GetPos()))
		{
			gimType_ = GimmickType::MAX;
			plantType_ = ObjType::ARCHERPLANT;
			grabGimmickFlag_ = true;
		}

		i++;

		if (Collision::CheckSquare({ i * DEPLOY_BETWEEN_WIDTH + DEPLOY_GIMMICK_OFFSET_X,boxpos.y_ + DEPLOY_GIMMICK_OFFSET_Y }, { GIMMICK_SIZE_X,GIMMICK_SIZE_Y }, lpSceneMng.cursol_->GetPos()))
		{
			gimType_ = GimmickType::HURRICANE;
			plantType_ = ObjType::MAX;
			grabGimmickFlag_ = true;
		}
	}
	else
	{
		if (!grabGimmickFlag_)
		{
			return;
		}

		//�������Ƃ�
		grabGimmickFlag_ = false;

		auto curPos = lpSceneMng.cursol_->GetPos() - Vector2(0, lpSceneMng.GetOffset()) - camoffset;

		if (gimType_ != GimmickType::MAX)
		{
			//�������Ƃ��ɃM�~�b�N����
			if (gimmickSum_[gimType_] != 0)
			{
				SetGimmick(GimmickType::HURRICANE, curPos, { GIMMICK_SIZE_X,GIMMICK_SIZE_Y });
				gimmickSum_[gimType_]--;
			}
		}
		else
		{
			auto armyMap = lpWarSituationGraph.GetArmyMap();
			if (plantSum_[plantType_] == 0)
			{
				//�c�ʂ��Ȃ��ꍇ
				lpInputString.CreateFadeOutString("��������0�ł�");
				return;
			}

			if (lpRouteCtl.costMap_[curPos.y_ / blocksize][curPos.x_ / blocksize] == lpRouteCtl.GetCantMovePoint())
			{
				//�ݒu�s�}�X
				lpInputString.CreateFadeOutString("���̃}�X�͐ݒu�ł��܂���");
				return;
			}

			if (armyMap[curPos.y_ / blocksize][curPos.x_ / blocksize] != SceneMng::Blue)
			{
				//���̒n�łȂ�
				lpInputString.CreateFadeOutString("���̒n�ł͂���܂���");
				return;
			}

			auto allyCorePos = lptmxobj.GetAllyCorePos();
			//�R�A��1�}�X����Ă���
			if ((curPos.x_ / blocksize == allyCorePos.x_ && curPos.y_ / blocksize == allyCorePos.y_ - 1) ||
				(curPos.x_ / blocksize == allyCorePos.x_ && curPos.y_ / blocksize == allyCorePos.y_))
			{
				//�����R�A�Ƃ��̏�̃}�X�I����
				lpInputString.CreateFadeOutString("�R�A�̏�ɂ͐ݒu�ł��܂���");
				return;
			}

			auto enemyCorePos = lptmxobj.GetEnemyCorePos();
			if ((curPos.x_ / blocksize == enemyCorePos.x_ && curPos.y_ / blocksize == enemyCorePos.y_ - 1) ||
				(curPos.x_ / blocksize == enemyCorePos.x_ && curPos.y_ / blocksize == enemyCorePos.y_))
			{
				//�G�R�A�Ƃ��̏�̃}�X�I����
				lpInputString.CreateFadeOutString("�R�A�̏�ɂ͐ݒu�ł��܂���");
				return;
			}

			for (auto& plant : plantPosList)
			{
				if (curPos / blocksize == plant)
				{
					//�����z�u�{�݂̏��I����
					lpInputString.CreateFadeOutString("���̃}�X�ɂ͂��łɐݒu����Ă��܂�");
					return;
				}
			}
			
			//�������Ƃ��{�ݐݒu
			entryPlantFlag_ = true;
			plantPosList.push_back(curPos / blocksize);

			entryPlantPos_ = curPos / blocksize * blocksize;

			//�J�E���g�����炷
			plantSum_[plantType_]--;
		}
	}
}

void DeployMng::CircleMoveRight(void)
{
	int amountofmove = static_cast<int>(circle_->GetPos().x_ + Easing::OutQuart(lpSceneMng.GetDeltaTime(), CIRCLEMOVETIME) * CIRCLEMOVE);

	circle_->SetPos({ amountofmove, circle_->GetPos().y_ });

	nowTime_ -= lpSceneMng.GetDeltaTime();

	//�ړ�����
	if (circle_->GetPos().x_ > DEF_CIRCLE_POS_X + CIRCLEMOVE)
	{
		circle_->SetPos({ DEF_CIRCLE_POS_X + CIRCLEMOVE, circle_->GetPos().y_ });

		nowTime_ = 0.0;

		moveFinishFlag_ = true;
	}
}

void DeployMng::CircleMoveLeft(void)
{
	int amount = abs((int)(Easing::OutQuart(lpSceneMng.GetDeltaTime(), CIRCLEMOVETIME) * CIRCLEMOVE));
	circle_->SetPos({ circle_->GetPos().x_ + (-amount), circle_->GetPos().y_ });

	//�ړ�����
	if (circle_->GetPos().x_ <= DEF_CIRCLE_POS_X)
	{
		circle_->SetPos({ DEF_CIRCLE_POS_X , circle_->GetPos().y_ });
		delayFlag_ = true;
		nowTime_ = 0.2;
		moveFinishFlag_ = false;
	}
}
