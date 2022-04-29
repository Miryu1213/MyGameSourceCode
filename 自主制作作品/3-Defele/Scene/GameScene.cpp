#include <string>
#include "GameScene.h"
#include "Transition/FadeInOut.h"
#include "ResultScene.h"
#include "SceneMng.h"
#include "Deploy/DeployMng.h"
#include "Obj/Unit/Smoke.h"
#include "Obj/Unit/AllUnitHeader.h"
#include "Obj/Plant/AllPlantHeader.h"
#include "Obj/Core.h"
#include "Obj/RouteCtl.h"
#include "Obj/SearchMapCtl.h"
#include "SystemObj/InputString.h"
#include "SystemObj/WarSituationGraph.h"
#include "UI/Circle.h"
#include "UI/Box.h"
#include "tmx/TmxObj.h"
#include "tmx/MapObject.h"
#include "../common/ImageMng.h"
#include "../common/AudioMng.h"
#include "../common/FontMng.h"
#include "../common/Easing.h"

#include "../_debug/_DebugConOut.h"

//�R�A�̎��͉��}�X�������̂��̂ɂ��邩
constexpr int CORERANGE = 5;

//UI
constexpr int UI_OFFSET_X = 4;
constexpr int UI_OFFSET_Y = 8;
constexpr int UI_SIZE_X = 64;
constexpr int UI_SIZE_Y = 24;

//�{��
//�ő�{��
constexpr double MAX_MAG = 3.0;
//��x�ɏオ��l
constexpr double MAG_GOUP_ONCE = 1.0;

//�v�����g�ݒu���̗����ʒu�I�t�Z�b�g
constexpr int FALL_PLANT_POS_OFFSET = 34;
//��������
constexpr double FALL_PLANT_TOTAL_TIME = 0.65;

//�y��
//�ݒu���̓y��������
constexpr int TOTAL_SMOKE = 16;

constexpr int CONVERGENCE_SUM = 23;
constexpr int EXPLOSION_SUM = 16;

constexpr double WAIT_TIME = 1.2;
constexpr double CONVERGENCE_TOTAL_ANIM_TIME = 1.5;
constexpr double EXP_TOTAL_ANIM_TIME = 1.6;

constexpr int BLOCK_SIZE_X = 64;
constexpr int BLOCK_SIZE_Y = 64;

//BGM�{�����[��
constexpr int TUTORIAL_BGM_VOLUME = 130;
constexpr int GAME_BGM_VOLUME = 180;

GameScene::GameScene(const char* mapname, bool isTutorial)
{
	//�}�b�v�l�[����map+�����ɉ��H
	mapName_ = mapname;
	musicName_ = mapname;

	mapName_.erase(0, mapName_.find_last_of("/") + 1);
	mapName_.erase(mapName_.find_last_of("."), static_cast<int>(mapName_.size()));
	musicName_ = "Resource/Audio/BGM/" + mapName_ + ".mp3";

	//�`���[�g���A���֌W������
	isTutorial_ = isTutorial;

	//�z�u�N���X�̍쐬
	lpDeployMng.Create(&objList_, lpSceneMng.GetOffset());
	lpDeployMng.Init(mapname);

	Init();
}

GameScene::~GameScene()
{
	//�틵�̍폜
	lpWarSituationGraph.Destroy();

	//�z�u�N���X�̔j��
	lpDeployMng.Destroy();

	//���[�g�N���X�̔j��
	lpRouteCtl.Destroy();

	lpImageMng.DeleteID("Shadow");
	lpImageMng.DeleteID("WarSituation");
	lpImageMng.DeleteID("AllyText");
	lpImageMng.DeleteID("EnemyText");
	lpImageMng.DeleteID("BlinkLight");
	lpImageMng.DeleteID("Convergence");
	lpImageMng.DeleteID("Explosion");
	
	lpAudioMng.DeleteID("KnightAtk");
	lpAudioMng.DeleteID("ArcherAtk");
	lpAudioMng.DeleteID("WarriorAtk");
	lpAudioMng.DeleteID("AttackedCore");
	lpAudioMng.DeleteID("AttackedPlant");
	lpAudioMng.DeleteID("BrokenCore");
	lpAudioMng.DeleteID("BrokenPlant");
	lpAudioMng.DeleteID("RepelArrow");
	lpAudioMng.DeleteID("DeployPlant");
}

