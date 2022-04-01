#include<Dxlib.h>
#include<windows.h>
#include"SceneMng.h"
#include "TitleScene.h"
#include"GameScene.h"
#include"EditScene.h"
#include"fadeinout.h"

#include"../_debug/_DebugConOut.h"

TitleScene::TitleScene()
{
	TRACE("TitleSceneの生成\n");
	Init();
}

TitleScene::~TitleScene()
{
	TRACE("TitleSceneの削除\n");
}

uniqueBaseScene TitleScene::UpDate(uniqueBaseScene own)
{
	if (CheckHitKey(KEY_INPUT_DOWN) || (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_DOWN))
	{
		if (oldflag == false)
		{
			PlaySoundFile("music/cursor-01.wav", DX_PLAYTYPE_BACK);
			oldflag = true;
			selectbutton++;
			if (selectbutton == 3)
			{
				selectbutton = 0;
			}
		}
	}
	else
	{
		oldflag = false;
	}

	if (CheckHitKey(KEY_INPUT_UP) || (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_UP))
	{
		if (upflag == false)
		{
			PlaySoundFile("music/cursor-01.wav", DX_PLAYTYPE_BACK);
			upflag = true;
			selectbutton--;
			if (selectbutton == -1)
			{
				selectbutton = 2;
			}
		}
	}
	else
	{
		upflag = false;
	}

	if ((CheckHitKey(KEY_INPUT_SPACE) || (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_A)) && selectbutton == 0)
	{
		if (startFlag == false)
		{
			PlaySoundFile("music/butt.mp3", DX_PLAYTYPE_BACK);
			startFlag = true;
			return std::make_unique<fadeinout>(std::move(own), std::make_unique<GameScene>());
		}
	}
	else
	{
		startFlag = false;
	}

	if ((CheckHitKey(KEY_INPUT_SPACE) || (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_A)) && selectbutton == 2)
	{
		
		if (exitFlag == false)
		{
			PlaySoundFile("music/butt.mp3", DX_PLAYTYPE_BACK);
			if (MessageBox(GetMainWindowHandle(), "ゲームを終了しますか？", "確認ダイヤログ", MB_OKCANCEL) == IDOK)
			{
				exitFlag = true;
				lpSceneMng.outkey = true;
			}
		}
	}
	else
	{
		exitFlag = false;
	}

	DrawOwnScreen();
	return std::move(own);
}

SceneID TitleScene::GetSceneID()
{
	return SceneID::TITLE;
}

bool TitleScene::Init()
{
	PlayMusic("music/theme.mp3", DX_PLAYTYPE_LOOP);
	auto GameScreenSize = lpSceneMng.viewArea_;

	buttonpos = { 246,54 };
	selectbutton = 0;

	oldflag = false;
	upflag = false;
	startFlag = false;
	exitFlag = false;
	Drawflag = false;

	maxtime = 1.5;
	doublecnt = 0;
	returnPoint = 0;

	titleimg = LoadGraph("image/Title.png");

	startimg = LoadGraph("image/Startボタン.png");
	creditimg = LoadGraph("image/Credit.png");
	exitimg = LoadGraph("image/Exitボタン.png");

	wakuimg = LoadGraph("image/waku.png");

	//+1は右端と下端の罫線描画のため
	titleScrID_ = MakeScreen(GameScreenSize.x + 1, GameScreenSize.y + 1, 255);

	//lpMapCtl.MapLoad(*this);
	DrawOwnScreen();

	////AddobjList(std::make_shared<Player>(camera_));

	return true;
}

void TitleScene::DrawOwnScreen(void)
{
	SetDrawScreen(titleScrID_);
	ClsDrawScreen();

	//シーンの内容の描画
	/*int StringWidth1 = GetDrawStringWidth("Title", strlen("Title"));
	SetFontSize(250);
	DrawString((lpSceneMng.viewArea_.x - StringWidth1) / 2, 20, "Title", GetColor(255, 255, 255));*/

	DrawGraph(0, 0, titleimg, true);

	int scrpos = (lpSceneMng.viewArea_.x - buttonpos.x) / 2;
	if (selectbutton == 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 230);
	}
	DrawGraph(scrpos, buttonpos.y * 5-10, startimg, true);
	if (selectbutton == 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	if (selectbutton == 1)
	{
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 230);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 215);
	}
	DrawGraph(scrpos+5, buttonpos.y * 6+7, creditimg, true);
	if (selectbutton == 1)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	if (selectbutton == 2)
	{
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 230);
	}
	DrawGraph(scrpos, buttonpos.y * 7+10, exitimg, true);
	if (selectbutton == 2)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	//広がり
	if ((CheckHitKey(KEY_INPUT_SPACE) || (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_A)) && WideFlag == false && selectbutton == 1)
	{
		PlaySoundFile("music/butt.mp3", DX_PLAYTYPE_BACK);
		doublecnt = 0;
		WideFlag = true;
		Drawflag = true;
	}
	if (WideFlag == true)
	{
		if (doublecnt <= maxtime)
		{
			SizeUpUI(doublecnt, maxtime);
		}
	}
	//狭く
	if ((CheckHitKey(KEY_INPUT_SPACE) || (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_A)) && WideFlag == true && returnPoint > 259)
	{
		doublecnt = maxtime;
		NarrowFlag = true;
	}
	if (NarrowFlag == true)
	{
		if (doublecnt <= maxtime * 2.0)
		{
			SizeUpUI(doublecnt, maxtime);
		}
		else
		{
			NarrowFlag = false;
			WideFlag = false;
			Drawflag = false;
		}
	}

	if (Drawflag)
	{
		Vector2 harfpos = (lpSceneMng.viewArea_) / 2;
		DrawExtendGraph(static_cast<int>(harfpos.x - returnPoint - 60), static_cast<int>(harfpos.y - returnPoint + 12),
			static_cast<int>(harfpos.x + returnPoint + 60), static_cast<int>(harfpos.y + returnPoint - 12), wakuimg, true);
		int StringWidth = GetDrawStringWidth("SPACE:戻る", static_cast<int>(strlen("SPACE:戻る")));	//「編集モード」の横サイズの取得
		DrawString(harfpos.x - StringWidth / 2, 0, "SPACE:戻る", GetColor(255, 255, 0), GetColor(255, 255, 255));

	}

	SetDrawScreen(screenScrID_);
	ClsDrawScreen();

	//シーンの内容の描画
	DrawGraph(0,0, titleScrID_, true);

	doublecnt += 0.05;
}

void TitleScene::SizeUpUI(double time, double totaltime)
{
	returnPoint = easing_.OutSine(time, totaltime) * 260;
}
