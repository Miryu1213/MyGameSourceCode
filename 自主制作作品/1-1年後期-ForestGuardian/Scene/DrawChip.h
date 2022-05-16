#pragma once
#include<DxLib.h>
#include"MapCtl.h"
#include"../common/vector2.h"
#include"../common/ImageMng.h"

#include"../_debug/_DebugConOut.h"
//標準描画
struct DrawChipDef
{
	//x,yはマス目数
	void operator()(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)
	{
		DrawGraph(x * chipSize.x + drawOffset.x, y * chipSize.y+drawOffset.y, imageID, true);
	}

};

//何も描画しない（BLANK用）
struct DrawChipNON
{
	//x,yはマス目数
	void operator()(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)
	{
	}
};

//脱出梯子用（金塊すべて取得時に描画）
//struct DrawChipEsc
//{
//	x,yはマス目数
//	void operator()(const int x, const int y, const vector2& chipSize, const vector2& drawOffset, const int imageID)
//	{
//		if (lpMapCtl.GetGlodCnt() == 0)
//		{
//			DrawGraph(x * chipSize.x + drawOffset.x, y * chipSize.y + drawOffset.y, lpImageMng.GetID("tileset")[static_cast<int>(ChipID::LAD)], true);
//		}
//	}
//};
