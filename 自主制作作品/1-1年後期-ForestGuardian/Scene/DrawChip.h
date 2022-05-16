#pragma once
#include<DxLib.h>
#include"MapCtl.h"
#include"../common/vector2.h"
#include"../common/ImageMng.h"

#include"../_debug/_DebugConOut.h"
//�W���`��
struct DrawChipDef
{
	//x,y�̓}�X�ڐ�
	void operator()(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)
	{
		DrawGraph(x * chipSize.x + drawOffset.x, y * chipSize.y+drawOffset.y, imageID, true);
	}

};

//�����`�悵�Ȃ��iBLANK�p�j
struct DrawChipNON
{
	//x,y�̓}�X�ڐ�
	void operator()(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)
	{
	}
};

//�E�o��q�p�i���򂷂ׂĎ擾���ɕ`��j
//struct DrawChipEsc
//{
//	x,y�̓}�X�ڐ�
//	void operator()(const int x, const int y, const vector2& chipSize, const vector2& drawOffset, const int imageID)
//	{
//		if (lpMapCtl.GetGlodCnt() == 0)
//		{
//			DrawGraph(x * chipSize.x + drawOffset.x, y * chipSize.y + drawOffset.y, lpImageMng.GetID("tileset")[static_cast<int>(ChipID::LAD)], true);
//		}
//	}
//};
