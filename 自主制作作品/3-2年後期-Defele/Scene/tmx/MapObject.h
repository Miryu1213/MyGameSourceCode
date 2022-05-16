#pragma once
#include <string>
#include <list>

//�莝���I�u�W�F�N�g���v
struct DeployObjSum
{
	DeployObjSum()
	{
		//������
		kpNum_ = 0;
		wpNum_ = 0;
		apNum_ = 0;

		huricaneNum_ = 0;
	}

	//�{��
	int kpNum_;
	int wpNum_;
	int apNum_;

	//�M�~�b�N
	int huricaneNum_;

	std::list<int> dontMoveList_;
};

struct MapObject
{
	[[nodiscard]] static DeployObjSum GetMapObject(std::string mapname);
};
