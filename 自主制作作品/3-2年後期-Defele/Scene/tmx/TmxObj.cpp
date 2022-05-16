#include <DxLib.h>
#include <cstdlib>
#include <sstream>
#include "TmxObj.h"
#include "../../common/ImageMng.h"

#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"

//�p�X�p�摜���ǉ�
const std::string IMAGEPASS = "Scene/tmx/";

//�u���b�N�T�C�Y
constexpr int DEF_BLOCK_SIZE = 64;

//����^�C����̎��̉摜�܂ł�ID��
constexpr int NEXT_IMAGE_TILE_ID = 32;

//�v�����g�̎�ސ�
constexpr int PLANT_TYPE_NUM = 3;

TmxObj::TmxObj()
{
	version_["1.6.0"] = 1;
	version_["1.7.0"] = 1;
	version_["1.7.2"] = 1;
}

TmxObj::~TmxObj()
{
}

bool TmxObj::LoadTMX(std::string fileName)
{
	//�t�@�C���l�[���̑��
	rapidxml::file<char> xmlFileName = fileName.c_str();
	//�t�@�C�����p�[�T�[��
	tmxDoc_.parse<0>(xmlFileName.data());									
	//�m�[�h�i�����map�j���擾
	rapidxml::xml_node<char>* mapnode = tmxDoc_.first_node("map");			
	
	//�o�[�W�����`�F�b�N
	if (CheckTiledVersion(mapnode, "TMX") == false)
	{
		return false;
	}

	//�e�m�[�h�̎擾
	auto tileset = mapnode->first_node("tileset");

	//�摜�ǂݍ��݂ɕK�v�ȃf�[�^�̓ǂݍ���
	Vector2 tilesize = { atoi(mapnode->first_attribute("tilewidth")->value()),atoi(mapnode->first_attribute("tileheight")->value()) };

	//��
	std::vector<int> columns;
	//�s
	std::vector<int> row;
	//�t�@�C����
	std::string source;

	//300�͐�΂ɂ��ǂ蒅���Ȃ�����
	for (int i = 0; i < 300; i++)
	{
		source = tileset->first_node("image")->first_attribute("source")->value();
		source = IMAGEPASS + source;

		//firstid�̕ۑ�
		firstGID_.push_back(atoi(tileset->first_attribute("firstgid")->value()));

		std::string imageName = tileset->first_attribute("name")->value();
		if (IsPlant(imageName))
		{
			//�{�݂ƃR�A��firstId�����ǂݍ��݉摜�͕ʂɓǂݍ���
			tileset = tileset->next_sibling();
			continue;
		}
		if (IsCore(imageName))
		{
			break;
		}

		//���i��j�̗v�f�����擾-x
		columns.push_back(atoi(tileset->first_attribute("columns")->value()));
		//�c�i�s�j�̗v�f���̎擾-y
		row.push_back(atoi(tileset->first_attribute("tilecount")->value()) / columns[i]);

		lpImageMng.GetID(source, firstGID_[i], tilesize, Vector2{ columns[i],row[i] });

		tileset = tileset->next_sibling();
	}

	//TMX�̒l�̎��o��
	nextLayerId_ = atoi(mapnode->first_attribute("nextlayerid")->value());
	nextObjectId_ = atoi(mapnode->first_attribute("nextobjectid")->value());
	worldArea_ = { atoi(mapnode->first_attribute("width")->value()) ,atoi(mapnode->first_attribute("height")->value()) };
	tileSize_ = { atoi(mapnode->first_attribute("tilewidth")->value()) ,atoi(mapnode->first_attribute("tileheight")->value()) };

	return SetMap();
}

bool TmxObj::SetMap(void)
{
	//�m�[�h�i�����map�j���擾
	rapidxml::xml_node<char>* mapnode = tmxDoc_.first_node("map");
	if (!mapnode)
	{
		TRACE("map�����݂��܂���\n");
		return false;
	}

	//�f�[�^�̎擾��mapData_��
	for (auto layernode = mapnode->first_node("layer"); layernode != nullptr; layernode = layernode->next_sibling())
	{
		std::string imageName = layernode->first_attribute("name")->value();

		auto datanode = layernode->first_node("data")->first_node();

		//�X�g�����O�X�g���[���^�ɕς��AmapData_�ɑ��
		std::stringstream dataStream;		        //�}�b�v�f�[�^�̃X�g���[���^
		dataStream << datanode->value();

		if (IsPlant(imageName) || IsCore(imageName))
		{
			//���W�̃Z�b�g
			SearcePlantorCoreAndSetPos(dataStream, imageName);
			continue;
		}

		auto layername = layernode->first_attribute("name")->value();
		//�L�[�̑��
		if (mapData_.try_emplace(layername).second == true)
		{
			//�f�[�^���̃��T�C�Y�E���
			mapData_[layername].resize(worldArea_.x_ * worldArea_.y_);
		}

		std::string numStr;
		for (auto& data : mapData_[layername])
		{
			numStr.clear();
			std::getline(dataStream, numStr, ',');	//�擾�i�R���}�����̈�̃}�b�v�̐����j
			data = atoi(numStr.c_str());
		}
	}
	return true;
}

