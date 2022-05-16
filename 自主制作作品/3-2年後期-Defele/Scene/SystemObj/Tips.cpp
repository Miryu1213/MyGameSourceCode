#include <iostream>
#include "Tips.h"
#include "../../common/csv.h"

//読み込み列数
constexpr int COLUMNS = 2;

Tips::Tips()
{
	int mapNumber;
	std::string mapTips;

	//CSV読み込み
	io::CSVReader<COLUMNS> in("Resource/TextData/MapTips.csv");
	//ヘッダー情報のセット
	in.read_header(io::ignore_extra_column, "map", "tips");

	//読み込み情報の保存
	while (in.read_row(mapNumber,mapTips))
	{
		if (tips_.try_emplace(mapNumber).second)
		{
			tips_[mapNumber] = mapTips;
		}
	}
}

std::string Tips::GetTip(int num)
{
	if (num < static_cast<int>(tips_.size()))
	{
		return tips_[num];
	}
	return nullptr;
}
