#include <iostream>
#include "Tips.h"
#include "../../common/csv.h"

//�ǂݍ��ݗ�
constexpr int COLUMNS = 2;

Tips::Tips()
{
	int mapNumber;
	std::string mapTips;

	//CSV�ǂݍ���
	io::CSVReader<COLUMNS> in("Resource/TextData/MapTips.csv");
	//�w�b�_�[���̃Z�b�g
	in.read_header(io::ignore_extra_column, "map", "tips");

	//�ǂݍ��ݏ��̕ۑ�
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
