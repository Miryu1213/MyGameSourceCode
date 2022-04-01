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

	//�Q�[���V�[���̃J�������`���[�g���A���̃J�����ɂ���
	nextScene_->SetCamera(camera_);

	Init();
}

TutorialScene::~TutorialScene()
{
}

bool TutorialScene::Init(void)
{
	//���b�Z�[�W�̋L��

	//�`���[�g���A���\����
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
		//�v���C�X���i�[
		stringMap_.try_emplace(place);
	}

	//���ꂼ��̃v���C�X�ɕ�������i�[
	stringMap_[PLACE::FIRST] = "Welcome To My Game !,�����ł̓`���[�g���A�����s���܂�";
	stringMap_[PLACE::PLANT] = "�{�݂̓��j�b�g�̐������s���܂�";
	stringMap_[PLACE::UNIT] = "���j�b�g�͎{�݂ɗ^����ꂽ|���[�g��𓮂��܂�,�G�⑊��̎{�݂��߂��ɂ���ƍU�����܂�";
	stringMap_[PLACE::CORE] = "�R�A�͔j�󂷂�Ώ����A|�j�󂳂��Δs�k�ł�";
	stringMap_[PLACE::BUTTON] = "�㕔�{�^���Ŏ{�݂̃��[�g�ύX���\�ł�,x1�Ŕ{���̐؂�ւ����ł��܂�";
	stringMap_[PLACE::WARSITUATION] = "�㕔�o�[�Ō��݂̐틵���m�F�ł��܂�";
	stringMap_[PLACE::DEPLOY] = "���̊ۂ���{�݂̔z�u|�M�~�b�N�̔������\�ł�";
	stringMap_[PLACE::ENEMYPLANT] = "����̑���͓��ɂ���悤�ł�";
	stringMap_[PLACE::MAX] = "Have a Good Time !";

	//���݂̕�����
	std::string nowString = stringMap_[orderList_.front()];

	//,��������ƕ�������
	int laststr =
		static_cast<int>(nowString.find_first_of(",")) != nowString.npos ?
		static_cast<int>(nowString.find_first_of(",")) : static_cast<int>(nowString.size());

	if (static_cast<int>(nowString.size()) != laststr)
	{
		//��������ꍇ
		nowString.erase(laststr, static_cast<int>(nowString.size()));

		//���݂̕�����܂ō폜
		stringMap_[orderList_.front()].erase(0, laststr + 1);

		//���̃��b�Z�[�W��
		lpInputString.CreateString(orderList_.front(), nowString, true);
	}
	else
	{
		//���݂̕����񂪍Ō�܂Ői��ł���ꍇ

	    //���̃��b�Z�[�W��
		lpInputString.CreateString(orderList_.front(), nowString, true);

		//�擪���폜
		orderList_.pop_front();
	}

	return true;
}

void TutorialScene::InitMusic(void)
{
	//���y���쐬���Ă��ꍇ�~�߂�
	lpAudioMng.StopMusicAll();
	//���y�Đ�
	PlaySoundMem(lpAudioMng.GetID(nextScene_->GetMusicName()), DX_PLAYTYPE_LOOP);
	SetVolumeMusicMem(TUTORIAL_BGM_VOLUME, lpAudioMng.GetID(nextScene_->GetMusicName()));
}

UniqueScene TutorialScene::UpDate(UniqueScene ownScene)
{
	//�J�[�\���̍X�V
	lpSceneMng.cursol_->Update();

	//�`���[�g���A���p�̃J�����̍X�V
	camera_->UpDateTutorial();

	//�}�E�X�̒l�̎擾
	auto mouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::NOW);
	auto oldmouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::OLD);

	if (mouseInput[MouseInputID::LEFT] && !oldmouseInput[MouseInputID::LEFT])
	{
		if (!orderList_.empty())
		{
			//���݂̕�����
			auto nowString = stringMap_[orderList_.front()];

			//,��������ƕ�������
			int laststr =
				static_cast<int>(nowString.find_first_of(",")) != nowString.npos ?
				static_cast<int>(nowString.find_first_of(",")) : static_cast<int>(nowString.size());

			if (static_cast<int>(nowString.size()) != laststr)
			{
				//��������ꍇ
				nowString.erase(laststr, static_cast<int>(nowString.size()));

				//���݂̕�����܂ō폜
				stringMap_[orderList_.front()].erase(0, laststr + 1);

				//���̃��b�Z�[�W��
				lpInputString.CreateString(orderList_.front(), nowString, true);
			}
			else
			{
				//���݂̕����񂪍Ō�܂Ői��ł���ꍇ

				//���̃��b�Z�[�W��
				lpInputString.CreateString(orderList_.front(), nowString, true);

				//�擪���폜
				orderList_.pop_front();
			}
		}
		else
		{
			//���̃��b�Z�[�W���Ȃ��ꍇ
			lpInputString.CreateString(PLACE::MAX, "", true);

			//�Q�[���V�[���̏���������
			nextScene_->InitFromTutorial();

			//�Q�[���V�[����
			return std::make_unique<CountDownScene>(std::move(nextScene_));
		}
	}

	//�\�����Ă�����̂��莝���̂Ƃ�
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
		//��ʂ𔖂�
		lpSceneMng.alpha_ = 128;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, lpSceneMng.alpha_);
	}
	
	nextScene_->Draw();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//������i�[��
	if (lpInputString.GetString() != "")
	{
		//�}�E�X�̒l�̎擾
		auto mouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::NOW);
		auto oldmouseInput = lpSceneMng.cursol_->GetMouseInput(InputType::OLD);

		//�}�E�X���N���b�N�ŏ���
		lpInputString.DissapperInputDraw();
	}
}