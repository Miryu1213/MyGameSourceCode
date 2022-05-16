#include<DxLib.h>
#include <algorithm>
#include<fstream>
#include<iostream>
#include<windows.h>
#include "MapCtl.h"
#include "SceneMng.h"
#include"../common/ImageMng.h"
#include"DrawChip.h"
#include"Obj/Unit.h"
#include"Obj/Slime.h"
#include"Obj/Flour.h"
#include"Obj/Warwolf.h"

#include"../_debug/_DebugConOut.h"

MapCtl* MapCtl::s_instance = nullptr;

void MapCtl::DrawEdit()
{
	DrawMap();
	DrawEditLine();
	//�G�f�B�b�g�A�j���[�V�����p
	//animCnt++;
}

void MapCtl::DrawGame()
{
	DrawMap();
	animCnt_++;
}

bool MapCtl::SetMapData(Vector2 pos, ChipID id, ChipGP chipgp)
{
	//�}�X�ڕϊ�
	Vector2 tmpPos = (pos / chipSize_);		//�u���b�N���W
	//�͈͓��`�F�b�N
	if (0 <= tmpPos.x && tmpPos.x < worldArea_.x && 0 <= tmpPos.y && tmpPos.y < worldArea_.y)
	{
		//�u�����N�̎�
		if (id == ChipID::BLANK && chipgp == ChipGP::MAX)
		{
			mapID_[tmpPos.y][tmpPos.x] = 0;
			TRACE("x:%d,y:%d,ID:%d\n", tmpPos.x, tmpPos.y, mapID_[tmpPos.y][tmpPos.x]);
			return true;
		}

		//���݂̑I���u���b�N��ID����
		//�c�������r�b�g�݂̂ɕϊ�
		auto Condata = mapID_[tmpPos.y][tmpPos.x] & (~(0x000000ff << static_cast<int>(chipgp) * 8));
		//�������݂������̂������̈ʒu��
		auto shiftdata = static_cast<int>(id) << (static_cast<int>(chipgp) * 8);
		//���̓�̘_���a�����}�b�vID�ɑ��
		mapID_[tmpPos.y][tmpPos.x] = Condata | shiftdata;

		//mapID_[tmpPos.y][tmpPos.x] = static_cast<int>(id);
	}
	else
	{
		return false;
	}
	TRACE("x:%d,y:%d,ID:%d\n", tmpPos.x, tmpPos.y, mapID_[tmpPos.y][tmpPos.x]);
	return true;
}

bool MapCtl::SetMapData(Vector2 pos, ChipID id)
{
	return SetMapData(pos, id, chipGP_[id]);
}

bool MapCtl::SetMapData(Vector2 pos, ChipID_ENEMY eneid, ChipGP chipgp)
{
	//�}�X�ڕϊ�
	Vector2 tmpPos = (pos / chipSize_);		//�u���b�N���W
	//�͈͓��`�F�b�N
	if (0 <= tmpPos.x && tmpPos.x < worldArea_.x && 0 <= tmpPos.y && tmpPos.y < worldArea_.y)
	{
		//�u�����N�̎�
		if (eneid == ChipID_ENEMY::MAX && chipgp == ChipGP::MAX)
		{
			mapID_[tmpPos.y][tmpPos.x] = 0;
			TRACE("x:%d,y:%d,ID:%d\n", tmpPos.x, tmpPos.y, mapID_[tmpPos.y][tmpPos.x]);
			return true;
		}

		//���݂̑I���u���b�N��ID����
		//�c�������r�b�g�݂̂ɕϊ�
		auto Condata = mapID_[tmpPos.y][tmpPos.x] & (~(0x000000ff << static_cast<int>(chipgp) * 8));
		//�������݂������̂������̈ʒu��
		auto shiftdata = static_cast<int>(eneid) << (static_cast<int>(chipgp) * 8);
		//���̓�̘_���a�����}�b�vID�ɑ��
		mapID_[tmpPos.y][tmpPos.x] = Condata | shiftdata;

		//mapID_[tmpPos.y][tmpPos.x] = static_cast<int>(id);
	}
	else
	{
		return false;
	}
	TRACE("x:%d,y:%d,ID:%d\n", tmpPos.x, tmpPos.y, mapID_[tmpPos.y][tmpPos.x]);
	return true;
}

bool MapCtl::SetMapData(Vector2 pos, ChipID_ENEMY eneid)
{
	return SetMapData(pos, eneid, chipeneGP_[eneid]);
}

ChipID MapCtl::GetMapDataGame(ChipGP gpID, unsigned int data)
{
	unsigned int Shiftdata = data >> static_cast<unsigned int>(gpID) * 8;
	auto Condata = Shiftdata & (0x000000ff);
	return ChipID(static_cast<ChipID>(Condata));
}

