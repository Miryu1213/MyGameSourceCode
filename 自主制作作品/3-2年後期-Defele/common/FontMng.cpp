#include <DxLib.h>
#include "FontMng.h"

const int& FontMng::GetFontSizeHandle(FontType fonttype, int size)
{
	if (fontHandles[fonttype].find(size) == fontHandles[fonttype].end())
	{
		//�Ȃ��ꍇ�쐬
		if (fonttype == FontType::DEFAULT)
		{
			//�f�t�H���g�t�H���g
			fontHandles[fonttype][size] = CreateFontToHandle(NULL, size, -1, DX_FONTTYPE_NORMAL);
		}
	}
	return fontHandles[fonttype][size];
}

void FontMng::DeleteFontHandle(FontType fonttype, int size)
{
	if (fontHandles.find(fonttype) != fontHandles.end())
	{
		if (fontHandles[fonttype].find(size) != fontHandles[fonttype].end())
		{
			//�t�H���g�폜
			DeleteFontToHandle(fontHandles[fonttype][size]);
			fontHandles[fonttype].erase(size);
		}
	}
}

FontMng::FontMng()
{
}

FontMng::~FontMng()
{
}
