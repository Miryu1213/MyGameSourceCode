#pragma once
#include <memory>
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
	void DrawDefault(void);
	void DrawStgSelect(void);
	//�}�b�v�̊g��
	bool MagniMap(void);
	void DrawMagniMap(void);
	//�}�b�v�̏k��
	void DrawDownUI(void);
	//�}�b�v�`����
	void DrawMapScreen(void);

	//�ŏI�m�F���
	bool ComfirmUi(void);
	void DrawComfirmUi(void);

	//�ʏ�
	bool DefMapUI(void);
	//�}�b�v�̒ʏ�`��
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

	//�}�b�v�k������UI
	bool MapDownUI(void);
	//�L�����Z��UI
	bool CancelUI(void);
	//�o��UI
	bool SetoffUI(void);

	//�ŏI�m�F
	//�L�����Z��
	bool CancelUI2(void);
	//�o��
	bool GoUI(void);

	//�������UI���X�g
	std::list<std::unique_ptr<UI>> defBtnList_;
	std::list<std::unique_ptr<UI>> defStageList_;

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

	//�}�b�v�I�u�W�F�N�g�ꗗ
	DeployObjSum mapObj_;
	//���݂̃}�b�v�l�[��
	std::string mapName_;

	std::vector<Vector2> stgSelectPoses_;
	bool moveEndFlag_;
	std::vector<double> moveTimes_;

	bool stgSelectFlag_;
	bool toTitleFlag_;

	//������̓t���O
	bool defScrEndFlag_;

	//�}�b�v�g��b��
	double mapMagniTime_;
	//�}�b�v�g��t���O
	bool mapMagniUpFlag_;
	//�}�b�v�k���t���O
	bool mapMagniDownFlag_;
	//�ŏI�m�F�t���O
	bool comScrFlag_;
	bool comEndFlag_;

	//�}�b�v�n���h��
	int mapGh_;

	//�w�i�O���t�B�b�N�n���h��
	int backGroundGh_;
};