ChipID MapCtl::GetMapDataEdit(ChipGP gpID, unsigned int data)
{
	unsigned int Shiftdata = data >> static_cast<unsigned int>(gpID) * 8;
	auto Condata = Shiftdata & (0x000000ff);

	return ChipID(static_cast<ChipID>(Condata));
}

ChipID MapCtl::GetMapData(ChipGP gpID, int x, int y, bool dig)
{
	//�͈͓��`�F�b�N
	/*if (0 <= x && x < viewArea_.x && 0 <= y && y < viewArea_.y)
	{
	}*/
	//clamp�͈͓��`�F�b�N
	x = std::clamp(x, 0, worldArea_.x - 1);
	y = std::clamp(y, 0, worldArea_.y - 1);
	return GetMapDataFunc(gpID, mapID_[y][x]);;
}

ChipID MapCtl::GetMapData(ChipGP gpID, Vector2 pos)
{
	Vector2 tmpPos = (pos / chipSize_);		//�u���b�N�l�ł̍��W
	return GetMapData(gpID, tmpPos.x, tmpPos.y, true);
}

ChipID_ENEMY MapCtl::GetMapDataEne(ChipGP gpID, int x, int y)
{
	x = std::clamp(x, 0, worldArea_.x - 1);
	y = std::clamp(y, 0, worldArea_.y - 1);
	auto data = mapID_[y][x];
	unsigned int Shiftdata = data >> static_cast<unsigned int>(gpID) * 8;
	auto Condata = Shiftdata & (0x000000ff);
	return ChipID_ENEMY(static_cast<ChipID_ENEMY>(Condata));
}

void MapCtl::SetCamera(std::weak_ptr<Camera> camera)
{
	camera_ = camera;
}

bool MapCtl::MapSave()
{
	DataHeader expData = {
		LDR_ID_NAME,
		LDR_ID_VER,
		{0,0},
		worldArea_.x,
		worldArea_.y,
		{0,0,0},
		0x00
	};
	for (auto num : basemapID_)
	{
		expData.sum += static_cast<unsigned char>(num);
	}
	std::ofstream ofp("mapdata.txt", std::ios::binary);
	ofp.write(reinterpret_cast<char*>(&expData), sizeof(expData));		//exp����������
	ofp.write(reinterpret_cast<char*>(basemapID_.data()), basemapID_.size() * sizeof(basemapID_[0]));	//mapid����������

	return true;
}

bool MapCtl::MapLoad()
{
	InitMap();
	DataHeader expData = {
		LDR_ID_NAME,
		LDR_ID_VER,
		{0,0},
		worldArea_.x,
		worldArea_.y,
		{0,0,0},
		0x00
	};
	std::ifstream ifp("mapdata.txt", std::ios::binary);
	ifp.read(reinterpret_cast<char*>(&expData), sizeof(expData));
	ifp.read(reinterpret_cast<char*>(basemapID_.data()), basemapID_.size() * sizeof(basemapID_[0]));	//�Z�[�u���Ɠ������Ԃœǂݍ���
	unsigned char tmpsum = 0;
	for (auto sum : basemapID_)
	{
		tmpsum += static_cast<unsigned char>(sum);
		/*if ((sum>> static_cast<int>(ChipGP::ITEM) * 8) == (static_cast<unsigned int>(ChipID::GOLD)))
		{
			goldCnt_++;
		}*/
	}
	if (std::string(expData.fileID) != LDR_ID_NAME)
	{
		TRACE("���̃Q�[���̃f�[�^�ł͂���܂���\n");
		InitMap();
		return false;
	}
	if (expData.verID != LDR_ID_VER)
	{
		TRACE("�f�[�^�̃o�[�W�������Ⴂ�܂�\n");
		InitMap();
		return false;
	}
	if (expData.sum != tmpsum)
	{
		TRACE("SUM�l���ُ�l�ł�\n");
		InitMap();
		return false;
	}
	/*if ((expData.fileID != LDR_ID_NAME) || (expData.verID != LDR_ID_VER) || (expData.sum != tmpsum))	//char*�̎d�l��Ō��\0�������߈�v���Ȃ�
	{
		InitMap();
		return false;
	}*/
	return true;
}

