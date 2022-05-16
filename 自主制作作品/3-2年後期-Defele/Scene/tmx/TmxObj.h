#pragma once
#include <string>
#include <map>
#include <vector>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "../Obj/Type/ObjType.h"
#include "../../common/Vector2.h"

#define lptmxobj TmxObj::GetInstance()

using MapData = std::map<std::string, std::vector<int>>;	//vector��TMX�̃}�b�v

class TmxObj
{
public:

	static TmxObj& GetInstance(void)
	{
		static TmxObj s_Instance;
		return s_Instance;
	}

	//TMX�̃��[�h�@�����͓ǂݍ��ރt�@�C��
	bool LoadTMX(std::string fileName);

	//TMX�̃}�b�v�����Z�b�g
	bool SetMap(void);

	//���Z�b�g
	void ResetTMX(void);

	const std::vector<int> GetfirstGID(void);
	const unsigned int GetlayerSize(void);
	const Vector2& GetworldArea(void);
	const Vector2& GettileSize(void);

	//�}�b�v�f�[�^�̎擾
	//�ʏ�
	const MapData& GetMapData(void);
	//layer��pos�̈ʒu(���W)��ID��Ԃ�
	const int GetMapData(std::string layer,Vector2 pos);
	//�u���b�N���W��ID��Ԃ�
	const int GetMapData(const std::string& layer, int x, int y);

	//�v�����g�̍��W���u���b�N�l�ŕԂ�
	const std::vector<std::pair<ObjType, Vector2>> GetAllyPlantPos(void);
	const std::vector<std::pair<ObjType, Vector2>> GetEnemyPlantPos(void);
	//�R�A�̍��W���u���b�N�l�ŕԂ�
	const Vector2 GetAllyCorePos(void);
	const Vector2 GetEnemyCorePos(void);

	//���W���͈͊O������
	bool IsOutOfRange(Vector2 blockPos);

	//�}�b�v�̕`��
	void DrawMap(void);

private:
	TmxObj();
	~TmxObj();

	//���� version������m�[�h
	bool CheckTiledVersion(rapidxml::xml_node<>* node, const char* string);

	//ID���I�u�W�F�N�g�^�C�v�֕ϊ�
	ObjType ConvertObjType(int id);

	//�����`�F�b�N
	bool IsAlly(std::string str);

	//�v�����g�`�F�b�N
	bool IsPlant(std::string str);
	//�R�A�`�F�b�N
	bool IsCore(std::string str);
	//�v�����g���R�A�̒T��
	void SearcePlantorCoreAndSetPos(std::stringstream& datastream, std::string str);
	//�v�����g���R�A�̍��W�̃Z�b�g
	void SetPlantorCorePos(std::string str ,int cnt, ObjType objtype);

	//Tiled�̃o�[�W�����Ǘ��p
	std::map<std::string, int> version_;

	//TMX
	//TMX�̃h�L�������g�ۑ��p
	rapidxml::xml_document<> tmxDoc_;
	//�^�C���Z�b�g�̍ŏ�(����)�̃O���[�o���^�C��ID (firstgid)
	std::vector<int> firstGID_;
	//���̃��C���[�܂ł�ID
	unsigned int nextLayerId_;
	//���̉摜�܂ł�ID
	unsigned int nextObjectId_;
	//�S�̂̃T�C�Y(map width height)
	Vector2 worldArea_;
	//�^�C���̃T�C�Y(tile width height)
	Vector2 tileSize_;

	//TMX�̃}�b�v�f�[�^
	MapData mapData_;

	//�{�݂̍��W�Ǝ��
	//����
	std::vector<std::pair<ObjType,Vector2>> allyPlantVArray_;
	//�G
	std::vector<std::pair<ObjType, Vector2>> enemyPlantVArray_;

	//�R�A�̍��W
	//����
	Vector2 allyCorePos_;
	//�G
	Vector2 enemyCorePos_;
};