bool GameScene::Init()
{
	auto blocksize = lpRouteCtl.GetBlockSize();
	auto tilesize = lptmxobj.GettileSize();
	auto worldarea = lptmxobj.GetworldArea();

	//�}�b�v�f�[�^�����ƂɃT�C�Y�̌���
	lpSceneMng.SetWorldSize(Vector2(worldarea.x_ * tilesize.x_, worldarea.y_ * tilesize.y_));
	//��ʃT�C�Y�Đݒ�
	offsetGh_ = MakeScreen(lpSceneMng.GetWorldSize().x_, lpSceneMng.GetWorldSize().y_, true);
	//�ő�u���b�N���̐ݒ�
	limitNumBlock_ = (worldarea * tilesize) / blocksize;

	//�J�����̏�����
	camera_->InitArea();

	//�����z�u�Z�b�g
	SetPlantAndCore();

	//�틵�̍쐬
	lpWarSituationGraph.Create(limitNumBlock_);
	//�����}�X�ڂ̒ǉ�
	lpWarSituationGraph.AddInitSquares(CORERANGE);

	//UI�֌W�̍쐬
	CreateUI();

	//���y���쐬���Ă��ꍇ�~�߂�
	lpAudioMng.StopMusicAll();
	
	if (isTutorial_)
	{
		SetVolumeMusicMem(TUTORIAL_BGM_VOLUME, lpAudioMng.GetID(musicName_));
	}
	else
	{
		SetVolumeMusicMem(GAME_BGM_VOLUME, lpAudioMng.GetID(musicName_));
	}
	//���y�Đ�
	PlaySoundMem(lpAudioMng.GetID(musicName_), DX_PLAYTYPE_LOOP);

	//�����o������
	deployPlantFunc_ = nullptr;
	isBackGroundLoading_ = false;

	effectFlag_ = false;
	oneFlag_ = false;
	waitEndFlag_ = false;
	convergenceEndFlag_ = false;
	expAnimTime_ = 0.0;
	
	return true;
}

UniqueScene GameScene::UpDate(UniqueScene ownScene)
{
	//�J�[�\���̍X�V
	lpSceneMng.cursol_->Update();

	//�J�����̍X�V
	camera_->UpDate();

	//�I�u�W�F�N�g�̍X�V �ǉ� �V�O�i���̍쐬
	for (auto objitr = objList_.begin(); objitr != objList_.end();)
	{
		//�X�V
		if (!(*objitr)->UpDate(objList_, camera_))
		{
			//�I�u�W�F�N�g���Ŏ�
			(*objitr)->aliveFlag_ = false;
		}

		//�G�t�F�N�g
		if ((*objitr)->effectFlag_)
		{
			if (!oneFlag_)
			{
				corePos_ = (*objitr)->GetPos();
				effectFlag_ = true;
				oneFlag_ = true;
			}
		}

		//�ǉ�
		if ((*objitr)->GetGenerateFlag())
		{
			//���[�g�̋�`�F�b�N
			if ((*objitr)->GetRoute().empty())
			{
				continue;
			}

			//���j�b�g�̐����E�ǉ�
			GenerateUnit(objitr);
			(*objitr)->SetGenerateFlag(false);
		}

		//�V�O�i���̒ǉ�
		if ((*objitr)->signalFlag_)
		{
			SignalType signalType;
			if ((*objitr)->GetGenerateType() == GenerateType::PLANT)
			{
				signalType = SignalType::ENEMYPLANT;
			}
			else
			{
				if ((*objitr)->GetArmyType() == ArmyType::ALLY)
				{
					signalType = SignalType::ALLYCOREATTACKED;
				}
				else
				{
					signalType = SignalType::ENEMYCOREATTACK;
				}
			}
			
			signalList_.push_back(std::make_unique<Signal>(camera_->GetPos(), (*objitr)->GetPos(), camera_->GetDrawOffset(), signalType));

			(*objitr)->signalFlag_ = false;
		}
		objitr++;
	}

	//�I�u�W�F�N�g�̍폜
	for (auto objitr = objList_.begin(); objitr != objList_.end();)
	{
		//�폜
		if (!(*objitr)->aliveFlag_)
		{
			//�R�A�j��
			if ((*objitr)->GetGenerateType() == GenerateType::CORE)
			{
				if ((*objitr)->GetArmyType() == ArmyType::ENEMY)
				{
					lpSceneMng.clearMap_[mapName_] = true;
				}
				return std::make_unique<FadeInOut>(std::move(ownScene), std::make_unique<ResultScene>((*objitr)->GetArmyType()), 1.0);
			}

			//�����̃A�h���X�𑼂������Ă���΍폜
			std::list<UniqueObj>::iterator tmpItr = objList_.begin();
			while (1)
			{
				const auto& checkItr = std::find_if(tmpItr, objList_.end(), [&](UniqueObj& obj) { return obj->enemy_ == &(*(*objitr)); });
				if (checkItr == objList_.end())
				{
					//���̃I�u�W�F�N�g�S�Ă������Ă��Ȃ��ꍇ
					break;
				}
				(*checkItr)->enemy_ = nullptr;

				tmpItr = checkItr;
			}

			objitr = objList_.erase(objitr);
			continue;
		}
		objitr++;
	}

	//�v�����g�̒ǉ�
	if (lpDeployMng.GetEntryFlag())
	{
		auto blocksize = lpRouteCtl.GetBlockSize();
		//�{�݂������Ă��鉉�o�I�����ݒu
		if (deployPlantFunc_ == nullptr)
		{
			auto entryPlantPos = lpDeployMng.GetEntryPlantPos();

			deployPlantFunc_ = new MoveObjectVoidVoid(
				Vector2(entryPlantPos.x_, entryPlantPos.y_ - FALL_PLANT_POS_OFFSET),
				entryPlantPos, MoveType::OUTEXP, FALL_PLANT_TOTAL_TIME,
				lpImageMng.ConvertObjTypeToKey(lpDeployMng.GetPlantType(), ArmyType::ALLY),
				std::bind(&GameScene::DeployPlant, this));
		}
		
		//�����I�����Ƀv�����g�ǉ��������s��
		deployPlantFunc_->Update()();
	}

	//�v�����g�ݒu���̓y��
	if (!smokeVArray_.empty())
	{
		for (auto smokeItr = smokeVArray_.begin(); smokeItr != smokeVArray_.end();)
		{
			if ((*smokeItr)->Update())
			{
				smokeItr = smokeVArray_.erase(smokeItr);
				continue;
			}
			smokeItr++;
		}
	}

	if (lpRouteCtl.routeChangeEndFlag_)
	{
		//���[�g�ύX���I��
		lpRouteCtl.routeChangeEndFlag_ = false;
	}

	UpDateUI();

	//�����A�N�V�����n�̍X�V
	lpDeployMng.UpDate(camera_->GetDrawOffset());

	return std::move(ownScene);
}

