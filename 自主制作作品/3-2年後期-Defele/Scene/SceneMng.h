#pragma once
#include"BaseScene.h"

//lp�@LongPtr
#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	//�ÓI�ȃV���O���g��
	static SceneMng& GetInstance(void)
	{
		static SceneMng s_Instance;
		return s_Instance;
	};

	//�F
	constexpr static UINT32 Blue = 0x0000ff;
	constexpr static UINT32 Red = 0xff0000;
	constexpr static UINT32 Black = 0x000000;
	constexpr static UINT32 Purple = 0xff00ff;

	void Run(void);

	//�o�ߎ��Ԃ̎擾
	double GetDeltaTime(void)
	{
		return delta_ * doubleValue_;
	}

	//���v�o�ߎ��Ԃ̎擾
	double GetTotalDeltaTime(void)
	{
		return totaldelta_;
	}

	//�\���G���A
	Vector2 GetViewArea(void) 
	{ 
		return viewArea_;
	}

	//�S�̃T�C�Y
	Vector2 GetWorldSize(void) 
	{ 
		return worldSize_;
	}
	void SetWorldSize(Vector2 worldsize);

	//�I�t�Z�b�g
	int GetOffset(void) 
	{
		return offset_;
	}
	//�I�t�Z�b�g���Z�ς݃J�[�\���̍��W��Ԃ�(�J�����̃I�t�Z�b�g�ł͂Ȃ�)
	Vector2 GetCurPosAddOffset(void);

	void FinishGame(void)
	{
		gameEndFlag_ = true;
	}

	//�J�[�\��(���͋@��܂Ƃ�)
	std::shared_ptr<Cursol> cursol_;

	//�{��
	double doubleValue_;
	int alpha_;

	//�}�b�v��
	std::map<std::string, bool> clearMap_;
	
private:
	SceneMng();
	~SceneMng();

	//�V�X�e���n������
	bool SysInit(void);

	bool Init(void);
	void UpDate(void);
	void Draw(void);

	//�`��͈�
	Vector2 viewArea_;
	//�}�b�v�S�̂̃T�C�Y
	Vector2 worldSize_;
	//��ʂ��炵�p�I�t�Z�b�g
	int offset_;

	//�V�[���̐錾
	UniqueScene scene_;

	//�o�ߎ���
	double delta_;

	//�V�[���J�n�ォ��̌o�ߎ���
	double totaldelta_;

	bool gameEndFlag_;
};
