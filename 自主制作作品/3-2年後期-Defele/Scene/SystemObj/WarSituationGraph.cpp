#include "WarSituationGraph.h"
#include "InputString.h"
#include "Camera.h"
#include "../SceneMng.h"
#include "../Obj/RouteCtl.h"
#include "../Obj/SearchMapCtl.h"
#include "../tmx/TmxObj.h" 

WarSituationGraph* WarSituationGraph::s_Instance = nullptr;

//�틵�I�t�Z�b�g
constexpr int WAR_OFFSET_X = 10;
constexpr int WAR_OFFSET_Y = 38;

constexpr int WAR_SIZE_Y = 20;

//�틵���S�T�C�Y
constexpr int WAR_SITUATION_LOGO_SIZE_X = 92;
constexpr int WAR_SITUATION_LOGO_SIZE_Y = 46;

//�G�������S�T�C�Y
constexpr int TEXT_SIZE_X = 72;
constexpr int TEXT_SIZE_Y = 36;

constexpr int TEXT_OFFSET_Y = 34;

//�틵�F
constexpr float DEF_COLOR = 150.0f;
//�{��
constexpr int COLOR_RATE = 25;

//�L�����̃|�C���g��
constexpr int POINT_DIFFRENCE_ADVANTAGE = 20;
//�L�����̉��o�ꖇ�̕\������
constexpr double ONE_DIR_TIME = 0.16;

//�\���摜�̑���
constexpr int BLINKLIGHT_SUM = 30;

constexpr int BLINKLIGHT_SIZE_X = 70;
constexpr int BLINKLIGHT_SIZE_Y = 30;

void WarSituationGraph::SetArmyMap(int blockX, int blockY, UINT32 color)
{
	armyMap_[blockY][blockX] = color;
}

void WarSituationGraph::InitMap(void)
{
	//���T�C�Y
	armyMap_.resize(limitNumBlock_.y_);
	for (int y = 0; y < limitNumBlock_.y_; y++)
	{
		armyMap_[y].resize(limitNumBlock_.x_);
	}

	//�����͍�
	for (int y = 0; y < limitNumBlock_.y_; y++)
	{
		for (int x = 0; x < limitNumBlock_.x_; x++)
		{
			armyMap_[y][x] = SceneMng::Black;
		}
	}
}

void WarSituationGraph::AddInitSquares(int coreRange)
{
	//�����̒n�쐬(����)
	for (auto& searchpoint : SearchMapCtl::GetSearchMovePointSum(lptmxobj.GetAllyCorePos(), coreRange, limitNumBlock_, lpRouteCtl.costMap_))
	{
		armyMap_[searchpoint.y_][searchpoint.x_] = SceneMng::Blue;
	}
	//�����̒n�쐬(�G)
	for (auto& searchpoint : SearchMapCtl::GetSearchMovePointSum(lptmxobj.GetEnemyCorePos(), coreRange, limitNumBlock_, lpRouteCtl.costMap_))
	{
		armyMap_[searchpoint.y_][searchpoint.x_] = SceneMng::Red;
	}
}

