#pragma once
#include<DxLib.h>
#include<memory>
#include"../common/vector2.h"

class obj;

class Camera
{
public:
	Camera();
	~Camera();
	bool InitArea();
	bool SetTarget(std::weak_ptr<obj> target);		//�J�����̑Ώۂ��Z�b�g
	void UpDate();

	void Draw();

	const Vector2& GetPos();
	void SetPos(const Vector2& pos);
	const Vector2 GetDrawOffset();		//�`��ƃJ�����ʒu�ɂ���Ă����ʂ̎擾
private:
	std::weak_ptr<obj> target_;
	Vector2 pos_;
	Vector2 oldpos_;

	Vector2 defpos_;			//�������W
	RECT antiMoveRect_;			//�J�����̈ړ��͈�
};


