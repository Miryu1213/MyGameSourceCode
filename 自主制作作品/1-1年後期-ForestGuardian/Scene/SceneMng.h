#pragma once
#include<memory>
#include"BaseScene.h"
#include"../common/vector2.h"

#define lpSceneMng SceneMng::GetInstance()
class SceneMng		//�S�̂̃V�[���̊Ǘ��B�������[�v�B�e�T�C�Y��
{
public:
	static SceneMng& GetInstance(void)
	{
		//���̎�����static�ŃO���[�o������,�������^�[���ŕԂ����ƂŃC���X�^���X���Ǐ��ŗ}������B
		static SceneMng s_Instance;
		return s_Instance;
	}

	void Run();
	void Draw();

	bool outkey;

	const Vector2 scnArea_;			//��ʉ𑜓x
	const Vector2 worldArea_;		//�Q�[�����E�S�̃h�b�g
	const Vector2 viewArea_;		//�\���h�b�g��
	const Vector2 PlayerSize_;		//�v���C���[�T�C�Y
private:
	uniqueBaseScene scene_;		//�e�V�[���̊Ǘ��ϐ�

	void Init();
	bool SysInit(void);		//DXlib�Ƃ����̕ӂ̏�����
	SceneMng();
	~SceneMng();
};


