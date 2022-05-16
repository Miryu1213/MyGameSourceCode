#include <DxLib.h>
#include "ImageMng.h"
#include "Vector3.h"

#include"../_debug/_DebugConOut.h"

const VecInt& ImageMng::GetID(const std::string& key)
{
	return imageMap_[key];
}

const VecInt& ImageMng::GetID(int key)
{
	auto keystr = std::to_string(key);
	return imageMap_[keystr];
}

const VecInt& ImageMng::GetID(std::string f_name, std::string key)
{
	//key�̗v�f��imageMap_���ɑ��݂��邩�`�F�b�N
	if (imageMap_.find(key) == imageMap_.end())
	{
		//���݂��Ă��Ȃ��Ƃ�
		imageMap_[key].resize(1);						//Vector�̃T�C�Y��ύX
		imageMap_[key][0] = LoadGraph(f_name.c_str());	//�O���t�B�b�N�n���h������(��ڂ̔z���Vector�̂���)
		if ((imageMap_[key][0]) == -1)
		{
			//assert(!"�ǂݍ��ݎ��s");
			TRACE("%s�ǂݍ��ݎ��s\n", f_name.c_str());
		}
	}
	return imageMap_[key];
}

const VecInt& ImageMng::GetID(std::string f_name, std::string key, Vector2 divsize, Vector2 divcnt)
{
	//key�̗v�f��imageMap_���ɑ��݂��邩�`�F�b�N
	if (imageMap_.find(key) == imageMap_.end())
	{
		//���݂��Ă��Ȃ��Ƃ�
		imageMap_[key].resize(static_cast<UINT64>(divcnt.x_ * divcnt.y_));	//Vector�̃T�C�Y��ύX
		if (LoadDivGraph(f_name.c_str(), divcnt.x_ * divcnt.y_, divcnt.x_, divcnt.y_, divsize.x_, divsize.y_, &imageMap_[key][0], true) == -1)
		{
			//assert(!"�ǂݍ��ݎ��s");
			TRACE("%s�ǂݍ��ݎ��s\n", f_name.c_str());
		}
	}
	return imageMap_[key];
}

const VecInt& ImageMng::GetID(std::string f_name, int key, Vector2 divsize, Vector2 divcnt)
{
	auto keystr = std::to_string(key);
	//key�̗v�f��imageMap_���ɑ��݂��邩�`�F�b�N
	if (imageMap_.find(keystr) == imageMap_.end())
	{
		//���݂��Ă��Ȃ��Ƃ�
		imageMap_[keystr].resize(static_cast<UINT64>(divcnt.x_ * divcnt.y_));	//Vector�̃T�C�Y��ύX
		if (LoadDivGraph(f_name.c_str(), divcnt.x_ * divcnt.y_, divcnt.x_, divcnt.y_, divsize.x_, divsize.y_, &imageMap_[keystr][0], true) == -1)
		{
			//assert(!"�ǂݍ��ݎ��s");
			TRACE("%s�ǂݍ��ݎ��s\n", f_name.c_str());
		}
	}
	return imageMap_[keystr];
}

void ImageMng::SetID(std::string f_name, std::string key)
{
	//���݂��Ă��Ȃ��Ƃ�
	imageMap_[key].resize(static_cast<int>(imageMap_[key].size()) + 1);						//Vector�̃T�C�Y��ύX
	imageMap_[key][static_cast<int>(imageMap_[key].size()) - 1] = LoadGraph(f_name.c_str());	//�O���t�B�b�N�n���h������(��ڂ̔z���Vector�̂���)
	if ((imageMap_[key][static_cast<int>(imageMap_[key].size()) - 1]) == -1)
	{
		//assert(!"�ǂݍ��ݎ��s");
		TRACE("%s�ǂݍ��ݎ��s\n", f_name.c_str());
	}
}

bool ImageMng::IsKey(std::string key)
{
	return imageMap_.find(key) != imageMap_.end();
}

void ImageMng::GraphBlightFilter(const std::string& key, int CmpType, int CmpParam, int ClipFillFlag, unsigned int ClipFillColor, int ClipFillAlpha)
{
	for (auto& gh : imageMap_[key])
	{
		GraphFilter(gh, DX_GRAPH_FILTER_BRIGHT_CLIP, CmpType, CmpParam, ClipFillFlag, ClipFillColor, ClipFillAlpha);
	}
}

const VecInt& ImageMng::GetDotID(std::string f_name, std::string key)
{
	//key�̗v�f��imageMap_���ɑ��݂��邩�`�F�b�N
	if (dotImageMap_.find(key) == dotImageMap_.end())
	{
		//���݂��Ă��Ȃ��Ƃ�
		dotImageMap_[key].resize(1);							//Vector�̃T�C�Y��ύX
		dotImageMap_[key][0] = LoadSoftImage(f_name.c_str());
		if ((dotImageMap_[key][0]) == -1)
		{
			//assert(!"�ǂݍ��ݎ��s");
			TRACE("%s�ǂݍ��ݎ��s\n", f_name.c_str());
		}
	}
	return dotImageMap_[key];
}

