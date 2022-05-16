#include <DxLib.h>
#include "UI.h"
#include "../../common/AudioMng.h"

UI::UI()
{
	modelKey_ = -1;

	IsMakeSE_ = false;

	if (!lpAudioMng.IsKey("dec"))
	{
		lpAudioMng.GetID("Resource/Audio/SE/Button/dec.mp3", "dec");
	}
}

UI::~UI()
{
}

void UI::SetPos(Vector2 pos)
{
	pos_ = pos;
}

bool UI::CheckButtonInput(int inputid, int oldinputid)
{
	if (inputid && !oldinputid)
	{
		PlaySoundMem(lpAudioMng.GetID("dec"), DX_PLAYTYPE_BACK);
		return true;
	}
	return false;
}
