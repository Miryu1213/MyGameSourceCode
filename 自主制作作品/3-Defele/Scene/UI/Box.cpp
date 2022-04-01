#include <DxLib.h>
#include <string>
#include "Box.h"
#include "../../common/AudioMng.h"

Box::Box(Vector2&& pos, Vector2&& size, MouseInputID mouseid, std::string str)
{
	pos_ = pos;
	size_ = size;
	str_ = str;

	mouseId_ = mouseid;

	if (!lpAudioMng.IsKey("cursol"))
	{
		lpAudioMng.GetID("Resource/Audio/SE/Button/cursol.mp3", "cursol");
	}
}

Box::Box(Vector2&& pos, Vector2&& size, std::function<bool(void)> function, MouseInputID mouseid, std::string str)
{
	pos_ = pos;
	size_ = size;
	str_ = str;
	function_ = function;

	mouseId_ = mouseid;

	if (!lpAudioMng.IsKey("cursol"))
	{
		lpAudioMng.GetID("Resource/Audio/SE/Button/cursol.mp3", "cursol");
	}
}

Box::Box(Vector2&& pos, Vector2&& size, MouseInputID mouseid, std::function<bool(int)> function, ObjType objType)
{
	pos_ = pos;
	size_ = size;
	function2_ = function;

	mouseId_ = mouseid;
	objType_ = objType;

	if (!lpAudioMng.IsKey("cursol"))
	{
		lpAudioMng.GetID("Resource/Audio/SE/Button/cursol.mp3", "cursol");
	}
}

Box::Box(Vector2&& pos, Vector2&& size, const char* mapname, MouseInputID mouseid, std::string str)
{
	pos_ = pos;
	size_ = size;
	str_ = str;
	mapName_ = const_cast<char*>(mapname);

	mouseId_ = mouseid;

	if (!lpAudioMng.IsKey("cursol"))
	{
		lpAudioMng.GetID("Resource/Audio/SE/cursol.mp3", "cursol");
	}
}

Box::~Box()
{
}

void Box::Update(void)
{
}

void Box::Draw(bool frame, int fontsize, std::string str)
{
	bool substitution = false;
	if (str_ == "x")
	{
		//������x�������Ƃ��ɑ��
		str_ = str_ + str;
		substitution = true;
	}

	if (frame)
	{
		int offset = 1;

		DrawBox(pos_.x_ - offset, pos_.y_ - offset, pos_.x_ + size_.x_ + offset, pos_.y_ + offset, 0xffffff, true);
		DrawBox(pos_.x_ - offset, pos_.y_ + size_.y_ - offset, pos_.x_ + size_.x_ + offset, pos_.y_ + size_.y_ + offset, 0xffffff, true);

		DrawBox(pos_.x_ - offset, pos_.y_ - offset, pos_.x_ + offset, pos_.y_ + size_.y_ + offset, 0xffffff, true);
		DrawBox(pos_.x_ + size_.x_ - offset, pos_.y_ - offset, pos_.x_ + size_.x_ + offset, pos_.y_ + size_.y_ + offset, 0xffffff, true);
	}

	if (modelKey_ == -1)
	{
		DrawBox(pos_.x_, pos_.y_, pos_.x_ + size_.x_, pos_.y_ + size_.y_, 0x000000, true);
	}
	else
	{
		//�O���t�B�b�N�n���h�����Z�b�g����Ă����
		DrawModiGraph(pos_.x_, pos_.y_, pos_.x_ + size_.x_, pos_.y_, pos_.x_ + size_.x_, pos_.y_ + size_.y_, pos_.x_, pos_.y_ + size_.y_, modelKey_, true);
	}

	//�����T�C�Y�擾
	auto fontHandle = lpFontMng.GetFontSizeHandle(FontType::DEFAULT, fontsize);
	int StringWidth = GetDrawStringWidthToHandle(str_.c_str(), static_cast<int>(strlen(str_.c_str())), fontHandle);
	
	DrawFormatStringToHandle(pos_.x_ + (size_.x_ - StringWidth) / 2, pos_.y_ + (size_.y_ - fontsize) / 2, 0xffffff, fontHandle, str_.c_str());

	if (substitution)
	{
		//�������x�ɖ߂�
		str_ = "x";
	}
}

bool Box::CheckColition(Vector2 otherpos)
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