void WarSituationGraph::Draw(Vector2 camOffset)
{
	//�u���b�N�T�C�Y
	auto BlockSize = lpRouteCtl.GetBlockSize();
	
	//�L���̒n�̃J�E���g
	int validCnt = 0;
	//�����̒n�J�E���g
	int allyCnt = 0;
	//�G�̒n�J�E���g
	int enemyCnt = 0;

	//�틵�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
	for (int y = 0; y < limitNumBlock_.y_; y++)
	{
		for (int x = 0; x < limitNumBlock_.x_; x++)
		{
			if (lpRouteCtl.costMap_[y][x] != lpRouteCtl.GetCantMovePoint())
			{
				//�L���̒n���J�E���g
				validCnt++;
			}

			if (armyMap_[y][x] == SceneMng::Black)
			{
				//�����̒n�͏Ȃ�
				continue;
			}

			if (armyMap_[y][x] == SceneMng::Blue)
			{
				//�����J�E���g
				allyCnt++;
			}

			if (armyMap_[y][x] == SceneMng::Red)
			{
				//�G�J�E���g
				enemyCnt++;
			}

			//�S�̐틵�̐F�u���b�N�\��
			DrawBox(x * BlockSize + camOffset.x_, y * BlockSize + camOffset.y_ + lpSceneMng.GetOffset(),
				x * BlockSize + BlockSize + camOffset.x_, y * BlockSize + BlockSize + camOffset.y_ + lpSceneMng.GetOffset(),
				armyMap_[y][x], true);
		}
	}

	//���l
	int alpha = 255;

	//���݂̃`���[�g���A���������ȊO���Ɣ�������
	if (lpInputString.GetNowPlace() != PLACE::WARSITUATION && lpInputString.GetNowPlace() != PLACE::MAX)
	{
		alpha = lpSceneMng.alpha_;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	//��̐틵�\��
	float allyRate = static_cast<float>((lpSceneMng.GetViewArea().x_ / 2) / validCnt)* allyCnt;
	float enemyRate = static_cast<float>((lpSceneMng.GetViewArea().x_ / 2) / validCnt)* enemyCnt;

	float blue = min(DEF_COLOR + (allyCnt / max(enemyCnt,1) * COLOR_RATE) , 255.0f);
	float red = min(DEF_COLOR + (enemyCnt / max(allyCnt, 1) * COLOR_RATE), 255.0f);

	DrawBox(WAR_OFFSET_X, WAR_OFFSET_Y,
		lpSceneMng.GetViewArea().x_ - WAR_OFFSET_X, WAR_OFFSET_Y + WAR_SIZE_Y,
		GetColor(static_cast<int>(red), 20, 20), true);
	DrawBoxAA(WAR_OFFSET_X, WAR_OFFSET_Y,
		static_cast<float>(lpSceneMng.GetViewArea().x_ / 2) + allyRate - enemyRate, WAR_OFFSET_Y + WAR_SIZE_Y,
		GetColor(20, 20, static_cast<int>(blue)), true);

	//�L�����̌�
	loadingTime_ += lpSceneMng.GetDeltaTime();
	if (ONE_DIR_TIME < loadingTime_)
	{
		animId_++;
		if (BLINKLIGHT_SUM <= animId_)
		{
			animId_ = 0;
		}
		loadingTime_ = 0.0;
	}

	//�u���R�v
	DrawGraph(0, TEXT_OFFSET_Y, lpImageMng.GetID("AllyText")[0], true);
	if (enemyCnt + POINT_DIFFRENCE_ADVANTAGE < allyCnt)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 205);
		DrawExtendGraph(0, TEXT_OFFSET_Y,
			BLINKLIGHT_SIZE_X, TEXT_OFFSET_Y + BLINKLIGHT_SIZE_Y,
			lpImageMng.GetID("BlinkLight")[animId_], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	//�u�G�R�v
	DrawGraph(lpSceneMng.GetViewArea().x_ - TEXT_SIZE_X, TEXT_OFFSET_Y,lpImageMng.GetID("EnemyText")[0], true);
	if (allyCnt + POINT_DIFFRENCE_ADVANTAGE < enemyCnt)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 205);
		DrawExtendGraph(lpSceneMng.GetViewArea().x_ - TEXT_SIZE_X - 15, TEXT_OFFSET_Y,
			lpSceneMng.GetViewArea().x_ - TEXT_SIZE_X + BLINKLIGHT_SIZE_X, TEXT_OFFSET_Y + BLINKLIGHT_SIZE_Y,
			lpImageMng.GetID("BlinkLight")[animId_], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	//�u�틵�v
	DrawExtendGraph((lpSceneMng.GetViewArea().x_ - WAR_SITUATION_LOGO_SIZE_X) / 2, 0,
		(lpSceneMng.GetViewArea().x_ - WAR_SITUATION_LOGO_SIZE_X) / 2 + WAR_SITUATION_LOGO_SIZE_X, WAR_SITUATION_LOGO_SIZE_Y,
		lpImageMng.GetID("WarSituation")[0], true);
	
	//���łɔ������Ă��邽�߂��Ƃɖ߂�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

WarSituationGraph::WarSituationGraph(Vector2 limitNumBlock)
{
	limitNumBlock_ = limitNumBlock;
	loadingTime_ = 0.0;
	animId_ = 0;

	InitMap();
}

WarSituationGraph::~WarSituationGraph()
{
	for (int i = 0; i < static_cast<int>(armyMap_.size()); i++)
	{
		armyMap_[i].clear();
	}
	armyMap_.clear();
}