void GameScene::UpDateUI(void)
{
	//�}�E�X�̒l�̎擾
	auto mouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::NOW);
	auto oldmouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::OLD);

	auto blocksize = lpRouteCtl.GetBlockSize();

	//�V�O�i���̍X�V
	for (auto signalItr = signalList_.begin(); signalItr != signalList_.end();)
	{
		if ((*signalItr)->Update(camera_->GetPos(),camera_->GetDrawOffset()))
		{
			//�폜
			signalItr = signalList_.erase(signalItr);
			continue;
		}
		signalItr++;
	}

	//�{�^���n
	//���[�g�I�𒆂�UI�i�{�^���j����
	if (lpRouteCtl.routeChangeFlag_)
	{
		bool buttonOn = false;
		for (auto& ui : lpRouteCtl.GetUIList())
		{
			//�{�^�������`�F�b�N

			//�}�E�X���̃`�F�b�N
			if (!((mouseInput[MouseInputID::LEFT] && !oldmouseInput[MouseInputID::LEFT]) &&
				(ui->CheckColition(lpSceneMng.cursol_->GetPos()))))
			{
				if (mouseInput[MouseInputID::LEFT] && !oldmouseInput[MouseInputID::LEFT])
				{
					//���͂���Ă����
					buttonOn = true;
				}

				//�p�b�h���̃`�F�b�N
				if (!(ui->CheckButtonInput(mouseInput[ui->GetCheckId()], oldmouseInput[ui->GetCheckId()])))
				{
					continue;
				}
			}

			buttonOn = true;
			if (ui->GetFunction()())
			{
				//���[�g�ύX�I����
				//���[�g�`�F���W���Ă������̂����o�����[�g�ύX
				
				for (const auto& route : lpRouteCtl.GetRouteAll(ArmyType::ALLY))
				{
					for (auto& object : objList_)
					{
						if (object->GetArmyType() == ArmyType::ENEMY)
						{
							continue;
						}
						if (object->GetGenerateType() != GenerateType::PLANT)
						{
							continue;
						}
						if (route.first != object->GetGeneOrder())
						{
							continue;
						}
						if (route.second == object->GetRoute())
						{
							continue;
						}
						object->SetRoute(route.second);
					}
					/*auto routeChangeObj = std::find_if(objList_.begin(), objList_.end(), [&](UniqueObj& obj) { return obj->GetRoute() != route[obj->GetGeneOrder()]; });
					if (routeChangeObj != objList_.end())
					{
						(*routeChangeObj)->SetRoute(lpRouteCtl.GetRoute(ArmyType::ALLY, (*routeChangeObj)->GetGeneOrder()));
						(*routeChangeObj)->SetRouteChangeFlag(false);
					}*/
				}
			}
		}

		//�{�^�������`�F�b�N
		if (buttonOn)
		{
			//�{�݂�I���ł��Ă��Ȃ��Ƃ��@���@���[�g�����Z�b�g����Ă���ꍇ
			if (lpRouteCtl.currentPlantOrder_ == 0 && lpRouteCtl.routeResetFlag_)
			{
				lpInputString.CreateFadeOutString("�����̎{�݂�I�����Ă�������");
			}
		}
	}

	//UI����
	//���[�g�`�F���W���ƃ��[�g�`�F���W�I���͏������Ȃ�
	if (!lpRouteCtl.routeChangeFlag_ && !lpRouteCtl.routeChangeEndFlag_)
	{
		//UI���X�g�̓����蔻��
		for (auto& ui : btnList_)
		{
			//�͈͓��`�F�b�N
			auto curPos = lpSceneMng.cursol_->GetPos();

			if (ui->CheckColition(lpSceneMng.cursol_->GetPos()))
			{
				//�{�^�������`�F�b�N
				if ((mouseInput[MouseInputID::LEFT] && !oldmouseInput[MouseInputID::LEFT]) ||
					(ui->CheckButtonInput(mouseInput[ui->GetCheckId()], oldmouseInput[ui->GetCheckId()])))
				{
					ui->GetFunction()();
				}
			}
		}
	}

	//�}�E�X�ł̔{������
	if (mouseInput[MouseInputID::MIDDLE] != 0 && oldmouseInput[MouseInputID::MIDDLE] == 0)
	{
		if (mouseInput[MouseInputID::MIDDLE] < 0)
		{
			CntDownDoubleElapTime();
		}
		if (0 < mouseInput[MouseInputID::MIDDLE])
		{
			//��������
			CntUpDoubleElapTime();
		}
	}
}

