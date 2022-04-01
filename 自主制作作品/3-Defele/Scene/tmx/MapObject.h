#pragma once
#include <string>
#include <list>

//手持ちオブジェクト合計
struct DeployObjSum
{
	DeployObjSum()
	{
		//初期化
		kpNum_ = 0;
		wpNum_ = 0;
		apNum_ = 0;

		huricaneNum_ = 0;
	}

	//施設
	int kpNum_;
	int wpNum_;
	int apNum_;

	//ギミック
	int huricaneNum_;

	std::list<int> dontMoveList_;
};

struct MapObject
{
	[[nodiscard]] static DeployObjSum GetMapObject(std::string mapname);
};
