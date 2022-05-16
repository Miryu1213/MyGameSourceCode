#include <DxLib.h>
#include <string>
#include "Square.h"
#include "../../common/AudioMng.h"

Square::Square(Vector2&& leftUpPos, Vector2&& leftDownPos, Vector2&& rightUpPos, Vector2&& rightDownPos,
	Vector2&& size, const char* mapname, MouseInputID mouseid, std::string str)
{
	leftUpPos_ = leftUpPos;
	leftDownPos_ = leftDownPos;
	rightUpPos_ = rightUpPos;
	rightDownPos_ = rightDownPos;

	pos_ = leftUpPos_;
	size_ = size;
	str_ = str;
	mapName_ = const_cast<char*>(mapname);

	mouseId_ = mouseid;

	if (!lpAudioMng.IsKey("cursol"))
	{
		lpAudioMng.GetID("Resource/Audio/SE/cursol.mp3", "cursol");
	}
}

Square::~Square()
{
}

void Square::Draw(bool frame, int fontsize, std::string str)
{
	if (modelKey_ == -1)
	{
		DrawBox(pos_.x_, pos_.y_, pos_.x_ + size_.x_, pos_.y_ + size_.y_, 0x000000, true);
	}
	else
	{
		//グラフィックハンドルがセットされている間
		DrawModiGraph(leftUpPos_.x_, leftUpPos_.y_,
			rightUpPos_.x_, rightUpPos_.y_,
			rightDownPos_.x_, rightDownPos_.y_,
			leftDownPos_.x_, leftDownPos_.y_,
			modelKey_, true);
	}

	//文字サイズ取得
	auto fontHandle = lpFontMng.GetFontSizeHandle(FontType::DEFAULT, fontsize);
	int StringWidth = GetDrawStringWidthToHandle(str_.c_str(), static_cast<int>(strlen(str_.c_str())), fontHandle);

	DrawFormatStringToHandle(pos_.x_ + (size_.x_ - StringWidth) / 2, pos_.y_ + (size_.y_ - fontsize) / 2, 0xffffff, fontHandle, str_.c_str());
}

bool Square::CheckColition(Vector2 otherpos)
{
	if (pos_ <= otherpos && otherpos <= pos_ + size_)
	{
		if (!IsMakeSE_)
		{
			PlaySoundMem(lpAudioMng.GetID("cursol"), DX_PLAYTYPE_BACK);
			IsMakeSE_ = true;
		}
		return true;
	}

	IsMakeSE_ = false;

	return false;
}