void GameScene::Draw(void)
{
	BeforeDraw();

	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	auto BlockSize = lpRouteCtl.GetBlockSize();
	Vector2 camOffset = camera_->GetDrawOffset();

	//���̔w�i�`��
	if (!isTutorial_)
	{
		DrawExtendGraph(0, 0, lpSceneMng.GetViewArea().x_, static_cast<int>(lpSceneMng.GetOffset() * 2.0), lpImageMng.GetID(mapName_)[0], true);
	}

	//�I�t�Z�b�g�����炵�ĕ`��
	//�}�b�v�̕`��
	if (!isBackGroundLoading_)
	{
		if (isTutorial_)
		{
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		//�w�i�����[�h���Ă��Ȃ���΃��[�h
		SaveDrawnTMX();
		if (isTutorial_)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, lpSceneMng.alpha_);
		}
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
	}
	DrawGraph(camOffset.x_, camOffset.y_ + lpSceneMng.GetOffset(), mapGh_, true);

	//�틵�̕`��
	lpWarSituationGraph.Draw(camOffset);

	//�I�u�W�F�N�g�n�̕`��
	DrawGraph(camOffset.x_, camOffset.y_ + lpSceneMng.GetOffset(), offsetGh_, true);

	//�莝���̕`��
	lpDeployMng.DrawDeploy(camera_, isTutorial_);
	//UI�̕`��
	DrawUI();
	//�V�O�i���̕`��
	for (auto& signal : signalList_)
	{
		signal->Draw(isTutorial_);
	}

	DrawBreakedCore();

	//�J�����̕`��
	camera_->Draw();

	//�V�X�e�����b�Z�[�W�̕`��
	lpInputString.FedeOutDraw();

	//�J�[�\���̕`��
	lpSceneMng.cursol_->Draw(isTutorial_);
}

void GameScene::BeforeDraw(void)
{
	//�I�t�Z�b�g�����炵�ĕ`��
	SetDrawScreen(offsetGh_);
	ClearDrawScreen();

	//�u���b�N�T�C�Y
	auto BlockSize = lpRouteCtl.GetBlockSize();
	//�J�����I�t�Z�b�g
	Vector2 camOffset = camera_->GetDrawOffset();

	//���[�g�ύX���̕`��
	DrawRouteChange();

	//�y���`��
	if (!smokeVArray_.empty())
	{
		for (auto& smoke : smokeVArray_)
		{
			smoke->Draw();
		}
	}

	//�e�I�u�W�F�N�g�̕`��
	for (auto& obj : objList_)
	{
		obj->Draw(camera_);
	}

	//�{�݂̗����`��
	if (deployPlantFunc_ != nullptr)
	{
		deployPlantFunc_->Draw();
	}

	//�M�~�b�N�`��
	lpDeployMng.Draw();
}