void TmxObj::ResetTMX(void)
{
	for (auto& id : firstGID_)
	{
		lpImageMng.DeleteID(id);
	}

	firstGID_.clear();
	//���̃��C���[�܂ł�ID
	nextLayerId_ = 0;
	//���̉摜�܂ł�ID
	nextObjectId_ = 0;

	//TMX�̃}�b�v�f�[�^
	mapData_.clear();

	//�{�݂̍��W�Ǝ��
	//����
	allyPlantVArray_.clear();
	//�G
	enemyPlantVArray_.clear();

	//�R�A�̍��W
	//����
	allyCorePos_ = { 0,0 };
	//�G
	enemyCorePos_ = { 0,0 };
}

const std::vector<int> TmxObj::GetfirstGID(void)
{
	return firstGID_;
}

const unsigned int TmxObj::GetlayerSize(void)
{
	return nextLayerId_;
}

const Vector2& TmxObj::GetworldArea(void)
{
	return worldArea_;
}

const Vector2& TmxObj::GettileSize(void)
{
	return tileSize_;
}

const MapData& TmxObj::GetMapData(void)
{
	return mapData_;
}

const int TmxObj::GetMapData(std::string layer, Vector2 pos)
{
	//���̍��W��
	return GetMapData(layer, pos.x_ / tileSize_.x_, pos.y_ / tileSize_.y_);
}

const int TmxObj::GetMapData(const std::string& layer, int x, int y)
{
	if (mapData_.count(layer) == 0)
	{
		//����layer�����݂��Ȃ��Ƃ�
		TRACE("���C���[%s�����݂��܂���\n", layer.c_str());
		return 0;
	}
	unsigned int point = y * worldArea_.x_ + x;
	if (0 <= point && point < mapData_[layer].size())
	{
		//�͈͓��̎�
		return mapData_[layer][point];
	}
	else
	{
		TRACE("%d,%d�͔͈͊O�ł�\n", x, y);
		return 0;
	}
}

const std::vector<std::pair<ObjType, Vector2>> TmxObj::GetAllyPlantPos(void)
{
	std::vector<std::pair<ObjType, Vector2>> tmpAllyPlantVec;
	for (auto& plantPos : allyPlantVArray_)
	{
		tmpAllyPlantVec.push_back({ plantPos.first,plantPos.second * tileSize_ / Vector2{ DEF_BLOCK_SIZE,DEF_BLOCK_SIZE } });
	}

	return tmpAllyPlantVec;
}

const std::vector<std::pair<ObjType, Vector2>> TmxObj::GetEnemyPlantPos(void)
{
	std::vector<std::pair<ObjType, Vector2>> tmpEnemyPlantVec;
	for (auto& plantPos : enemyPlantVArray_)
	{
		tmpEnemyPlantVec.push_back({ plantPos.first,plantPos.second * tileSize_ / Vector2{ DEF_BLOCK_SIZE,DEF_BLOCK_SIZE } });
	}

	return tmpEnemyPlantVec;
}

const Vector2 TmxObj::GetAllyCorePos(void)
{
	return allyCorePos_ * tileSize_ / Vector2{ DEF_BLOCK_SIZE,DEF_BLOCK_SIZE };
}

const Vector2 TmxObj::GetEnemyCorePos(void)
{
	return enemyCorePos_ * tileSize_ / Vector2{ DEF_BLOCK_SIZE,DEF_BLOCK_SIZE };
}

