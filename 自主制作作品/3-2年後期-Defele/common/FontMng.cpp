#include <DxLib.h>
#include "FontMng.h"

const int& FontMng::GetFontSizeHandle(FontType fonttype, int size)
{
	if (fontHandles[fonttype].find(size) == fontHandles[fonttype].end())
	{
		//ない場合作成
		if (fonttype == FontType::DEFAULT)
		{
			//デフォルトフォント
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
			//フォント削除
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
