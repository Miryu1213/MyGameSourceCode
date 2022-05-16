#include<Dxlib.h>
#include "ImageMng.h"
#include"../_debug/_DebugConOut.h"

const Vecint& ImageMng::GetID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt)
{
	if (imgMap_.find(key) == imgMap_.end())
	{
		TRACE("�t�@�C���̓ǂݍ���\n");
		imgMap_[key].resize(divCnt.x * divCnt.y);
		//�摜�ǂݍ���łȂ��ꍇ�A�摜�ǂݍ���
		if ((LoadDivGraph(f_name.c_str(), divCnt.x * divCnt.y, divCnt.x, divCnt.y, divSize.x, divSize.y, &imgMap_[key][0], true)) == -1)
		{
			TRACE("�摜�̓ǂݍ��݂Ɏ��s\n");
		}
		if (key == "player")
		{
			LoadDivGraph("image/Atack1.png", 3, 3, 1, 276, 96, atack1, true);
			LoadDivGraph("image/Atack2.png", 3, 3, 1, 276, 192, atack2, true);
		}
	}
	return imgMap_[key];
}

const Vecint& ImageMng::GetID(std::string key)
{
	if (imgMap_.find(key) == imgMap_.end())
	{
		//�摜��ǂݍ��߂ĂȂ��Ƃ�
		imgMap_[key].resize(1);
		imgMap_[key][0] = LoadGraph(key.c_str());
	}
	return imgMap_[key];
}

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}
