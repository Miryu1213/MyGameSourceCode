#include <algorithm>
#include<DxLib.h>
#include"SceneMng.h"
#include "Camera.h"
#include"obj/obj.h"
#include"../_debug/_DebugDispOut.h"
#include"../_debug/_DebugConOut.h"

Camera::Camera()
{
	Vector2 tmpPos = (lpSceneMng.worldArea_ / 2);
	pos_.x = tmpPos.x;
	pos_.y = tmpPos.y;
}

Camera::~Camera()
{
}

bool Camera::InitArea()
{
	Vector2 tmpPos = (lpSceneMng.viewArea_ / 2);	//画面半分の座標（ブロック値ではない）
	antiMoveRect_.left = tmpPos.x;
	antiMoveRect_.top = tmpPos.y;
	antiMoveRect_.right = ((lpSceneMng.worldArea_.x - (lpSceneMng.viewArea_.x / 2)) - 4);
	antiMoveRect_.bottom = tmpPos.y;
	defpos_.x = antiMoveRect_.left;
	defpos_.y = antiMoveRect_.top;
	return false;
}

bool Camera::SetTarget(std::weak_ptr<obj> target)
{
	target_ = target;
	if (target_.expired())
	{
		return false;
	}
	InitArea();
	pos_.x = target_.lock()->GetPos().x;
	pos_.x = std::clamp(pos_.x, static_cast<int>(antiMoveRect_.left), static_cast<int>(antiMoveRect_.right));
	pos_.y = antiMoveRect_.top;
	oldpos_ = pos_;
	return true;
}

void Camera::UpDate()
{
	if (!target_.expired())
	{
		auto plpos = target_.lock()->GetPos();
		oldpos_ = pos_;

		if (oldpos_.x - plpos.x > 40)
		{
			//左移動
			if (plpos.x != pos_.x)
			{
				pos_.x -= 2;
			}
		}
		else if (oldpos_.x - plpos.x < -40)
		{
			//右移動
			if (plpos.x != pos_.x)
			{
				pos_.x += 2;
			}
		}
		else
		{
			pos_.x = target_.lock()->GetPos().x;
			//pos_.x = std::clamp(target_.lock()->GetPos().x, static_cast<int>(antiMoveRect_.left), static_cast<int>(antiMoveRect_.right));
		}
		pos_.x = std::clamp(pos_.x, static_cast<int>(antiMoveRect_.left), static_cast<int>(antiMoveRect_.right));
	}
}

void Camera::Draw()
{
	//auto offset = lpSceneMng.drawOffset_;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 130);
	DrawBox(pos_.x, pos_.y, pos_.x + 32, pos_.y + 32, 0x00ff00, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//TRACE("posx:%d\n", pos_.x);
}

const Vector2& Camera::GetPos()
{
	return pos_;
}

void Camera::SetPos(const Vector2& pos)
{
	pos_ = pos;
	pos_.x = std::clamp(pos_.x, static_cast<int>(antiMoveRect_.left), static_cast<int>(antiMoveRect_.right));
	pos_.y = std::clamp(pos_.y, static_cast<int>(antiMoveRect_.top), static_cast<int>(antiMoveRect_.bottom));
}

const Vector2 Camera::GetDrawOffset()
{
	return defpos_ - pos_;
}