void GameScene::DrawUI(void)
{
	//���l
	int alpha = 255;

	//���݂̃`���[�g���A���������ȊO���Ɣ�������
	if (lpInputString.GetNowPlace() != PLACE::BUTTON && lpInputString.GetNowPlace() != PLACE::MAX)
	{
		alpha = lpSceneMng.alpha_;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	if (lpRouteCtl.routeChangeFlag_)
	{
		//���[�g�ύX���̏㕔�̕`��
		for (auto& ui : lpRouteCtl.GetUIList())
		{
			//�͈͓����g���\��
			bool frameFlag = false;
			if (ui->CheckColition(lpSceneMng.cursol_->GetPos()))
			{
				frameFlag = true;
			}
			ui->Draw(frameFlag);
		}
	}
	else
	{
		//�ʏ펞�㕔UI
		//UI���X�g�̕`��-��̔{���Ȃ�
		for (auto& ui : btnList_)
		{
			//�͈͓����g���\��
			bool frameFlag = false;
			if (ui->CheckColition(lpSceneMng.cursol_->GetPos()))
			{
				frameFlag = true;
			}
			ui->Draw(frameFlag,20, std::to_string(static_cast<int>(lpSceneMng.doubleValue_)));
		}
	}
}

void GameScene::SaveDrawnTMX(void)
{
	//��ʍ쐬
	offsetGh_ = MakeScreen(lpSceneMng.GetWorldSize().x_, lpSceneMng.GetWorldSize().y_, true);
	mapGh_ = 0;

	auto blocksize = lpRouteCtl.GetBlockSize();
	auto worldarea = lptmxobj.GetworldArea();
	auto tilesize = lptmxobj.GettileSize();

	mapGh_ = MakeScreen(worldarea.x_ * blocksize, worldarea.y_ * blocksize, true);

	if (SetDrawScreen(mapGh_) == -1)
	{
		return;
	}
	ClsDrawScreen();

	//���݂̃��C���[ID
	int id = 0;
	//id - layerId�̍ŏ��i���o�[
	int minlayerid = 5000;
	//���背�C���[
	int decLayer = 0;

	//TMX�}�b�v�f�[�^�S��
	auto mapdata = lptmxobj.GetMapData();
	//���߂�ID
	auto firstid = lptmxobj.GetfirstGID();

	//�}�b�v�f�[�^�̕`��
	for (auto layer = --mapdata.end(); layer != --mapdata.begin(); layer--)
	{
		for (int y = 0; y < worldarea.y_; y++)
		{
			for (int x = 0; x < worldarea.x_; x++)
			{
				id = lptmxobj.GetMapData(layer->first, x, y);

				//id��0�̏ꍇ
				if (id == 0)
				{
					continue;
				}

				minlayerid = 5000;

				for (auto layerId : firstid)
				{
					//id-layerid�������Ƃ����������̂�layerid�Ƃ���
					if (id - layerId < minlayerid && 0 <= id - layerId)
					{
						minlayerid = id - layerId;
						decLayer = layerId;
					}
				}
				//ImageMng����0����n�܂��Ă��邽��fistid������
				id = id - decLayer;

				DrawGraph(x * tilesize.x_, y * tilesize.y_, lpImageMng.GetID(decLayer)[id], true);
			}
		}
	}
	isBackGroundLoading_ = true;
}

void GameScene::DrawRouteChange(void)
{
	//���[�g�ύX���̕`��
	if (lpRouteCtl.routeChangeFlag_)
	{
		//�u���b�N�T�C�Y
		auto BlockSize = lpRouteCtl.GetBlockSize();
		//���݂̑I���ꏊ
		auto curPos = lpSceneMng.cursol_->GetPos() - Vector2{ 0, lpSceneMng.GetOffset() } -camera_->GetDrawOffset();

		Vector2 blockPos;
		blockPos.x_ = curPos.x_ / BlockSize * BlockSize;
		blockPos.y_ = curPos.y_ / BlockSize * BlockSize;

		//���[�g�S�̂̎擾
		//auto routemap = lpRouteCtl.GetRouteAll(ArmyType::ALLY);
		//lpRouteCtl.GetRoute

			//���[�g�S�̂̕`��
			//for (auto& route : routemap)
			//{
			//	//����`��
			//	lpRouteCtl.DrawRoute(route.second, Vector2{ BlockSize , BlockSize }, SceneMng::Blue,route.first);
			//}
			//�I�����[�g�̕`��
		lpRouteCtl.DrawRoute(lpRouteCtl.GetCurRoute(), Vector2{ BlockSize , BlockSize }, SceneMng::Blue, lpRouteCtl.currentPlantOrder_);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 178);

		//���݂̍��W�𔒂�
		DrawBox(blockPos.x_, blockPos.y_, blockPos.x_ + BlockSize, blockPos.y_ + BlockSize, 0xffffff, true);
	}

	//���l
	int alpha = 255;
	if (isTutorial_)
	{
		//�`���[�g���A�����̃��l��ς���
		alpha = lpSceneMng.alpha_;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
}

void GameScene::DrawBreakedCore(void)
{
	if (!effectFlag_)
	{
		return;
	}

	expAnimTime_ += lpSceneMng.GetDeltaTime();

	if (!waitEndFlag_)
	{
		if (WAIT_TIME < expAnimTime_)
		{
			waitEndFlag_ = true;
			expAnimTime_ = 0.0;
			PlaySoundMem(lpAudioMng.GetID("Convergence"), DX_PLAYTYPE_BACK);
		}
		return;
	}

	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	Vector2 camPos = camera_->GetDrawOffset();
	camPos += Vector2(0, lpSceneMng.GetOffset());

	if (!convergenceEndFlag_)
	{
		int id = max(static_cast<int>(expAnimTime_ / (CONVERGENCE_TOTAL_ANIM_TIME / (CONVERGENCE_SUM - 1))), 0);
		DrawExtendGraph(corePos_.x_ + BLOCK_SIZE_X / 2 - BLOCK_SIZE_X * 3 + camPos.x_, corePos_.y_ - BLOCK_SIZE_Y * 3 - BLOCK_SIZE_Y / 2 + camPos.y_,
			corePos_.x_ + BLOCK_SIZE_X / 2 + BLOCK_SIZE_X * 3 + camPos.x_, corePos_.y_ + BLOCK_SIZE_Y * 3 + camPos.y_,
			lpImageMng.GetID("Convergence")[id], true);

		if (CONVERGENCE_TOTAL_ANIM_TIME < expAnimTime_)
		{
			convergenceEndFlag_ = true;
			expAnimTime_ = 0.0;
			PlaySoundMem(lpAudioMng.GetID("Explosion"), DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		int id = max(static_cast<int>(expAnimTime_ / (EXP_TOTAL_ANIM_TIME / (EXPLOSION_SUM - 1))), 0);
		DrawExtendGraph(corePos_.x_ + BLOCK_SIZE_X / 2 - BLOCK_SIZE_X * 6 + camPos.x_, corePos_.y_ - BLOCK_SIZE_Y * 6 - BLOCK_SIZE_Y / 2 + camPos.y_,
			corePos_.x_ + BLOCK_SIZE_X / 2 + BLOCK_SIZE_X * 6 + camPos.x_, corePos_.y_ + BLOCK_SIZE_Y * 6 + camPos.y_,
			lpImageMng.GetID("Explosion")[id], true);
		if (EXP_TOTAL_ANIM_TIME < expAnimTime_)
		{
			effectFlag_ = false;
			waitEndFlag_ = false;
			convergenceEndFlag_ = false;
			expAnimTime_ = 0.0;
		}
	}

	//���l
	int alpha = 255;
	if (isTutorial_)
	{
		//�`���[�g���A�����̃��l��ς���
		alpha = lpSceneMng.alpha_;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
}

void GameScene::GenerateUnit(std::list<UniqueObj>::iterator& objitr)
{
	//�u���b�N�T�C�Y
	auto BlockSize = lpRouteCtl.GetBlockSize();

	auto route = (*objitr)->GetRoute();

	//��������
	if ((*objitr)->GetArmyType() == ArmyType::ALLY)
	{
		//���[�g�̍ŏI�ʒu�ɂ���Ό��̈ʒu�֖߂�
		//���]���[�g
		auto Reverseroute = (*objitr)->GetRoute();
		//�����͔�邽�ߏȂ�
		Reverseroute.pop_back();

		std::reverse(Reverseroute.begin(), Reverseroute.end());

		for (auto point = Reverseroute.begin(); point != Reverseroute.end(); point++)
		{
			route.push_back(*point);
		}
	}

	//���j�b�g�̐���
	if ((*objitr)->GetObjType() == ObjType::KNIGHTPLANT)
	{
		objList_.push_back(std::make_unique<Knight>((*objitr)->GetPos(), Vector2{ BlockSize / 4,BlockSize / 4 }, route, (*objitr)->GetColor(), (*objitr)->GetArmyType(), (*objitr)->GetGeneOrder()));
	}
	if ((*objitr)->GetObjType() == ObjType::WARRIORPLANT)
	{
		objList_.push_back(std::make_unique<Warrior>((*objitr)->GetPos(), Vector2{ BlockSize / 4,BlockSize / 4 }, route, (*objitr)->GetColor(), (*objitr)->GetArmyType(), (*objitr)->GetGeneOrder()));
	}
	if ((*objitr)->GetObjType() == ObjType::ARCHERPLANT)
	{
		objList_.push_back(std::make_unique<Archer>((*objitr)->GetPos(), Vector2{ BlockSize / 4,BlockSize / 4 }, route, (*objitr)->GetColor(), (*objitr)->GetArmyType(), (*objitr)->GetGeneOrder()));
	}
}

void GameScene::SetPlantAndCore(void)
{
	//�u���b�N�T�C�Y�̎擾
	auto blocksize = lpRouteCtl.GetBlockSize();

	//�G�����{�݂ƃR�A�̈ʒu�̎擾
	const auto& allyPlantPosVec = lptmxobj.GetAllyPlantPos();
	const auto& enemyPlantPosVec = lptmxobj.GetEnemyPlantPos();
	auto allyCorePos = lptmxobj.GetAllyCorePos();
	auto enemyCorePos = lptmxobj.GetEnemyCorePos();

	//�v�����g�v�[���̍쐬
	//����
	for (auto& allyObjType : allyPlantPosVec)
	{
		if (allyObjType.first == ObjType::KNIGHTPLANT)
		{
			plantAllyPool_.push_back(std::make_unique<KnightPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Blue, ArmyType::ALLY, static_cast<int>(plantAllyPool_.size()) + 1));
		}
		if (allyObjType.first == ObjType::WARRIORPLANT)
		{
			plantAllyPool_.push_back(std::make_unique<WarriorPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Blue, ArmyType::ALLY, static_cast<int>(plantAllyPool_.size()) + 1));
		}
		if (allyObjType.first == ObjType::ARCHERPLANT)
		{
			plantAllyPool_.push_back(std::make_unique<ArcherPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Blue, ArmyType::ALLY, static_cast<int>(plantAllyPool_.size()) + 1));
		}
	}
	//�G
	int cnt = 1;
	for (auto& enemyObjType : enemyPlantPosVec)
	{
		if (enemyObjType.first == ObjType::KNIGHTPLANT)
		{
			plantEnemyPool_.push_back(std::make_unique<KnightPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Red, ArmyType::ENEMY, static_cast<int>(plantEnemyPool_.size()) + 1));
		}
		if (enemyObjType.first == ObjType::WARRIORPLANT)
		{
			plantEnemyPool_.push_back(std::make_unique<WarriorPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Red, ArmyType::ENEMY, static_cast<int>(plantEnemyPool_.size()) + 1));
		}
		if (enemyObjType.first == ObjType::ARCHERPLANT)
		{
			plantEnemyPool_.push_back(std::make_unique<ArcherPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Red, ArmyType::ENEMY, static_cast<int>(plantEnemyPool_.size()) + 1));
		}
	}

	//�莝�����v�[���ɒǉ�
	for (auto deployplant : lpDeployMng.GetDeployPlant())
	{
		for (int i = 0; i < deployplant.second; i++)
		{
			if (deployplant.first == ObjType::KNIGHTPLANT)
			{
				plantAllyPool_.push_back(std::make_unique<KnightPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Blue, ArmyType::ALLY, static_cast<int>(plantAllyPool_.size()) + 1));
			}
			if (deployplant.first == ObjType::WARRIORPLANT)
			{
				plantAllyPool_.push_back(std::make_unique<WarriorPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Blue, ArmyType::ALLY, static_cast<int>(plantAllyPool_.size()) + 1));
			}
			if (deployplant.first == ObjType::ARCHERPLANT)
			{
				plantAllyPool_.push_back(std::make_unique<ArcherPlant>(Vector2{ 0,0 }, Vector2{ 64,64 }, std::vector<Vector2>(), SceneMng::Blue, ArmyType::ALLY, static_cast<int>(plantAllyPool_.size()) + 1));
			}
		}
	}

	//�������[�g�̊i�[�Ǝ{�݂̒ǉ�
	//����
	for (int i = 0; i < static_cast<int>(allyPlantPosVec.size()); i++)
	{
		auto plantPos = allyPlantPosVec[i].second;
		//�v�����g�̐���
		PlantEntryObjList(plantPos * Vector2{ blocksize,blocksize }, lpRouteCtl.GetRoute(ArmyType::ALLY, i + 1), ArmyType::ALLY, allyPlantPosVec[i].first);
		//�V�O�i���̍쐬
		signalList_.push_back(std::make_unique<Signal>(camera_->GetPos(), plantPos * Vector2{ blocksize,blocksize }, camera_->GetDrawOffset(), SignalType::ALLYPLANT));
	}
	//�G
	for (int i = 0; i < static_cast<int>(enemyPlantPosVec.size()); i++)
	{
		auto plantPos = enemyPlantPosVec[i].second;
		//�v�����g�̐���
		PlantEntryObjList(plantPos * Vector2{ blocksize,blocksize }, lpRouteCtl.GetRoute(ArmyType::ENEMY, i + 1), ArmyType::ENEMY, enemyPlantPosVec[i].first);

		//�V�O�i���̍쐬
		signalList_.push_back(std::make_unique<Signal>(camera_->GetPos(), plantPos * Vector2{ blocksize,blocksize }, camera_->GetDrawOffset(), SignalType::ENEMYPLANT));
	}

	//�R�A�̐���
	objList_.push_back(std::make_unique<Core>(allyCorePos * Vector2{ blocksize,blocksize }, Vector2{ blocksize,blocksize }, 0x0000ff, ArmyType::ALLY));
	signalList_.push_back(std::make_unique<Signal>(camera_->GetPos(), allyCorePos * Vector2{ blocksize,blocksize }, camera_->GetDrawOffset(), SignalType::ALLYCOREDISPLAY));
	objList_.push_back(std::make_unique<Core>(enemyCorePos * Vector2{ blocksize,blocksize }, Vector2{ blocksize,blocksize }, 0xff0000, ArmyType::ENEMY));
	signalList_.push_back(std::make_unique<Signal>(camera_->GetPos(), enemyCorePos * Vector2{ blocksize,blocksize }, camera_->GetDrawOffset(), SignalType::ENEMYCOREDISPLAY));
}

