#include "MapObject.h"

DeployObjSum MapObject::GetMapObject(std::string mapname)
{
	DeployObjSum tmpDeploySum;

	if (mapname == "map0")
	{
		tmpDeploySum.kpNum_ = 2;
		tmpDeploySum.apNum_ = 2;
		tmpDeploySum.wpNum_ = 2;

		tmpDeploySum.huricaneNum_ = 5;

		tmpDeploySum.dontMoveList_.push_back(353);
	}
	if (mapname == "map1")
	{
		tmpDeploySum.kpNum_ = 1;
		tmpDeploySum.apNum_ = 0;
		tmpDeploySum.wpNum_ = 2;

		tmpDeploySum.huricaneNum_ = 6;

		tmpDeploySum.dontMoveList_.push_back(1729);
		tmpDeploySum.dontMoveList_.push_back(4033);
	}
	if (mapname == "map2")
	{
		tmpDeploySum.kpNum_ = 2;
		tmpDeploySum.apNum_ = 2;
		tmpDeploySum.wpNum_ = 1;

		tmpDeploySum.huricaneNum_ = 2;

		tmpDeploySum.dontMoveList_.push_back(1);
	}
	if (mapname == "map3")
	{
		tmpDeploySum.kpNum_ = 1;
		tmpDeploySum.apNum_ = 2;
		tmpDeploySum.wpNum_ = 1;

		tmpDeploySum.huricaneNum_ = 3;
	}
	if (mapname == "map4")
	{
		tmpDeploySum.kpNum_ = 1;
		tmpDeploySum.apNum_ = 1;
		tmpDeploySum.wpNum_ = 0;

		tmpDeploySum.huricaneNum_ = 2;

		tmpDeploySum.dontMoveList_.push_back(1729);
		tmpDeploySum.dontMoveList_.push_back(4033);
		tmpDeploySum.dontMoveList_.push_back(6337);
		tmpDeploySum.dontMoveList_.push_back(6985);
	}

	return tmpDeploySum;
}
