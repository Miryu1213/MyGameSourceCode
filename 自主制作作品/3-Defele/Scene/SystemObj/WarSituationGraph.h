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

	//�}�b�v�̏�����
	void InitMap(void);
	//�}�b�v�̏����̃}�X�ڒǉ�
	void AddInitSquares(int coreRange);

	void Draw(Vector2 camOffset);

private:
	WarSituationGraph(Vector2 limitNumBlock);
	~WarSituationGraph();

	static WarSituationGraph* s_Instance;

	//�}�b�v�S�̂̐틵(�F)
	std::vector<std::vector<UINT32>> armyMap_;

	//�s�����E�}�X
	Vector2 limitNumBlock_;

	double loadingTime_;
	int animId_;
};

