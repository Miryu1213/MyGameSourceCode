#include <DxLib.h>
#include <cmath>
#include <memory>
#include "TipsScene.h"
#include "CountDownScene.h"
#include "GameScene.h"
#include "TutorialScene.h"
#include "Transition/FadeInOut.h"
#include "Transition/FadeOut.h"
#include "Obj/RouteCtl.h"
#include "tmx/TmxObj.h"
#include "tmx/MapObject.h"
#include "../common/ImageMng.h"
#include "../common/AudioMng.h"
#include "../common/FontMng.h"

//�uTIPS�v
constexpr int TIPS_OFFSET_Y = 220;

//TIPS�̓��e
constexpr int CONTENT_TIPS_OFFSET_Y = 340;
constexpr int CONTENT_TIPS_SIZE_Y = 70;

//�u�\�v
constexpr int DASH_OFFSET_Y = 10;

//�����e�L�X�g
constexpr int TEXT_NOWLOADING_OFFSET_X = 304;
constexpr int TEXT_NOWLOADING_OFFSET_Y = 46;

constexpr int DOT_OFFSET_X = 50;
constexpr int DOT_OFFSET_Y = 22;

constexpr int DOT_SIZE_X = 10;

//��̓_�̕\������
constexpr double ONE_DOT_TIME = 0.8;
//�\���|�C���g��
constexpr int ONE_DOT_SUM = 3;

//���s������
constexpr size_t NEWLINE_NUMBER = 19;

//�t�H���g
constexpr int TIPS_FONT_SIZE = 40;
constexpr int CONTENT_TIPS_FONT_SIZE = 26;

constexpr double BACKLIGHT_TIME = 0.1;
constexpr int BACKLIGHT_SUM = 40;

constexpr double CLICK_LIGHT_TIME = 0.04;
constexpr int CLICK_LIGHT_SUM = 14;

constexpr int CLICK_LIGHT_SIZE_X = 300;
constexpr int CLICK_LIGHT_SIZE_Y = 180;

//�t�F�[�h����
constexpr double FADE_TIME = 0.6;

TipsScene::TipsScene(std::string mapName)
{
	mapName_ = mapName;

	//���������ɕϊ�
	mapName.erase(0, mapName.find_first_of("p") + 1);
	mapName.erase(mapName.find_first_of("."), mapName.size());

	mapNumber_ = atoi(mapName.c_str());

	Init();
}

TipsScene::~TipsScene()
{
	if (routeThread_.joinable())
	{
		routeThread_.join();
	}
	
	lpImageMng.DeleteID("NowLoading");
	lpImageMng.DeleteID("PushTheScreen");
	lpImageMng.DeleteID("dot");
	lpImageMng.DeleteID("BackLight");
	lpImageMng.DeleteID("OneBackLight");

	lpAudioMng.DeleteID("ClickStar");
	lpAudioMng.DeleteID("EndLoading");
}

bool TipsScene::Init(void)
{
	//�����o������
	loadingTime_ = 0.0;
	backLightTime_ = 0.0;
	clickLightTime_ = 0.0;

	alpha_ = 0.0;
	IsFade_ = false;

	endLoadingFlag_ = false;
	oneRingFlag_ = false;
	clickLightFlag_ = false;

	//�������
	curScreen_ = CURSCREEN::START;

	//���y���쐬���Ă��ꍇ�~�߂�
	lpAudioMng.StopMusicAll();

	//���[�g�N���X�̍쐬
	lpRouteCtl.Create();

	//TMX���[�h
	lptmxobj.LoadTMX(mapName_);

	auto blocksize = lpRouteCtl.GetBlockSize();
	auto tilesize = lptmxobj.GettileSize();
	auto worldarea = lptmxobj.GetworldArea();

	//�}�b�v�̏�����
	lpRouteCtl.InitMap(worldarea * tilesize / blocksize);

	CreateCostMap();

	//�ʃX���b�h�Ń��[�g�̍쐬
	routeThread_ = std::thread([this] { InitByThread(); });

	lpAudioMng.GetID("Resource/Audio/SE/Unit/AttackedCore.mp3", "AttackedCore");
	lpAudioMng.GetID("Resource/Audio/SE/Unit/AttackedPlant.mp3", "AttackedPlant");
	lpAudioMng.GetID("Resource/Audio/SE/Unit/BrokenCore.mp3", "BrokenCore");
	lpAudioMng.GetID("Resource/Audio/SE/Unit/BrokenPlant.mp3", "BrokenPlant");

	lpAudioMng.GetID("Resource/Audio/SE/ClickStar.mp3", "ClickStar");
	lpAudioMng.GetID("Resource/Audio/SE/EndLoading.mp3", "EndLoading");

	return true;
}

