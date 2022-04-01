#pragma once
#include<memory>
#include<list>
#include"SceneID.h"
#include"Camera.h"
//#include"obj/obj.h"

class obj;
class BaseScene;
//���j�[�N�|�C���^-���Ŏ����ꂪ�Ǘ����Ă�����̂������ŏ���
using uniqueBaseScene = std::unique_ptr<BaseScene>;		//���j�[�N�|�C���^�Ƃ��ĊǗ�
using ObjPtr = std::shared_ptr<obj>;
using ObjList = std::list<ObjPtr>;		//obj�N���X���V�F�A�[�h�|�C���^�Ƃ��ĊǗ��A��������X�g��

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual uniqueBaseScene UpDate(uniqueBaseScene own) = 0;
	virtual SceneID GetSceneID() = 0;		//���݂̃V�[��ID�̎擾
	void Draw();
	void ExtendDraw(int firstX, int firstY, int lastX, int lastY, bool inv);	//�T�C�Y���w�肵�Ă̕`��
	virtual void DrawOwnScreen(void) = 0;

	ObjList::iterator AddobjList(ObjPtr&& objptr);		//objlist�ɃI�u�W�F�N�g��ǉ�
	ObjList objList_;		//�I�u�W�F�N�g�̈ꗗ
private:

protected:
	virtual bool Init(void) = 0;

	int screenScrID_;		//�e�V�[���ł̕`��p�n���h��

	std::shared_ptr<Camera> camera_;

	bool UpDateObj(void);		//�I�u�W�F�N�g�n�̍X�V

	bool DrawObj(void);
};