const VecInt& ImageMng::GetDotID(std::string key)
{
	return dotImageMap_[key];
}

const VecInt& ImageMng::GetDotID(std::string f_name, std::string key, Vector2 divsize, Vector2 divcnt)
{
	//key�̗v�f��imageMap_���ɑ��݂��邩�`�F�b�N
	if (dotImageMap_.find(key) == dotImageMap_.end())
	{
		//�����O�̉摜�̎擾
		int tmpImage = LoadSoftImage(f_name.c_str());

		//���݂��Ă��Ȃ��Ƃ�
		dotImageMap_[key].resize(static_cast<UINT64>(divcnt.x_ * divcnt.y_));	//Vector�̃T�C�Y��ύX
		if (CreateDivGraphFromSoftImage(tmpImage, divcnt.x_ * divcnt.y_, divcnt.x_, divcnt.y_, divsize.x_, divsize.y_, dotImageMap_[key].data()) == -1)
		{
			//assert(!"�ǂݍ��ݎ��s");
			TRACE("%s�ǂݍ��ݎ��s\n", f_name.c_str());
		}
	}
	return dotImageMap_[key];
}

void ImageMng::SetColorAndID(const int fromColor, const TOCOLOR toColor, const std::string& f_name, const std::string& key, Vector2 divsize, Vector2 divcnt)
{
	//�ύX�F�̊m��
	Vector3 tocolor;

	//���̐F���e�J���[���ƂɎ擾
	Vector3 fromColorVec3 = Vector3{ (fromColor & 0xff0000) >> (8 * 2) ,(fromColor & 0x00ff00) >> (8),fromColor & 0x0000ff };

	//�l����ԑ傫���l��ύX����
	int checkColorVal = max(max(fromColorVec3.x, fromColorVec3.y), fromColorVec3.z);

	//��L���ǂ̐F�����f
	CHECKCOLOR checkColor;
	if (checkColorVal == fromColorVec3.x)
	{
		checkColor = CHECKCOLOR::RED;
	}
	else
	{
		if (checkColorVal == fromColorVec3.y)
		{
			checkColor = CHECKCOLOR::GREEN;
		}
		else
		{
			checkColor = CHECKCOLOR::BLUE;
		}
	}

	//���̓�l�̍��̌��E
	int limitDiff = 16;
	if (key.find("Warrior") != key.npos)
	{
		//��m�̏ꍇ�́A�摜�̊֌W�㍷�̌��E�l�����炷
		limitDiff = 2;
	}

	//�擾�����F
	Vector3 imgColor;
	int imgAlpha = 0;

	//�����O�̉摜�̎擾
	int tmpImage = LoadSoftImage(f_name.c_str());

	for (int y = 0; y < divcnt.y_ * divsize.y_; y++)
	{
		for (int x = 0; x < divcnt.x_ * divsize.x_; x++)
		{
			GetPixelSoftImage(tmpImage, x, y, &imgColor.x, &imgColor.y, &imgColor.z, &imgAlpha);

			switch (toColor)
			{
			case TOCOLOR::BLUE:
				tocolor.x = imgColor.x;
				tocolor.y = imgColor.y;
				tocolor.z = 235;
				break;
			case TOCOLOR::RED:
				tocolor.x = 235;
				tocolor.y = imgColor.y;
				tocolor.z = imgColor.z;
				break;
			default:
				TRACE("�F�̒l���s���ł�");
				return;
			}

			bool continueFlag = false;
			switch (checkColor)
			{
			case CHECKCOLOR::RED:
				//���̓�l�̂��F�̒l���Ⴂ�ꍇ
				if ((imgColor.x - limitDiff > imgColor.y) && (imgColor.x - limitDiff > imgColor.z))
				{
					continueFlag = true;
				}
				break;
			case CHECKCOLOR::GREEN:
				//���̓�l�̂��F�̒l���Ⴂ�ꍇ
				if ((imgColor.y - limitDiff < imgColor.x) || (imgColor.y - limitDiff < imgColor.z))
				{
					continueFlag = true;
				}
				break;
			case CHECKCOLOR::BLUE:
				//���̓�l�̂��F�̒l���Ⴂ�ꍇ
				if ((imgColor.z - limitDiff < imgColor.y) || (imgColor.z - limitDiff < imgColor.x))
				{
					continueFlag = true;
				}
				break;
			default:
				TRACE("�F�̒l���s���ł�");
				return;
			}

			if (continueFlag)
			{
				continue;
			}

			DrawPixelSoftImage(tmpImage, x, y, tocolor.x, tocolor.y, tocolor.z, imgAlpha);
		}
	}

	dotImageMap_[key].resize(static_cast<UINT64>(divcnt.x_* divcnt.y_));	//Vector�̃T�C�Y��ύX
	if (CreateDivGraphFromSoftImage(tmpImage, divcnt.x_ * divcnt.y_, divcnt.x_, divcnt.y_, divsize.x_, divsize.y_, &dotImageMap_[key][0]) == -1)
	{
		//assert(!"�ǂݍ��ݎ��s");
		TRACE("%s�ǂݍ��ݎ��s\n", f_name.c_str());
	}
}

