#pragma once
#include <DxLib.h>
#include <vector>
#include "../../common/Vector2.h"

#define lpWarSituationGraph WarSituationGraph::GetInstance()

class WarSituationGraph
{
public:
	static void Create(Vector2 limitNumBlock)
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new WarSituationGraph(limitNumBlock);
		}
	}
	static void Destroy(void)
	{
		if (s_Instance != nullptr)
		{
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	static WarSituationGraph& GetInstance(void)
	{
		return *s_Instance;
	}

	const std::vector<std::vector<UINT32>>& GetArmyMap(void)
	{
		return armyMap_;
	}

	void SetArmyMap(int blockX, int blockY, UINT32 color);

	//マップの初期化
	void InitMap(void);
	//マップの初期のマス目追加
	void AddInitSquares(int coreRange);

	void Draw(Vector2 camOffset);

private:
	WarSituationGraph(Vector2 limitNumBlock);
	~WarSituationGraph();

	static WarSituationGraph* s_Instance;

	//マップ全体の戦況(色)
	std::vector<std::vector<UINT32>> armyMap_;

	//行動限界マス
	Vector2 limitNumBlock_;

	double loadingTime_;
	int animId_;
};

