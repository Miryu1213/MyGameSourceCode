#include <DxLib.h>
#include <cmath>
#include "Circle.h"

Circle::Circle(Vector2&& pos, Vector2&& size, MouseInputID mouseid, const char* str)
{
	pos_ = pos;
	size_ = size;
	str_ = str;

	mouseId_ = mouseid;
}

Circle::Circle(Vector2&& pos, Vector2&& size, std::function<bool(void)> function, MouseInputID mouseid, const char* str)
{
	pos_ = pos;
	size_ = size;
	str_ = str;
	function_ = function;

	mouseId_ = mouseid;
}

Circle::~Circle()
{
}

void Circle::Update(void)
{
}

void Circle::Draw(bool frame, int fontsize, std::string str)
{
	if (str_ == "" && str != "")
	{
		//空白時代入
		str_ = str;
	}

	if (modelKey_ == -1)
	{
		DrawCircle(pos_.x_, pos_.y_, size_.x_ / 2, 0xffffff, true);
	}
	else
	{
		//グラフィックハンドルがセットされている間
		DrawModiGraph(pos_.x_ - size_.x_ / 2, pos_.y_ - size_.y_ / 2,
			pos_.x_ + size_.x_ - size_.x_ / 2, pos_.y_ - size_.y_ / 2,
			pos_.x_ + size_.x_ - size_.x_ / 2, pos_.y_ + size_.y_ - size_.y_ / 2,
			pos_.x_ - size_.x_ / 2, pos_.y_ + size_.y_ - size_.y_ / 2, modelKey_, true);
	}

	if (frame)
	{
		DrawCircle(pos_.x_, pos_.y_, size_.x_ / 2 - 1, 0xffffff, false);
		DrawCircle(pos_.x_, pos_.y_, size_.x_ / 2, 0xffffff, false);
	}

	//文字サイズ取得
	auto fontHandle = lpFontMng.GetFontSizeHandle(FontType::DEFAULT, fontsize);
	int StringWidth = GetDrawStringWidthToHandle(str_.c_str(), static_cast<int>(strlen(str_.c_str())), fontHandle);
	DrawFormatStringToHandle(pos_.x_ + (size_.x_ - StringWidth) / 2, pos_.y_ + size_.y_ / 2, true, fontHandle, str_.c_str());
}

bool Circle::CheckColition(Vector2 otherpos)
{
	Vector2 distance = otherpos - pos_;
	if (size_.x_ / 2 > sqrt(distance.x_ * distance.x_ + distance.y_ * distance.y_))
	{
		return true;
	}
	return false;
}