void GameScene::PlantEntryObjList(Vector2 pos, std::vector<Vector2> route, ArmyType armytype, ObjType objtype)
{
	//��v������̂�{��
	UniqueObj tmpPlant;
	//�v�[�������v������̂��폜���I�u�W�F���X�g��
	if (armytype == ArmyType::ALLY)
	{
		//����
		for (auto plantItr = plantAllyPool_.begin(); plantItr != plantAllyPool_.end();)
		{
			if ((*plantItr)->GetObjType() == objtype && (*plantItr)->GetArmyType() == armytype)
			{
				tmpPlant = std::move(*plantItr);
				plantItr = plantAllyPool_.erase(plantItr);
				break;
			}
			plantItr++;
		}
	}
	else
	{
		//�G
		for (auto plantItr = plantEnemyPool_.begin(); plantItr != plantEnemyPool_.end();)
		{
			if ((*plantItr)->GetObjType() == objtype && (*plantItr)->GetArmyType() == armytype)
			{
				tmpPlant = std::move(*plantItr);
				plantItr = plantEnemyPool_.erase(plantItr);
				break;
			}
			plantItr++;
		}
	}

	//�ԍ��𐶐����ɒ���
	int cnt = 1;
	//���������J�E���g
	for (auto& obj : objList_)
	{
		if (obj->GetGenerateType() == GenerateType::PLANT && obj->GetArmyType() == armytype)
		{
			cnt++;
		}
	}

	tmpPlant->SetGeneOrder(cnt);

	//��v�������̂֍��W�ƃ��[�g����
	tmpPlant->InitPlantGenerationFromPool(pos, route);

	objList_.push_front(std::move(tmpPlant));
}

