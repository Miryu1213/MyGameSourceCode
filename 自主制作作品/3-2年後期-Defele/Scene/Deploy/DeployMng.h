#pragma once
#include <string.>
#include <memory>
#include <list>
#include "../Obj/Obj.h"
#include "Gimmick/BaseGimmick.h"
#include "../UI/Place.h"

#define lpDeployMng DeployMng::GetInstance()

class UI;

//���I�ȃV���O���g��
class DeployMng
{
public:
	static void Create(std::list<std::unique_ptr<Obj>>* objlist,int offset)
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new DeployMng(objlist,offset);
		}
	}
	static void Destroy(void)
	{
		if (s_Instance != nullptr)
		{
			delete s_Instance;
			s_Instance = nullptr;
		}
	}
	static DeployMng& GetInstance(void)
	{
		return (*s_Instance);
	}

	const UI* GetBox(void)
	{
		return box_;
	}

	const UI* GetCircle(void)
	{
		return circle_;
	}

	bool GetEntryFlag(void)
	{
		return entryPlantFlag_;
	}
	void SetEntryFlag(bool flag);

	ObjType GetPlantType(void)
	{
		return plantType_;
	}

	bool GetShowBoxFlag(void)
	{
		return showboxFlag_;
	}
	void SetShowBoxFlag(bool flag)
	{
		showboxFlag_ = flag;
	}

	Vector2 GetEntryPlantPos(void)
	{
		return entryPlantPos_;
	}

	//�莝���{�ݑS�̂̎擾
	const std::map<ObjType, int>& GetDeployPlant(void)
	{
		return plantSum_;
	}


	//���������̏�����
	bool Init(std::string mapname);

	void UpDate(Vector2 camoffset);
	void Draw(void);

	//UI�̕`��
	void DrawDeploy(SharedCamera& camera, bool isTutorial);
	//�����Ă�����̂�`��
	void DrawWhatPlayerHas(Vector2 camOffset,bool isTutorial);
	//�{�b�N�X��̕`��
	void DrawinBox(void);
	//����ʒu�ɓ���T�C�Y�ŕ`��
	void DrawindentGraph(Vector2 pos, Vector2 size, int gh);

	//�M�~�b�N�̃Z�b�g
	void SetGimmick(GimmickType gimmicktype, Vector2 pos, Vector2 size);

	//�M�~�b�N�����̏���
	void GrabFunction(Vector2 camoffset);

	//���ۈړ�
	void CircleMoveRight(void);
	void CircleMoveLeft(void);

	//�I�u�W�F�N�g���X�g
	std::list<std::unique_ptr<Obj>>* objList_;

	//�M�~�b�N�Ɖ摜�}�b�v
	std::map<GimmickType, std::vector<int>> gimmickGhMap_;

protected:
	//�v�����g�摜�}�b�v
	std::map<ObjType, int> plantGhMap_;
private:
	DeployMng(std::list<std::unique_ptr<Obj>>* objlist, int offset);
	~DeployMng();

	//�B��̃C���X�^���X
	static DeployMng* s_Instance;

	//��UI�̏���
	void DeployUIFunction(void);

	//�M�~�b�N�Ǘ�
	std::list<std::unique_ptr<BaseGimmick>> gimList_;

	//�\���n
	//���݂̎���
	double nowTime_;
	//�ړ��I���t���O
	bool moveFinishFlag_;
	//�x���p�t���O
	bool delayFlag_;

	//UI
	UI* circle_;
	UI* box_;

	//�ݒu���W
	Vector2 entryPlantPos_;
	//�v�����g�ݒu�t���O
	bool entryPlantFlag_;
	//�M�~�b�N����
	bool grabGimmickFlag_;
	//Box�\���t���O
	bool showboxFlag_;

	//���݂̑I���^�C�v
	GimmickType gimType_;
	ObjType plantType_;

	//�z�u�ł��鐔
	std::map<ObjType, int> plantSum_;
	std::map<GimmickType, int> gimmickSum_;

	//�{�ݔz�u�ʒu���v
	std::list<Vector2> plantPosList;
};
