#pragma once
#include <DxLib.h>
#include <string>
#include <functional>
#include "../Easing.h"
#include "../ImageMng.h"
#include "../AudioMng.h"
#include "../Vector2.h"
#include "../../Scene/SceneMng.h"

enum class MoveType
{
	OUTEXP
};

enum class MoveDir
{
	//移動方向
	NONE,
	X,
	Y,
	XY
};

template <class Rtn, class... Args> class MoveObject
{
public:
	MoveObject(Vector2 startPos, Vector2 endPos, MoveType moveType,double totalMoveTime, std::string modelKey , std::function<Rtn(Args...)> endFunction);
	MoveObject(Vector2 startPos, Vector2 endPos, MoveType moveType,double totalMoveTime, std::string modelKey , 
		const std::string& soundKey, double soundStartTime_, std::function<Rtn(Args...)> endFunction);
	~MoveObject();

	std::function<Rtn(Args...)> Update(void);
	void Draw(void);

private:

	//動作タイプから関数を設定
	void SetFunctionByType(MoveType moveType);
	//移動方向の設定
	void SetMoveDir(void);

	Vector2 startPos_;
	Vector2 endPos_;
	Vector2 distance_;
	MoveDir moveDir_;
	double totalMoveTime_;
	double loadingTime_;

	std::string modelKey_;
	std::string soundKey_;
	double soundStartTime_;
	bool oneSoundFlag_;

	Vector2 pos_;
	Vector2 size_;

	//返り値のとなる処理
	std::function<Rtn(Args...)> endFunction_;

	//移動用関数
	std::function<double(double, double)> moveFunction_;
};

using MoveObjectBoolVoid = MoveObject<bool>;
using MoveObjectVoidVoid = MoveObject<void>;

template<class Rtn, class ...Args>
MoveObject<Rtn, Args...>::MoveObject(Vector2 startPos, Vector2 endPos, MoveType moveType, double totalMoveTime, std::string modelKey, std::function<Rtn(Args...)> endFunction)
{
	pos_ = startPos;
	startPos_ = startPos;
	endPos_ = endPos;

	distance_ = endPos_ - startPos_;
	distance_.x_ = abs(distance_.x_);
	distance_.y_ = abs(distance_.y_);

	loadingTime_ = 0.0;
	totalMoveTime_ = totalMoveTime;
	modelKey_ = modelKey;

	soundKey_ = "";
	soundStartTime_ = 10000.0;
	oneSoundFlag_ = false;

	endFunction_ = endFunction;

	SetMoveDir();
	SetFunctionByType(moveType);
}

template<class Rtn, class ...Args>
inline MoveObject<Rtn, Args...>::MoveObject(Vector2 startPos, Vector2 endPos, MoveType moveType, double totalMoveTime, std::string modelKey, const std::string& soundKey, double soundStartTime, std::function<Rtn(Args...)> endFunction)
{
	pos_ = startPos;
	startPos_ = startPos;
	endPos_ = endPos;

	distance_ = endPos_ - startPos_;
	distance_.x_ = abs(distance_.x_);
	distance_.y_ = abs(distance_.y_);

	loadingTime_ = 0.0;
	totalMoveTime_ = totalMoveTime;
	modelKey_ = modelKey;

	soundKey_ = soundKey;
	soundStartTime_ = soundStartTime;
	oneSoundFlag_ = false;

	endFunction_ = endFunction;

	SetMoveDir();
	SetFunctionByType(moveType);
}

template<class Rtn, class ...Args>
MoveObject<Rtn, Args...>::~MoveObject()
{
}

template<class Rtn, class ...Args>
std::function<Rtn(Args...)> MoveObject<Rtn, Args...>::Update(void)
{
	loadingTime_ += lpSceneMng.GetDeltaTime();

	if (!oneSoundFlag_)
	{
		if (soundStartTime_ < loadingTime_)
		{
			PlaySoundMem(lpAudioMng.GetID(soundKey_), DX_PLAYTYPE_BACK);
			oneSoundFlag_ = true;
		}
	}

	switch (moveDir_)
	{
	case MoveDir::NONE:
		//削除
		return endFunction_;
		break;
	case MoveDir::X:
		pos_.x_ = startPos_.x_ + static_cast<int>(moveFunction_(loadingTime_, totalMoveTime_) * distance_.x_);
		break;
	case MoveDir::Y:
		pos_.y_ = startPos_.y_ + static_cast<int>(moveFunction_(loadingTime_, totalMoveTime_) * distance_.y_);
		break;
	case MoveDir::XY:
		pos_ = startPos_ + Vector2(static_cast<int>(moveFunction_(loadingTime_, totalMoveTime_)), static_cast<int>(moveFunction_(loadingTime_, totalMoveTime_))) * distance_;
		break;
	default:
		break;
	}

	//ダブル型で計算しているため誤差回収
	if (pos_.y_ - 4 < endPos_.y_ && endPos_.y_ < pos_.y_ + 4)
	{
		return endFunction_;
	}

	//空の処理を返す
	return []() {return Rtn(); };
}

template<class Rtn, class ...Args>
void MoveObject<Rtn, Args... > ::Draw(void)
{
	DrawGraph(pos_.x_, pos_.y_, lpImageMng.GetID(modelKey_)[0], true);
}

template<class Rtn, class ...Args>
void MoveObject<Rtn, Args...>::SetFunctionByType(MoveType moveType)
{
	if (moveType == MoveType::OUTEXP)
	{
		moveFunction_ = [](double time, double totalTime) { return Easing::OUTEXP(time, totalTime); };
	}
}

template<class Rtn, class ...Args>
void MoveObject<Rtn, Args...>::SetMoveDir(void)
{
	moveDir_ = MoveDir::NONE;

	if (startPos_.x_ != endPos_.x_)
	{
		moveDir_ = MoveDir::X;
		if (startPos_.y_ != endPos_.y_)
		{
			moveDir_ = MoveDir::XY;
			return;
		}
		return;
	}
	if (startPos_.y_ != endPos_.y_)
	{
		moveDir_ = MoveDir::Y;
	}
}