bool MapCtl::MapLoad(BaseScene& scene)
{
	if (MapLoad() == 0)
	{
		return false;
	}
	for (int y = 0; y < worldArea_.y; y++)
	{
		for (int x = 0; x < worldArea_.x; x++)
		{
			if (GetMapDataEne(ChipGP::CHAR, x, y) == ChipID_ENEMY::SLIME)
			{
				auto Eneobj = scene.AddobjList(std::make_shared<Slime>(scene.GetSceneID(), camera_));
				Vector2 EnePos;
				EnePos.x = x * chipSize_.x;
				EnePos.y = y * chipSize_.y;
				(*Eneobj)->SetPos(EnePos);
				(*Eneobj)->SetDefPos(EnePos);
				(*Eneobj)->SetDirLR();
			}
			if (GetMapDataEne(ChipGP::CHAR, x, y) == ChipID_ENEMY::FLOUR)
			{
				auto Eneobj = scene.AddobjList(std::make_shared<Flour>(scene.GetSceneID(), camera_));
				Vector2 EnePos;
				EnePos.x = x * chipSize_.x;
				EnePos.y = y * chipSize_.y;
				(*Eneobj)->SetPos(EnePos);
				(*Eneobj)->SetDefPos(EnePos);
				(*Eneobj)->SetDirLR();
			}
			if (GetMapDataEne(ChipGP::CHAR, x, y) == ChipID_ENEMY::WARWOLF)
			{
				auto Eneobj = scene.AddobjList(std::make_shared<Warwolf>(scene.GetSceneID(), camera_));
				Vector2 EnePos;
				EnePos.x = x * chipSize_.x;
				EnePos.y = y * chipSize_.y;
				(*Eneobj)->SetPos(EnePos);
				(*Eneobj)->SetDefPos(EnePos);
				(*Eneobj)->SetDirLR();
			}
		}
	}
	return true;
}

void MapCtl::DrawMap()
{
	Vector2 camOffset;		//�I�t�Z�b�g���W
	Vector2 Blockcamoff;	//�u���b�N�l�ł̃I�t�Z�b�g
	if (!camera_.expired())
	{
		//�J�����������Ă���΁A�`��p�̃I�t�Z�b�g���Z�b�g����
		camOffset = camera_.lock()->GetDrawOffset();
		Blockcamoff = -(camOffset / chipSize_);
	}
	SetFontSize(10);
	for (auto chipGP : ChipGP())
	{
		for (int y = 0; y < worldArea_.y; y++)
		{
			for (int x = Blockcamoff.x; x <= viewArea_.x + Blockcamoff.x; x++)
			{
				if (chipGP != ChipGP::CHAR)
				{
					auto id = GetMapData(chipGP, x, y, false);
					DrawChip_[id](x, y, chipSize_, camOffset, lpImageMng.GetID("mapchip")[static_cast<int>(id)]);
					//DrawFormatString(x * chipSize_.x, y * chipSize_.y, GetColor(255, 255, 255), "x:%d\ny:%d", x * chipSize_.x, y * chipSize_.y);
				}
				else
				{
					auto id = GetMapDataEne(chipGP, x, y);
					if (id == ChipID_ENEMY::BLANK || id == ChipID_ENEMY::MAX)
					{
						continue;
					}
					auto intid = static_cast<int>(id);
					DrawChipEne_[id](x, y, chipSize_, camOffset, lpImageMng.GetID("Enemy" + std::to_string(intid))[0]);
				}
				//if (mapID_[y][x] > 0)
				//{
				//	auto Condata = static_cast<unsigned int>(GetMapData(chipGP, x, y));
				//	if (Condata > 0)
				//	{
				//		DrawChipDef drawDef;
				//		drawDef(x, y, chipSize_, camOffset, lpImageMng.GetID("tileset")[Condata]);
				//		//DrawGraph(x * chipSize_.x + camOffset.x, y * chipSize_.y + camOffset.y, lpImageMng.GetID("tileset")[Condata], true);
				//	}
				//}
			}
		}
	}
}

void MapCtl::DrawEditLine()
{
	//����
	for (int y = 0; y <= viewArea_.y; y++)
	{
		DrawLine(0, y * chipSize_.y, viewArea_.x * chipSize_.x, y * chipSize_.y, GetColor(255, 255, 255));
	}
	//�c��
	for (int x = 0; x <= viewArea_.x; x++)
	{
		DrawLine(x * chipSize_.x, 0, x * chipSize_.x, viewArea_.y * chipSize_.y, GetColor(255, 255, 255));
	}

}

MapCtl::MapCtl(SceneID sceneID)
{
	sceneID_ = sceneID;
	Init();
	if (sceneID == SceneID::EDIT)
	{
		DrawFunc = std::bind(&MapCtl::DrawEdit, this);
		GetMapDataFunc = std::bind(&MapCtl::GetMapDataEdit, this, std::placeholders::_1, std::placeholders::_2);
		InitDrawChipEdit();
	}
	if (sceneID == SceneID::GAME)
	{
		DrawFunc = std::bind(&MapCtl::DrawGame, this);
		GetMapDataFunc = std::bind(&MapCtl::GetMapDataGame, this, std::placeholders::_1, std::placeholders::_2);
		InitDrawChippGame();
	}
}
MapCtl::~MapCtl()
{
}

