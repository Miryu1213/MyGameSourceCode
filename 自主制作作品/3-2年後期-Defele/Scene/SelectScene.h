#pragma once
#include <memory>
#include <functional>
#include <list>
#include <map>
#include "BaseScene.h"
#include "GameScene.h"
#include "UI/UI.h"
#include "tmx/MapObject.h"

class SelectScene :
	public BaseScene
{
public:
	SelectScene();
	~SelectScene();

	bool Init(void) override;
	UniqueScene UpDate(UniqueScene ownScene) override;
	void Draw(void) override;

	Scene GetSceneID(void) override
	{
		return Scene::SELECT;
	};

private:

	//���݂̉��
	enum class CURSCREEN
	{
		NONE,
		START,
		STARTEND,
		STAGESELECT,
		MAP,
		MAPMAGNI,
		MAPSHRINK,
		CONFIRM
	};

	//�������
	void UpdateStartScreen(void);
	void DrawStartScreen(void);
	//�ړ��I��
	void UpdateStartEndScreen(void);
	void DrawStartEndScreen(void);
	//�X�e�[�W�Z���N�g���
	void UpdateStgSelect(void);
	void DrawStgSelect(void);
	//�}�b�v�ڍ׉��
	void UpdateDetailedMap(void);
	void DrawDetailedMap(void);
	//�}�b�v�g��
	void UpdateMagniMap(void);
	void DrawMagniMap(void);
	//�}�b�v�k��
	void UpdateShrinkMap(void);
	void DrawShrinkMap(void);
	//�ŏI�m�F���
	void UpdateConfirm(void);
	void DrawConfirm(void);

	//�}�b�v�`��
	void DrawMap(void);

	void CreatePlantMenu(void);
	void DrawStatus(void);
	void DrawByObjType(Vector2 pos, ObjType objType);
	//�\�͕`��p��ID���Z�b�g
	void SetUnitActiveID(void);
	void SetID(ObjType objType, int startIdX, int startIdY, int divX, int frameSum);
	int CaliculateId(ObjType objType);

	//�S�Ẳ摜���[�h
	void LoadImageAll(void);

	//�������UI���X�g
	std::list<std::unique_ptr<UI>> startBtnList_;
	std::list<std::unique_ptr<UI>> stgSelectList_;

	//�}�b�v�\����UI���X�g
	std::list<std::unique_ptr<UI>> mapUiList_;
	std::list<std::unique_ptr<UI>> plantList_;
	int plantSum_;
	std::map<ObjType, std::vector<int>> unitIdMap_;
	double animTime_;

	//�}�b�v�g����UI���X�g
	std::list<std::unique_ptr<UI>> mapMagniUiList_;

	//�ŏI�m�F���UI���X�g
	std::list<std::unique_ptr<UI>> confirmUiList_;

	//�e��ʊ֐�
	std::map<CURSCREEN, std::function<void(void)>> update_;
	std::map<CURSCREEN, std::function<void(void)>> draw_;

	//�}�b�v�I�u�W�F�N�g�ꗗ
	DeployObjSum mapObj_;
	//���݂̃}�b�v�l�[��
	std::string mapName_;

	//�X�e�[�W�Z���N�g
	std::vector<Vector2> stgSelectPoses_;
	std::vector<double> moveTimes_;

	//���݂̃X�N���[��
	CURSCREEN curScreen_;
	bool toTitleFlag_;
	bool toNextSceneFlag_;

	//�}�b�v�g��b��
	double mapMagniTime_;

	//�}�b�v�n���h��
	int mapGh_;
	//�w�i�O���t�B�b�N�n���h��
	int backGroundGh_;
};

