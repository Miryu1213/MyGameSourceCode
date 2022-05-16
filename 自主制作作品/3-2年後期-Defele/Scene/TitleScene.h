#pragma once
#include <functional>
#include <memory>
#include <map>
#include <list>
#include "BaseScene.h"

class UI;
class Circle;

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();

	bool Init(void) override;
	UniqueScene UpDate(UniqueScene ownScene) override;
	void Draw(void) override;

	Scene GetSceneID(void) override
	{
		return Scene::TITLE;
	};

private:

	//���݂̉��
	enum class CURSCREEN
	{
		NONE,
		START,
		FADE,
		BURN,
		FADEUI,
		DEFAULT,
		CREDIT
	};

	void UpdateDefault(void);
	void UpdateCredit(void);

	void DrawFirstImage(void);	//������ʁi1�j
	void DrawFadeImage(void);	//�t�F�[�h��ʁi2�j
	void DrawBurn(void);		//�����ʁi3�j
	void DrawFadeUI(void);		//�t�F�[�hUI��ʁi4�j
	void DrawDefault(void);		//�ʏ탁�j���[��ʁi5�j
	void DrawCredit(void);		//�N���W�b�g���

	CURSCREEN curScreen_;		//���݂̃X�N���[��

	//�e��ʂ̍X�V
	std::map<CURSCREEN, std::function<void(void)>> update_;
	std::map<CURSCREEN, std::function<void(void)>> draw_;

	//���j���[��ʂł̃{�^�����X�g
	std::list<std::unique_ptr<UI>> btnList_;

	double time_;		//���ԊǗ��p�^�C��
	bool musicFlag_;	//���y�Đ�
	bool toSelectFlag_;	//�Z���N�g�V�[����

	//�N���W�b�g
	std::unique_ptr<UI> closingCredit_;		//�N���W�b�gUI
	
	//���j���[�{�^��
	Vector2 buttonPos_;
	Vector2 buttonDefPos_;

	//���S
	Vector2 logoPos_;
	Vector2 logoDefPos_;
};