bool MapCtl::Init()
{
	lpImageMng.GetID("image/mapchip.png", "mapchip", { 32,32 }, { 4,3 });

	animCnt_ = 0;

	chipSize_ = { 32,32 };
	viewArea_ = lpSceneMng.viewArea_ / chipSize_;
	worldArea_ = lpSceneMng.worldArea_ / chipSize_;
	

	//�`�b�v�O���[�v�̐ݒ�
	chipGP_[ChipID::BLANK] = ChipGP::MAX;
	chipGP_[ChipID::BLANK2] = ChipGP::MAX;
	chipGP_[ChipID::GRASS9] = ChipGP::BG;
	chipGP_[ChipID::GRASS6] = ChipGP::BG;
	chipGP_[ChipID::GRASS1] = ChipGP::BG;	
	chipGP_[ChipID::GRASS2] = ChipGP::BG;
	chipGP_[ChipID::GRASS3] = ChipGP::BG;
	chipGP_[ChipID::GRASS4] = ChipGP::BG;
	chipGP_[ChipID::GRASS5] = ChipGP::BG;
	chipGP_[ChipID::GRASS7] = ChipGP::BG;
	chipGP_[ChipID::GRASS8] = ChipGP::BG;
	chipGP_[ChipID::EDIT_CUR] = ChipGP::CHAR;		//�G�f�B�b�g�p�J�[�\��

	chipeneGP_[ChipID_ENEMY::BLANK] = ChipGP::MAX;
	chipeneGP_[ChipID_ENEMY::SLIME] = ChipGP::CHAR;
	chipeneGP_[ChipID_ENEMY::FLOUR] = ChipGP::CHAR;
	chipeneGP_[ChipID_ENEMY::WARWOLF] = ChipGP::CHAR;
	chipeneGP_[ChipID_ENEMY::MAX] = ChipGP::MAX;

	InitMap();
	return true;
}

bool MapCtl::InitMap()
{
	basemapID_.resize(worldArea_.x * worldArea_.y);
	mapID_.resize(worldArea_.y);
	for (int y = 0; y < worldArea_.y; y++)
	{
		mapID_[y] = &basemapID_[y * worldArea_.x];
	}
	for (auto& data : basemapID_)
	{
		data = 0;
	}
	return true;
}

void MapCtl::InitDrawChipEdit()
{
	DrawChip_.try_emplace(ChipID::BLANK, DrawChipNON());
	DrawChip_.try_emplace(ChipID::BLANK2, DrawChipNON());
	DrawChip_.try_emplace(ChipID::GRASS9, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS6, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS1, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS2, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS3, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS4, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS5, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS7, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS8, DrawChipDef());
	//�G�f�B�b�g�p�J�[�\��
	DrawChip_.try_emplace(ChipID::EDIT_CUR, DrawChipNON());

	DrawChipEne_.try_emplace(ChipID_ENEMY::BLANK, DrawChipNON());
	DrawChipEne_.try_emplace(ChipID_ENEMY::SLIME, DrawChipDef());
	DrawChipEne_.try_emplace(ChipID_ENEMY::FLOUR, DrawChipDef());
	DrawChipEne_.try_emplace(ChipID_ENEMY::WARWOLF, DrawChipDef());
	DrawChipEne_.try_emplace(ChipID_ENEMY::MAX, DrawChipNON());
}

void MapCtl::InitDrawChippGame()
{
	DrawChip_.try_emplace(ChipID::BLANK, DrawChipNON());
	DrawChip_.try_emplace(ChipID::BLANK2, DrawChipNON());
	DrawChip_.try_emplace(ChipID::GRASS9, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS1, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS2, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS3, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS4, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS5, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS6, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS7, DrawChipDef());
	DrawChip_.try_emplace(ChipID::GRASS8, DrawChipDef());
	//�G�f�B�b�g�p�J�[�\��
	DrawChip_.try_emplace(ChipID::EDIT_CUR, DrawChipNON());

	DrawChipEne_.try_emplace(ChipID_ENEMY::SLIME, DrawChipNON());
	DrawChipEne_.try_emplace(ChipID_ENEMY::FLOUR, DrawChipNON());
	DrawChipEne_.try_emplace(ChipID_ENEMY::WARWOLF, DrawChipNON());
	DrawChipEne_.try_emplace(ChipID_ENEMY::MAX, DrawChipNON());
}