bool TmxObj::IsOutOfRange(Vector2 blockPos)
{
	//�ꎟ���ɕύX
	auto point = blockPos.x_ + blockPos.y_ * worldArea_.x_;

	for (auto& layer : mapData_)
	{
		if (0 <= point && point < mapData_[layer.first].size())
		{
			//�͈͓��̎�
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void TmxObj::DrawMap(void)
{
	//���݂̃��C���[ID
	int id = 0;
	//id - layerId�̍ŏ��i���o�[
	int minlayerid = 5000;
	//���背�C���[
	int decLayer = 0;
	
	//�}�b�v�f�[�^�̕`��
	for (auto layer = --mapData_.end(); layer != --mapData_.begin(); layer--)
	{
		for (int y = 0; y < worldArea_.y_; y++)
		{
			for (int x = 0; x < worldArea_.x_; x++)
			{
				id = GetMapData(layer->first, x, y);

				//id��0�̏ꍇ
				if (id == 0)
				{
					continue;
				}

				minlayerid = 5000;

				for (auto layerId : firstGID_)
				{
					//id-layerid�������Ƃ����������̂�layerid�Ƃ���
					if (id - layerId < minlayerid && 0 <= id - layerId)
					{
						minlayerid = id - layerId;
						decLayer = layerId;
					}
				}
				//ImageMng����0����n�܂��Ă��邽��fistid������
				id = id - decLayer;

				DrawGraph(x * tileSize_.x_, y * tileSize_.y_, lpImageMng.GetID(decLayer)[id], true);
			}
		}
	}
}

bool TmxObj::CheckTiledVersion(rapidxml::xml_node<>* node, const char* string)
{
	//NULL�`�F�b�N
	if (!node)
	{
		TRACE("%s:node�����݂��܂���\n", string);
		return false;
	}
	auto tiledversion = node->first_attribute("tiledversion");
	if (!tiledversion)
	{
		TRACE("%s:tiledversion�����݂��܂���\n", string);
		return false;
	}
	//�o�[�W�����`�F�b�N
	if (!(version_.count(tiledversion->value())))
	{
		//��v������̂��Ȃ��Ƃ�
		TRACE("%s��Ή��o�[�W����\n", string);
		return false;
	}
	if (!version_[tiledversion->value()])
	{
		//��d�`�F�b�N
		TRACE("%s��Ή��o�[�W����\n",string);
		return false;
	}
	TRACE("%s�Ή��o�[�W����\n", string);
	return true;
}

ObjType TmxObj::ConvertObjType(int id)
{
	//�{�݂�ID�̏ꍇ
	int plantId = *(--(--firstGID_.end()));
	int plantNextId = firstGID_.back();

	if (!(plantId <= id && id < plantNextId))
	{
		//�R�A�̏ꍇ
		return ObjType::CORE;
	}

	//ID�̌v�Z
	id -= plantId;
	int objId = id / NEXT_IMAGE_TILE_ID;

	//�G�̏ꍇ�����邽��
	
	ObjType tmpobjtype;

	switch (objId)
	{
	case 0:
		tmpobjtype = ObjType::KNIGHTPLANT;
		break;
	case 1:
		tmpobjtype = ObjType::ARCHERPLANT;
		break;
	case 2:
		tmpobjtype = ObjType::WARRIORPLANT;
		break;
	default:
		assert(!"�s����ID�ł�");
		break;
	}

	return tmpobjtype;
}

bool TmxObj::IsAlly(std::string str)
{
	return str.find("ally") != str.npos;
}

bool TmxObj::IsPlant(std::string str)
{
	return str.find("plant") != str.npos;
}

bool TmxObj::IsCore(std::string str)
{
	return str.find("core") != str.npos;
}

void TmxObj::SearcePlantorCoreAndSetPos(std::stringstream& datastream, std::string str)
{
	std::string numStr;

	int cnt = 0;
	while (std::getline(datastream, numStr, ','))
	{
		//�������Ƒ�O�����͏��߂̃��e�����ɋL�ڂ���Ă��镶���@
		if ((numStr != "\r\n0") && (numStr != "0") && (numStr != "0\r\n"))
		{
			//�{�݂��R�A���������ꍇ
			SetPlantorCorePos(str, cnt, ConvertObjType(atoi(numStr.c_str())));
		}
		cnt++;
	}
}

void TmxObj::SetPlantorCorePos(std::string str ,int cnt , ObjType objtype)
{
	Vector2 pos = Vector2(cnt % worldArea_.x_, cnt / worldArea_.x_);
	Vector2 blockPos = pos * tileSize_ / Vector2{ DEF_BLOCK_SIZE,DEF_BLOCK_SIZE };

	//���̒l�����łɊi�[����Ă���ꍇ�e��
	if (std::find_if(allyPlantVArray_.begin(), allyPlantVArray_.end(),[&](std::pair<ObjType,Vector2> vec) { return vec.second * tileSize_ / Vector2{ DEF_BLOCK_SIZE,DEF_BLOCK_SIZE } == blockPos; }) != allyPlantVArray_.end())
	{
		return;
	}
	if (std::find_if(enemyPlantVArray_.begin(), enemyPlantVArray_.end(), [&](std::pair<ObjType, Vector2> vec) { return vec.second * tileSize_ / Vector2{ DEF_BLOCK_SIZE,DEF_BLOCK_SIZE } == blockPos; }) != enemyPlantVArray_.end())
	{
		return;
	}


	if (IsPlant(str))
	{
		//�{��
		if (IsAlly(str))
		{
			//����
			allyPlantVArray_.push_back({ objtype , pos });
		}
		else
		{
			//�G
			enemyPlantVArray_.push_back({ objtype , pos });
		}
	}
	else
	{
		//�R�A
		if (IsAlly(str))
		{
			//����
			allyCorePos_ = pos;
		}
		else
		{
			//�G
			enemyCorePos_ = pos;
		}
	}
}