void GameScene::DeployPlant(void)
{
	PlaySoundMem(lpAudioMng.GetID("DeployPlant"), DX_PLAYTYPE_BACK);

	//�v�����g��z�u
	//�z�u�t���O���I�t��
	lpDeployMng.SetEntryFlag(false);

	//���[�u�I�u�W�F�N�g���폜
	delete deployPlantFunc_;
	deployPlantFunc_ = nullptr;

	//�e���W�ƃT�C�Y�̎擾
	auto blocksize = lpRouteCtl.GetBlockSize();
	auto entryPlantPos = lpDeployMng.GetEntryPlantPos();
	CreateSmokeWhenDeployingPlant(entryPlantPos);

	//A*�Ń��[�g�����߂�
	std::vector<Vector2> tmproute = lpRouteCtl.CreateRoute(entryPlantPos / blocksize, lptmxobj.GetEnemyCorePos(), SceneMng::Blue);

	//�v�����g���G���g���[
	PlantEntryObjList(entryPlantPos, tmproute, ArmyType::ALLY, lpDeployMng.GetPlantType());
}

void GameScene::CreateSmokeWhenDeployingPlant(Vector2 plantPos)
{
	auto blockSize = lpRouteCtl.GetBlockSize();
	for (int i = 0; i < TOTAL_SMOKE; i++)
	{
		smokeVArray_.push_back(new Smoke(Vector2{ plantPos.x_ + GetRand(blockSize), plantPos.y_ + GetRand(blockSize) }, false));
	}
}

