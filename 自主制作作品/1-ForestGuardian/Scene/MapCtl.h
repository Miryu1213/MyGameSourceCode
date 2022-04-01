#pragma once
#include<vector>
#include<map>
#include<functional>
#include<list>
#include"Obj/Obj.h"
#include"Obj/ChipID.h"
#include"Obj/ChipGP.h"
#include"../common/vector2.h"

#include"BaseScene.h"

using MapChipGP = std::map<ChipID, ChipGP>;			//ID����GP��
using MapChipEneGP= std::map<ChipID_ENEMY, ChipGP>;

//�����o�Q�Ɨp
#define lpMapCtl MapCtl::GetInstance()

#define LDR_ID_NAME "LDR_MAP_DATA"
#define LDR_ID_VER 0x01

struct DataHeader
{
	char fileID[13];			//�t�@�C����ID���
	unsigned char verID;		//�t�@�C���̃o�[�W�������
	unsigned char Reserve1[2];	//�A���C�����g���\��̈�Ƃ���
	int sizeX;					//�}�b�v�̉��T�C�Y
	int sizeY;					//�}�b�v�̏c�T�C�Y
	unsigned char Reserve2[3];	//�A���C�����g���\��̈�Ƃ���
	unsigned char sum;			//SUM�l
};

class MapCtl
{
public:
	static void Create(SceneID sceneID)
	{
		if (s_instance == nullptr)
		{
			s_instance = new MapCtl(sceneID);
		}
	}
	static void Destroy(void)
	{
		if (s_instance != nullptr)
		{
			delete s_instance;
		}
		s_instance = nullptr;
	}
	static MapCtl& GetInstance()
	{
		//Create();
		return (*s_instance);
	}

	bool SetMapData(Vector2 pos, ChipID id, ChipGP chipgp);	//�}�b�v�f�[�^�̃Z�b�g pos-���W�i�}�X�ڂł͂Ȃ��j
	bool SetMapData(Vector2 pos, ChipID id);				//�}�b�v�f�[�^�̃Z�b�g pos-���W�i�}�X�ڂł͂Ȃ��j
	bool SetMapData(Vector2 pos, ChipID_ENEMY eneid, ChipGP chipgp);		//�G�̃}�b�v�f�[�^�̃Z�b�g
	bool SetMapData(Vector2 pos, ChipID_ENEMY eneid);

	ChipID GetMapDataGame(ChipGP gpID, unsigned int data);
	ChipID GetMapDataEdit(ChipGP gpID, unsigned int data);		//�w��f�[�^����CHIPID�����o���BgpID-�擾�������u���b�N�̃O���[�v�Bdata-�w�肵���}�X�ڏ��,ChipID-���̃}�X�̏��
	ChipID GetMapData(ChipGP gpID, int x, int y, bool dig);	//�w��}�X�ڂ���f�[�^�����o���Ă���CHIPID�����o�� dig-dig�`�F�b�N�����邩�ۂ�
	ChipID GetMapData(ChipGP gpID, Vector2 pos);			//�w����W����f�[�^�����o���Ă���ACHIPID�����o��

	ChipID_ENEMY GetMapDataEne(ChipGP gpID, int x, int y);

	void SetCamera(std::weak_ptr<Camera> camera);		//�J�����̍��W���Z�b�g

	bool MapSave();						//�}�b�v�̃Z�[�u
	bool MapLoad();						//�}�b�v�̃��[�h(�G�f�B�b�g�V�[���p)
	bool MapLoad(BaseScene& scene);		//�}�b�v�̃��[�h(�Q�[���V�[���p)

	std::function<ChipID(ChipGP, unsigned int)> GetMapDataFunc;		//�ǂ����GetMapData���g����
	std::function<void(void)> DrawFunc;		//�V�[�����Ƃ̕`��ϐ�

private:
	void DrawEdit();		//�G�f�B�b�g�V�[���p��draw�֐�
	void DrawGame();		//�Q�[���V�[���p��Draw�֐�
	void DrawMap();			//�Q�[���V�[����������drawedit�֐��œ����I�ɌĂяo��
	void DrawEditLine();	//�G�f�B�b�g�V�[���p�r��

	MapCtl(SceneID sceneID);
	~MapCtl();

	bool Init();
	bool InitMap();
	void InitDrawChipEdit();		//DrawChip�̏������i�G�f�B�b�g�V�[���p�j
	void InitDrawChippGame();		//DrawChip�̏������i�Q�[���V�[���p�j

	static MapCtl* s_instance;
	SceneID sceneID_;

	Vector2 viewArea_;		//���ʂɕ\�������}�b�v�̍L��(�`�b�v��)
	Vector2 worldArea_;		//�Q�[���̍L���i�`�b�v���j
	Vector2 chipSize_;		//�`�b�v�T�C�Y�i���W�j

	//id�̎w��Ŋ֐����Ăяo����
	std::map<ChipID, std::function<void(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)>> DrawChip_;
	std::map<ChipID_ENEMY, std::function<void(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)>> DrawChipEne_;
	//std::vector< std::vector<unsigned int>> mapID_;

	std::vector<unsigned int> basemapID_;		//�ԐڎQ�Ɣz�񂷂ׂ�
	std::vector<unsigned int*> mapID_;			//�ԐڎQ�Ɨp(�񎟌��z��̏c)

	std::weak_ptr<Camera> camera_;

	MapChipGP chipGP_;		//id�̃u���b�N�̃O���[�v�𔻕�
	MapChipEneGP chipeneGP_;

	int animCnt_;			//�A�j���[�V�����J�E���g

	friend class Camera;
	friend class GameScene;
};