void ImageMng::SetAllColorAndID(const TOCOLOR toColor, const std::string& f_name, const std::string& key, Vector2 divsize, Vector2 divcnt)
{
	//�ύX�F�̊m��
	Vector3 tocolor;
	
	//���̓�l�̍��̌��E
	int limitDiff = 16;
	if (key.find("Warrior") != key.npos)
	{
		//��m�̏ꍇ�́A�摜�̊֌W�㍷�̌��E�l�����炷
		limitDiff = 2;
	}

	//�擾�����F
	Vector3 imgColor;
	int imgAlpha = 0;

	//�����O�̉摜�̎擾
	int tmpImage = LoadSoftImage(f_name.c_str());

	for (int y = 0; y < divcnt.y_ * divsize.y_; y++)
	{
		for (int x = 0; x < divcnt.x_ * divsize.x_; x++)
		{
			GetPixelSoftImage(tmpImage, x, y, &imgColor.x, &imgColor.y, &imgColor.z, &imgAlpha);

			switch (toColor)
			{
			case TOCOLOR::BLUE:
				tocolor.x = max(imgColor.x - 25, 0);
				tocolor.y = max(imgColor.y - 25, 0);
				tocolor.z = 255;
				break;
			case TOCOLOR::RED:
				tocolor.x = 255;
				tocolor.y = max(imgColor.y - 25, 0);
				tocolor.z = max(imgColor.z - 25, 0);
				break;
			default:
				TRACE("�F�̒l���s���ł�");
				return;
			}

			//�ǂݍ��񂾂��ׂĂ��w�肵���F�ŉ��Z����
			DrawPixelSoftImage(tmpImage, x, y, tocolor.x, tocolor.y, tocolor.z, imgAlpha);
		}
	}

	dotImageMap_[key].resize(static_cast<UINT64>(divcnt.x_ * divcnt.y_));	//Vector�̃T�C�Y��ύX
	if (CreateDivGraphFromSoftImage(tmpImage, divcnt.x_ * divcnt.y_, divcnt.x_, divcnt.y_, divsize.x_, divsize.y_, &dotImageMap_[key][0]) == -1)
	{
		//assert(!"�ǂݍ��ݎ��s");
		TRACE("%s�ǂݍ��ݎ��s\n", f_name.c_str());
	}
}

bool ImageMng::IsDotKey(std::string key)
{
	return dotImageMap_.find(key) != dotImageMap_.end();
}

void ImageMng::DeleteID(const std::string& key)
{
	for (auto& gh : imageMap_[key])
	{
		DeleteGraph(gh);
	}
	imageMap_[key].clear();
	imageMap_[key].shrink_to_fit();
	imageMap_.erase(key);
	return;
}

void ImageMng::DeleteID(int key)
{
	auto keystr = std::to_string(key);
	//key�̗v�f��imageMap_���ɑ��݂��邩�`�F�b�N
	if (imageMap_.find(keystr) != imageMap_.end())
	{
		for (auto& gh : imageMap_[keystr])
		{
			DeleteGraph(gh);
		}
		//���݂��Ă���΍폜
		imageMap_[keystr].clear();
		imageMap_.erase(keystr);
	}
	return;
}

std::string ImageMng::ConvertObjTypeToKey(ObjType objtype, ArmyType armytype)
{
	std::string tmpKey;

	switch (objtype)
	{
	case ObjType::KNIGHT:
		tmpKey = "Knight";
		break;
	case ObjType::WARRIOR:
		tmpKey = "Warrior";
		break;
	case ObjType::ARCHER:
		tmpKey = "Archer";
		break;
	case ObjType::KNIGHTPLANT:
		tmpKey = "Knightplant";
		break;
	case ObjType::WARRIORPLANT:
		tmpKey = "Warriorplant";
		break;
	case ObjType::ARCHERPLANT:
		tmpKey = "Archerplant";
		break;
	default:
		TRACE("�o�^�O�ł�");
		break;
	}

	if (armytype == ArmyType::ENEMY)
	{
		//�G�̏ꍇ
		switch (objtype)
		{
		case ObjType::KNIGHTPLANT:
			tmpKey = "Knightplantenemy";
			break;
		case ObjType::WARRIORPLANT:
			tmpKey = "Warriorplantenemy";
			break;
		case ObjType::ARCHERPLANT:
			tmpKey = "Archerplantenemy";
			break;
		default:
			TRACE("�o�^�O�ł�");
			break;
		}
	}

	return tmpKey;
}

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}
