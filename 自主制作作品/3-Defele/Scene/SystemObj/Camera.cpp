#include <algorithm>
#include "Camera.h"
#include "../SceneMng.h"
#include "../../common/ImageMng.h"
#include "Cursol.h"
#include "../Deploy/DeployMng.h"
#include "../SystemObj/InputString.h"
#include "../../_debug/_DebugConOut.h"

//カメラの移動可能領域量
constexpr int MOVABLEAREA_X = 100;
constexpr int MOVABLEAREA_Y = 120;

//移動量
constexpr int AMOUNTOFMOVE = 14;

constexpr double TO_TARGET_MOVE_TIME = 1.2;

Camera::Camera()
{
	//カメラの初期座標を中心
	Vector2 harfViewArea = (lpSceneMng.GetViewArea() / 2);
	pos_.x_ = harfViewArea.x_;
	pos_.y_ = harfViewArea.y_;

	outofRangeFlag_ = false;

	breakedCoreFlag_ = false;
	time_ = 0.0;

	//画像の読み込み
	lpImageMng.GetID("Resource/image/UI/allow.png", "AllowRight");
	lpImageMng.GetID("Resource/image/UI/allow2.png", "AllowLeft");

}

Camera::~Camera()
{
}

bool Camera::InitArea(void)
{
	//移動制限登録
	Vector2 harfViewArea = (lpSceneMng.GetViewArea() / 2);

	antiMoveRect_.left = harfViewArea.x_;
	antiMoveRect_.top = harfViewArea.y_;
	antiMoveRect_.right = max((lpSceneMng.GetWorldSize().x_ - lpSceneMng.GetViewArea().x_ / 2), harfViewArea.x_);
	antiMoveRect_.bottom = max((lpSceneMng.GetWorldSize().y_ - lpSceneMng.GetViewArea().y_ / 2), harfViewArea.y_);

	//座標の初期化
	pos_ = harfViewArea;

	//初期座標の登録
	defpos_ = harfViewArea;

	return false;
}

bool Camera::SetTarget(std::weak_ptr<Cursol> target)
{
	//ターゲットの空白チェック
	if (target.expired())
	{
		return false;
	}
	target_ = target;

	//初期化
	InitArea();

	//カメラの座標をターゲットに合わせる
	pos_.x_ = std::clamp(
		target_.lock()->GetPos().x_,
		static_cast<int>(antiMoveRect_.left),
		static_cast<int>(antiMoveRect_.right));
	pos_.y_ = std::clamp(
		target_.lock()->GetPos().y_,
		static_cast<int>(antiMoveRect_.top),
		static_cast<int>(antiMoveRect_.bottom));

	return true;
}

void Camera::UpDate(void)
{
	if (breakedCoreFlag_)
	{
		UpDateCoreBreaked();
		return;
	}
	//ギミック表示時カメラ移動をさせない
	if (lpDeployMng.GetShowBoxFlag())
	{
		return;
	}

	//上下120は移動不可
	if (target_.lock()->GetPos().y_ < MOVABLEAREA_Y || lpSceneMng.GetViewArea().y_ - MOVABLEAREA_Y < target_.lock()->GetPos().y_)
	{
		outofRangeFlag_ = true;
		return;
	}
	outofRangeFlag_ = false;

	if (!target_.expired())
	{
		//カーソルの座標が範囲内ならカメラの移動

		//右移動
		if (lpSceneMng.GetViewArea().x_ - MOVABLEAREA_X < target_.lock()->GetPos().x_)
		{
			pos_.x_ = std::clamp(
				pos_.x_ + AMOUNTOFMOVE,
				static_cast<int>(antiMoveRect_.left),
				static_cast<int>(antiMoveRect_.right));
		}
		else
		{
			//左移動
			if (target_.lock()->GetPos().x_ < MOVABLEAREA_X)
			{
				pos_.x_ = std::clamp(
					pos_.x_ - AMOUNTOFMOVE,
					static_cast<int>(antiMoveRect_.left),
					static_cast<int>(antiMoveRect_.right));

			}
		}

		////上移動
		//if (lpSceneMng.GetViewArea().y_ - MOVABLEAREA_X < target_.lock()->GetPos().y_)
		//{
		//	pos_.y_ = std::clamp(
		//		pos_.y_ + AMOUNTOFMOVE,
		//		static_cast<int>(antiMoveRect_.top),
		//		static_cast<int>(antiMoveRect_.bottom));
		//}
		//else
		//{
		//	//下移動
		//	if (target_.lock()->GetPos().y_ < MOVABLEAREA_X)
		//	{
		//		pos_.y_ = std::clamp(
		//			pos_.y_ - AMOUNTOFMOVE,
		//			static_cast<int>(antiMoveRect_.top),
		//			static_cast<int>(antiMoveRect_.bottom));

		//	}
		//}

		//SetPos(target_.lock()->GetPos());
	}
}