UniqueScene TipsScene::UpDate(UniqueScene ownScene)
{
	//�J�[�\���̍X�V
	lpSceneMng.cursol_->Update();

	if (endLoadingFlag_)
	{
		if (!oneRingFlag_)
		{
			PlaySoundMem(lpAudioMng.GetID("EndLoading"), DX_PLAYTYPE_BACK);
			oneRingFlag_ = true;
		}

		if (lpSceneMng.cursol_->GetMouseInput(InputType::NOW)[MouseInputID::LEFT])
		{
			if (!clickLightFlag_)
			{
				PlaySoundMem(lpAudioMng.GetID("ClickStar"), DX_PLAYTYPE_BACK);
				clickLightFlag_ = true;
			}
		}
	}

	//NOWLOADING
	loadingTime_ += lpSceneMng.GetDeltaTime();
	if (ONE_DOT_TIME * (ONE_DOT_SUM + 1) < loadingTime_)
	{
		loadingTime_ = 0.0;
	}

	//�o�b�N���C�g
	backLightTime_ += lpSceneMng.GetDeltaTime();
	if (BACKLIGHT_TIME * BACKLIGHT_SUM < backLightTime_)
	{
		backLightTime_ = 0.0;
	}

	//�N���b�N���C�g
	if (clickLightFlag_)
	{
		clickLightTime_ += lpSceneMng.GetDeltaTime();
		if (CLICK_LIGHT_TIME * CLICK_LIGHT_SUM < clickLightTime_)
		{
			//�Q�[���V�[����
			if (clickLightFlag_)
			{
				nextScene_->InitMusic();
				return std::make_unique<FadeInOut>(std::move(ownScene), std::move(nextScene_), FADE_TIME);
			}
		}
	}

	return ownScene;
}

