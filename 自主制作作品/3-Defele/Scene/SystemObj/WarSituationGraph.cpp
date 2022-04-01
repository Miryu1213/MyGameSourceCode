#include "WarSituationGraph.h"
#include "InputString.h"
#include "Camera.h"
#include "../SceneMng.h"
#include "../Obj/RouteCtl.h"
#include "../Obj/SearchMapCtl.h"
#include "../tmx/TmxObj.h" 

WarSituationGraph* WarSituationGraph::s_Instance = nullptr;

//戦況オフセット
constexpr int WAR_OFFSET_X = 10;
constexpr int WAR_OFFSET_Y = 38;

constexpr int WAR_SIZE_Y = 20;

//戦況ロゴサイズ
constexpr int WAR_SITUATION_LOGO_SIZE_X = 92;
constexpr int WAR_SITUATION_LOGO_SIZE_Y = 46;

//敵味方ロゴサイズ
constexpr int TEXT_SIZE_X = 72;
constexpr int TEXT_SIZE_Y = 36;

constexpr int TEXT_OFFSET_Y = 34;

//戦況色
constexpr float DEF_COLOR = 150.0f;
//倍率
constexpr int COLOR_RATE = 25;

//有利時のポイント差
constexpr int POINT_DIFFRENCE_ADVANTAGE = 20;
//有利時の演出一枚の表示時間
constexpr double ONE_DIR_TIME = 0.16;

//表示画像の総数
constexpr int BLINKLIGHT_SUM = 30;

constexpr int BLINKLIGHT_SIZE_X = 70;
constexpr int BLINKLIGHT_SIZE_Y = 30;

void WarSituationGraph::SetArmyMap(int blockX, int blockY, UINT32 color)
{
	armyMap_[blockY][blockX] = color;
}

void WarSituationGraph::InitMap(void)
{
	//リサイズ
	armyMap_.resize(limitNumBlock_.y_);
	for (int y = 0; y < limitNumBlock_.y_; y++)
	{
		armyMap_[y].resize(limitNumBlock_.x_);
	}

	//初期は黒
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
	//初期領地作成(味方)
	for (auto& searchpoint : SearchMapCtl::GetSearchMovePointSum(lptmxobj.GetAllyCorePos(), coreRange, limitNumBlock_, lpRouteCtl.costMap_))
	{
		armyMap_[searchpoint.y_][searchpoint.x_] = SceneMng::Blue;
	}
	//初期領地作成(敵)
	for (auto& searchpoint : SearchMapCtl::GetSearchMovePointSum(lptmxobj.GetEnemyCorePos(), coreRange, limitNumBlock_, lpRouteCtl.costMap_))
	{
		armyMap_[searchpoint.y_][searchpoint.x_] = SceneMng::Red;
	}
}

void WarSituationGraph::Draw(Vector2 camOffset)
{
	//ブロックサイズ
	auto BlockSize = lpRouteCtl.GetBlockSize();
	
	//有効領地のカウント
	int validCnt = 0;
	//味方領地カウント
	int allyCnt = 0;
	//敵領地カウント
	int enemyCnt = 0;

	//戦況の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
	for (int y = 0; y < limitNumBlock_.y_; y++)
	{
		for (int x = 0; x < limitNumBlock_.x_; x++)
		{
			if (lpRouteCtl.costMap_[y][x] != lpRouteCtl.GetCantMovePoint())
			{
				//有効領地をカウント
				validCnt++;
			}

			if (armyMap_[y][x] == SceneMng::Black)
			{
				//中立領地は省く
				continue;
			}

			if (armyMap_[y][x] == SceneMng::Blue)
			{
				//味方カウント
				allyCnt++;
			}

			if (armyMap_[y][x] == SceneMng::Red)
			{
				//敵カウント
				enemyCnt++;
			}

			//全体戦況の色ブロック表示
			DrawBox(x * BlockSize + camOffset.x_, y * BlockSize + camOffset.y_ + lpSceneMng.GetOffset(),
				x * BlockSize + BlockSize + camOffset.x_, y * BlockSize + BlockSize + camOffset.y_ + lpSceneMng.GetOffset(),
				armyMap_[y][x], true);
		}
	}

	//α値
	int alpha = 255;

	//現在のチュートリアルが自分以外だと薄くする
	if (lpInputString.GetNowPlace() != PLACE::WARSITUATION && lpInputString.GetNowPlace() != PLACE::MAX)
	{
		alpha = lpSceneMng.alpha_;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	//上の戦況表示
	float allyRate = static_cast<float>((lpSceneMng.GetViewArea().x_ / 2) / validCnt)* allyCnt;
	float enemyRate = static_cast<float>((lpSceneMng.GetViewArea().x_ / 2) / validCnt)* enemyCnt;

	float blue = min(DEF_COLOR + (allyCnt / max(enemyCnt,1) * COLOR_RATE) , 255.0f);
	float red = min(DEF_COLOR + (enemyCnt / max(allyCnt, 1) * COLOR_RATE), 255.0f);

	//float blue = min(DEF_COLOR + (200.0f / static_cast<float>(validCnt))* allyCnt, 255.0f);
	//float red = min(DEF_COLOR + (200.0f / static_cast<float>(validCnt))* enemyCnt, 255.0f);
	
	DrawBox(WAR_OFFSET_X, WAR_OFFSET_Y,
		lpSceneMng.GetViewArea().x_ - WAR_OFFSET_X, WAR_OFFSET_Y + WAR_SIZE_Y,
		GetColor(static_cast<int>(red), 20, 20), true);
	DrawBoxAA(WAR_OFFSET_X, WAR_OFFSET_Y,
		static_cast<float>(lpSceneMng.GetViewArea().x_ / 2) + allyRate - enemyRate, WAR_OFFSET_Y + WAR_SIZE_Y,
		GetColor(20, 20, static_cast<int>(blue)), true);

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

	//「自軍」
	DrawGraph(0, TEXT_OFFSET_Y, lpImageMng.GetID("AllyText")[0], true);
	if (enemyCnt + POINT_DIFFRENCE_ADVANTAGE < allyCnt)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 205);
		DrawExtendGraph(0, TEXT_OFFSET_Y,
			BLINKLIGHT_SIZE_X, TEXT_OFFSET_Y + BLINKLIGHT_SIZE_Y,
			lpImageMng.GetID("BlinkLight")[animId_], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	//「敵軍」
	DrawGraph(lpSceneMng.GetViewArea().x_ - TEXT_SIZE_X, TEXT_OFFSET_Y,lpImageMng.GetID("EnemyText")[0], true);
	if (allyCnt + POINT_DIFFRENCE_ADVANTAGE < enemyCnt)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 205);
		DrawExtendGraph(lpSceneMng.GetViewArea().x_ - TEXT_SIZE_X - 15, TEXT_OFFSET_Y,
			lpSceneMng.GetViewArea().x_ - TEXT_SIZE_X + BLINKLIGHT_SIZE_X, TEXT_OFFSET_Y + BLINKLIGHT_SIZE_Y,
			lpImageMng.GetID("BlinkLight")[animId_], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	//「戦況」
	DrawExtendGraph((lpSceneMng.GetViewArea().x_ - WAR_SITUATION_LOGO_SIZE_X) / 2, 0,
		(lpSceneMng.GetViewArea().x_ - WAR_SITUATION_LOGO_SIZE_X) / 2 + WAR_SITUATION_LOGO_SIZE_X, WAR_SITUATION_LOGO_SIZE_Y,
		lpImageMng.GetID("WarSituation")[0], true);
	
	//すでに薄くしているためもとに戻す
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