void Camera::UpDateTutorial(void)
{
	if (lpInputString.GetNowPlace() == PLACE::ENEMYPLANT)
	{
		//右移動
		pos_.x_ = std::clamp(pos_.x_ + AMOUNTOFMOVE, static_cast<int>(antiMoveRect_.left), static_cast<int>(antiMoveRect_.right));
	}
	else
	{
		//左移動
		pos_.x_ = std::clamp(pos_.x_ - AMOUNTOFMOVE, static_cast<int>(antiMoveRect_.left), static_cast<int>(antiMoveRect_.right));
	}
}

void Camera::Draw(void)
{
	if (!outofRangeFlag_)
	{
		//範囲内
		int alpha = 255;
		if (lpInputString.GetNowPlace() != PLACE::MAX)
		{
			alpha = lpSceneMng.alpha_;
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		return;
	}

	auto viewArea = lpSceneMng.GetViewArea();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	//右側
	if (target_.lock()->GetPos().x_ > viewArea.x_ - MOVABLEAREA_X)
	{ 
		if (pos_.x_ == static_cast<int>(antiMoveRect_.right))
		{
			//移動不可のとき
			return;
		}

		DrawModiGraph(viewArea.x_ - MOVABLEAREA_X, MOVABLEAREA_Y,
			viewArea.x_, MOVABLEAREA_Y,
			viewArea.x_, viewArea.y_ - MOVABLEAREA_Y,
			viewArea.x_ - MOVABLEAREA_X, viewArea.y_ - MOVABLEAREA_Y,
			lpImageMng.GetID("AllowRight")[0], true);
	}

	//左側
	if (target_.lock()->GetPos().x_ < MOVABLEAREA_X)
	{
		if (pos_.x_ == static_cast<int>(antiMoveRect_.left))
		{
			//移動不可のとき
			return;
		}
		DrawModiGraph(0, MOVABLEAREA_Y,
			MOVABLEAREA_X, MOVABLEAREA_Y,
			MOVABLEAREA_X, viewArea.y_ - MOVABLEAREA_Y,
			0, viewArea.y_ - MOVABLEAREA_Y,
			lpImageMng.GetID("AllowLeft")[0], false);
	}

	int alpha = 255;
	if (lpInputString.GetNowPlace() != PLACE::MAX)
	{
		alpha = lpSceneMng.alpha_;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
}

const Vector2& Camera::GetPos()
{
	return pos_;
}

void Camera::SetPos(const Vector2& pos)
{
	pos_.x_ = std::clamp(pos.x_, static_cast<int>(antiMoveRect_.left), static_cast<int>(antiMoveRect_.right));
	pos_.y_ = std::clamp(pos.y_, static_cast<int>(antiMoveRect_.top), static_cast<int>(antiMoveRect_.bottom));
}

void Camera::SetCoreBreakedMode(Vector2 targetPos)
{
	breakedCorePos_.x_ = std::clamp(targetPos.x_, static_cast<int>(antiMoveRect_.left), static_cast<int>(antiMoveRect_.right));
	breakedCorePos_.y_ = std::clamp(targetPos.y_, static_cast<int>(antiMoveRect_.top), static_cast<int>(antiMoveRect_.bottom));

	beforePos_ = pos_;

	lpSceneMng.doubleValue_ = 1.0;

	breakedCoreFlag_ = true;
}

void Camera::UpDateCoreBreaked(void)
{
	if (!breakedCoreFlag_)
	{
		return;
	}

	int distance = breakedCorePos_.x_ - beforePos_.x_;
	pos_.x_ = beforePos_.x_ + static_cast<int>((time_ / TO_TARGET_MOVE_TIME) * distance);

	time_ = min(time_ + lpSceneMng.GetDeltaTime(), TO_TARGET_MOVE_TIME);
}

const Vector2 Camera::GetDrawOffset()
{
	return defpos_ - pos_;
}