void TipsScene::Draw(void)
{
	//�o�C�g���̓s����2�{����
	int newLineNum = NEWLINE_NUMBER * 2;
	//�uTIPS�v�̕`��
	auto tipsHandle = lpFontMng.GetFontSizeHandle(FontType::DEFAULT, TIPS_FONT_SIZE);
	int tipsWidth = GetDrawStringWidthToHandle("TIPS", static_cast<int>(strlen("TIPS")), tipsHandle);
	DrawFormatStringToHandle((lpSceneMng.GetViewArea().x_ - tipsWidth) / 2, TIPS_OFFSET_Y, 0xffffff, tipsHandle, "TIPS");

	std::string tipsText = lpTips.GetTip(mapNumber_);
	
	std::vector<std::string> tipsVArray;
	//�`�b�v�X�̉��s�p�؂���
	for (;;)
	{
		if (tipsText.size() < newLineNum)
		{
			tipsVArray.push_back(tipsText);
			break;
		}
		//NEWLINE_NUMBER���؂���
		tipsVArray.push_back(tipsText.substr(0, min(newLineNum, tipsText.size())));
		tipsText.erase(0, min(newLineNum, tipsText.size()));
	}

	//TIPS���e�̕`��
	auto tipsTextHandle = lpFontMng.GetFontSizeHandle(FontType::DEFAULT, CONTENT_TIPS_FONT_SIZE);

	//�����̎擾
	int height = 0;
	GetDrawFormatStringSizeToHandle(nullptr, &height, nullptr, tipsTextHandle, tipsVArray.front().c_str());

	std::string dash;
	for (int i = 0; i < newLineNum * 2 / 3; i++)
	{
		//�\�̍쐬
		dash += "�\";
	}
	int dashWidth = GetDrawStringWidthToHandle(dash.c_str(), static_cast<int>(strlen(dash.c_str())), tipsTextHandle);

	for (int i = 0; i < static_cast<int>(tipsVArray.size()); i++)
	{
		DrawFormatStringToHandle((lpSceneMng.GetViewArea().x_ - dashWidth) / 2,
			CONTENT_TIPS_OFFSET_Y + CONTENT_TIPS_SIZE_Y * i - CONTENT_TIPS_SIZE_Y / 2,
			0xffffff, tipsTextHandle, dash.c_str());

		int tipsTextWidth = GetDrawStringWidthToHandle(tipsVArray[i].c_str(), static_cast<int>(strlen(tipsVArray[i].c_str())), tipsTextHandle);
		DrawFormatStringToHandle((lpSceneMng.GetViewArea().x_ - tipsTextWidth) / 2, CONTENT_TIPS_OFFSET_Y + CONTENT_TIPS_SIZE_Y * i, 0xffffff, tipsTextHandle, tipsVArray[i].c_str());
	}

	DrawFormatStringToHandle((lpSceneMng.GetViewArea().x_ - dashWidth) / 2,
		CONTENT_TIPS_OFFSET_Y + CONTENT_TIPS_SIZE_Y * static_cast<int>(tipsVArray.size()) - CONTENT_TIPS_SIZE_Y / 2,
		0xffffff, tipsTextHandle, dash.c_str());
	
	if (!endLoadingFlag_)
	{
		//NOWLOADING
		DrawGraph(lpSceneMng.GetViewArea().x_ - TEXT_NOWLOADING_OFFSET_X, lpSceneMng.GetViewArea().y_ - TEXT_NOWLOADING_OFFSET_Y, lpImageMng.GetID("NowLoading")[0], true);
		for (int i = 0; i < ONE_DOT_SUM; i++)
		{
			if (ONE_DOT_TIME * (i + 1) < loadingTime_)
			{
				DrawGraph(lpSceneMng.GetViewArea().x_ - DOT_OFFSET_X + DOT_SIZE_X * i, lpSceneMng.GetViewArea().y_ - DOT_OFFSET_Y, lpImageMng.GetID("dot")[0], true);
			}
		}
	}
	else
	{
		//�o�b�N���C�g
		SetDrawBlendMode(DX_BLENDMODE_ADD, 180);
		int extendRate = 2;
		DrawExtendGraph(lpSceneMng.GetViewArea().x_ - TEXT_NOWLOADING_OFFSET_X * extendRate, lpSceneMng.GetViewArea().y_ - TEXT_NOWLOADING_OFFSET_Y * extendRate,
			lpSceneMng.GetViewArea().x_ + TEXT_NOWLOADING_OFFSET_X , lpSceneMng.GetViewArea().y_ + TEXT_NOWLOADING_OFFSET_Y,
			lpImageMng.GetID("BackLight")[static_cast<int>(backLightTime_ / BACKLIGHT_TIME)], true);

		if (IsFade_)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_));
		}
		else
		{
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}

		//PUSHTHESCREEN
		DrawGraph(lpSceneMng.GetViewArea().x_ - TEXT_NOWLOADING_OFFSET_X, lpSceneMng.GetViewArea().y_ - TEXT_NOWLOADING_OFFSET_Y, lpImageMng.GetID("PushTheScreen")[0], true);
	}

	//�J�[�\���̕`��
	lpSceneMng.cursol_->Draw(false);

	//�N���b�N���C�g
	SetDrawBlendMode(DX_BLENDMODE_ADD, 180);
	auto curPos = lpSceneMng.cursol_->GetPos();
	DrawExtendGraph(curPos.x_ - CLICK_LIGHT_SIZE_X / 2, curPos.y_ - CLICK_LIGHT_SIZE_Y / 2,
		curPos.x_ + CLICK_LIGHT_SIZE_X / 2, curPos.y_ + CLICK_LIGHT_SIZE_Y / 2,
		lpImageMng.GetID("OneBackLight")[static_cast<int>(clickLightTime_ / CLICK_LIGHT_TIME)], true);
	if (IsFade_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_));
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}