void GameScene::CreateUI(void)
{
	//�{���\��
	Vector2 uiSize = { UI_SIZE_X, UI_SIZE_Y };
	btnList_.push_back(std::make_unique<Box>(
		Vector2{ lpSceneMng.GetViewArea().x_ - uiSize.x_ * 3 - UI_OFFSET_X * 2,UI_OFFSET_Y },
		std::move(uiSize),
		std::bind(&GameScene::CntUpDoubleElapTime, this),
		MouseInputID::LEFT,
		"x"));
	btnList_.back()->SetModelID(lpImageMng.GetID("woodframe04")[0]);

	//���[�g�ύX�\��
	btnList_.push_back(std::make_unique<Box>(
		Vector2{ lpSceneMng.GetViewArea().x_ - uiSize.x_ * 2 - UI_OFFSET_X,UI_OFFSET_Y },
		std::move(Vector2{ uiSize.x_ * 2 , uiSize.y_ }),
		std::bind(&GameScene::RouteChange, this),
		MouseInputID::LEFT,
		"���[�g�ύX"));
	btnList_.back()->SetModelID(lpImageMng.GetID("woodframe04")[0]);
}

bool GameScene::CntUpDoubleElapTime(void)
{
	//�J�E���g�A�b�v
	lpSceneMng.doubleValue_ += MAG_GOUP_ONCE;
	if (lpSceneMng.doubleValue_ > MAX_MAG)
	{
		//�����l�ɖ߂�
		lpSceneMng.doubleValue_ = 1.0;
	}
	return true;
}

bool GameScene::CntDownDoubleElapTime(void)
{
	//�J�E���g�_�E��
	lpSceneMng.doubleValue_ -= MAG_GOUP_ONCE;
	if (lpSceneMng.doubleValue_ < 1.0)
	{
		//�ő��
		lpSceneMng.doubleValue_ = MAX_MAG;
	}
	return true;
}

bool GameScene::RouteChange(void)
{
	//���[�g�ύX�t���O���I���ɂ���
	lpRouteCtl.routeChangeFlag_ = true;
	return true;
}

void GameScene::InitFromTutorial(void)
{
	//�`���[�g���A���֌W������
	isTutorial_ = false;
	lpInputString.SetNowPlace(PLACE::MAX);

	//���y���쐬���Ă��ꍇ�~�߂�
	lpAudioMng.StopMusicAll();

	//���y�Đ�
	SetVolumeMusicMem(GAME_BGM_VOLUME, lpAudioMng.GetID(musicName_));
	PlaySoundMem(lpAudioMng.GetID(musicName_), DX_PLAYTYPE_LOOP);

	//�J�����̏�����
	camera_->InitArea();
}

void GameScene::InitMusic(void)
{
	//���y���쐬���Ă��ꍇ�~�߂�
	lpAudioMng.StopMusicAll();

	//���y�Đ�
	if (isTutorial_)
	{
		SetVolumeMusicMem(TUTORIAL_BGM_VOLUME, lpAudioMng.GetID(musicName_));
	}
	else
	{
		//���y�Đ�
		SetVolumeMusicMem(GAME_BGM_VOLUME, lpAudioMng.GetID(musicName_));
	}
	PlaySoundMem(lpAudioMng.GetID(musicName_), DX_PLAYTYPE_LOOP);
}
