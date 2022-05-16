#pragma once
#include <DxLib.h>
#include <memory>
#include "../../common/vector2.h"

class Cursol;

class Camera
{
public:
	Camera();
	~Camera();

	//�`��ʒu�ݒ�
	bool InitArea(void);

	//�J�����̑Ώۂ��Z�b�g
	bool SetTarget(std::weak_ptr<Cursol> target);

	void UpDate(void);
	//�`���[�g���A�����̍X�V
	void UpDateTutorial(void);

	void Draw(void);

	const Vector2& GetPos();
	void SetPos(const Vector2& pos);

	void SetCoreBreakedMode(Vector2 targetPos);
	void UpDateCoreBreaked(void);

	//�`��ƃJ�����ʒu�ɂ���Ă����ʂ̎擾
	const Vector2 GetDrawOffset();
private:
	std::weak_ptr<Cursol> target_;
	Vector2 pos_;

	Vector2 defpos_;			//�������W
	RECT antiMoveRect_;			//�J�����̈ړ��͈�

	//�͈͊O
	bool outofRangeFlag_;

	//�R�A�j��
	bool breakedCoreFlag_;
	Vector2 breakedCorePos_;
	Vector2 beforePos_;

	//����
	double time_;
};