void TipsScene::SetCantMovePoint(void)
{
	//�}�b�v�l�[����map+�����ɉ��H
	std::string tmpMapName = mapName_;
	tmpMapName.erase(0, tmpMapName.find_last_of("/") + 1);
	tmpMapName.erase(tmpMapName.find_last_of("."), static_cast<int>(tmpMapName.size()));
	//�}�b�v����ړ��s�̈�ID�̐ݒ�
	for (auto& dontMovePoint : MapObject::GetMapObject(tmpMapName).dontMoveList_)
	{
		lpRouteCtl.cantMoveTileId_.push_back(dontMovePoint);
	}
}

void TipsScene::CreateCostMap(void)
{
	//�ړ��s�|�C���g�̍쐬
	SetCantMovePoint();

	auto blocksize = lpRouteCtl.GetBlockSize();
	auto tilesize = lptmxobj.GettileSize();
	auto worldarea = lptmxobj.GetworldArea();

	//�ő�u���b�N���̐ݒ�
	Vector2 limitNumBlock_ = {
		worldarea.x_ * tilesize.x_ / blocksize,
		worldarea.y_ * tilesize.y_ / blocksize };

	//���݂̃��C���[ID
	int id = 0;
	//id - layerId�̍ŏ��i���o�[
	int minlayerid = 5000;
	//���背�C���[
	int decLayer = 0;

	//�}�b�v�̏�����
	lpRouteCtl.InitMap(limitNumBlock_);

	//TMX�}�b�v�f�[�^�S��
	auto mapdata = lptmxobj.GetMapData();
	//���߂�ID
	auto firstid = lptmxobj.GetfirstGID();

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

				//�ړ��}�b�v�̍쐬
				//�ꎞ�I��
				lpRouteCtl.CreateCostMap(Vector2(x, y), decLayer, blocksize, tilesize);
			}
		}
	}
}

void TipsScene::InitByThread(void)
{
	InitRoute();

	//���̃V�[���̍쐬
	if (mapName_ == "Scene/tmx/map0.tmx")
	{
		nextScene_ = std::make_unique<TutorialScene>(mapName_.c_str(), std::make_unique<GameScene>(mapName_.c_str(), true));
		//���y���쐬���Ă��ꍇ�~�߂�
		lpAudioMng.StopMusicAll();
	}
	else
	{
		nextScene_ = std::make_unique<CountDownScene>(std::make_unique<GameScene>(mapName_.c_str(), false));
	}

	//���[�h�I��
	endLoadingFlag_ = true;
}

void TipsScene::InitRoute(void)
{
	//�u���b�N�T�C�Y�̎擾
	auto blocksize = lpRouteCtl.GetBlockSize();

	//�G�����{�݂ƃR�A�̈ʒu�̎擾
	const auto& allyPlantPosVec = lptmxobj.GetAllyPlantPos();
	const auto& enemyPlantPosVec = lptmxobj.GetEnemyPlantPos();
	auto allyCorePos = lptmxobj.GetAllyCorePos();
	auto enemyCorePos = lptmxobj.GetEnemyCorePos();

	//�������[�g�̊i�[�Ǝ{�݂̒ǉ�
	//����
	for (auto allyPlantPos : allyPlantPosVec)
	{
		//�{�ݐ����� ���̒l���i�[���邽��+1
		int orderNum = lpRouteCtl.GetRouteSum(ArmyType::ALLY) + 1;
		const auto route = lpRouteCtl.CreateRoute(allyPlantPos.second, enemyCorePos, SceneMng::Blue);

		//���[�g�̐���
		lpRouteCtl.SetRoute(ArmyType::ALLY, orderNum, route);
	}
	//�G
	for (auto enemyPlantPos : enemyPlantPosVec)
	{
		//�{�ݐ����� ���̒l���i�[���邽��+1
		auto orderNum = lpRouteCtl.GetRouteSum(ArmyType::ENEMY) + 1;

		//���[�g�̐���	
		lpRouteCtl.SetRoute(ArmyType::ENEMY, orderNum, lpRouteCtl.CreateRoute(enemyPlantPos.second, allyCorePos, SceneMng::Blue));
	}
}
