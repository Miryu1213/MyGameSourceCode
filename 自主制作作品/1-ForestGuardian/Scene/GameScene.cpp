#include<Dxlib.h>
#include"SceneMng.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"Obj/Unit.h"
#include"Obj/Player.h"
#include"Obj/Slime.h"
#include"Obj/Flour.h"
#include"Obj/Warwolf.h"
#include"../common/ImageMng.h"

#include"MapCtl.h"
#include"../_debug/_DebugConOut.h"

GameScene::GameScene()
{
	TRACE("GameSceneÇÃê∂ê¨\n");
	Init();
}

GameScene::~GameScene()
{
	TRACE("GameSceneÇÃçÌèú\n");
	DeleteGraph(BGimage_);
}


uniqueBaseScene GameScene::UpDate(uniqueBaseScene own)
{
	if (descflag == false)
	{
		if ((CheckHitKeyAll(DX_CHECKINPUT_KEY) || CheckHitKeyAll(DX_CHECKINPUT_PAD)))
		{
			descflag = true;
		}
	}
	else
	{
		enemypopfre();

		if (dethFlag == false && effFlag == false)
		{
			if (UpDateObj() == true)
			{
				dethFlag = true;
			}
			camera_->UpDate();
			if ((count_ / 60) <= 60)
			{
				count_++;
			}
			if ((count_ / 60) == 61)
			{
				effFlag = true;
				//auto checkint = GetDrawScreenGraph(0, 0, lpSceneMng.viewArea_.x, lpSceneMng.viewArea_.y, chapimg_);		//âÊñ ÇÃÉLÉÉÉvÉ`ÉÉ
				/*if (checkint == -1)
				{
					TRACE("é∏îs\n");
				}*/
				//count_++;
			}
		}

		if (dethFlag == true)
		{
			if (CheckHitKey(KEY_INPUT_Z))
			{
				MapCtl::Destroy();
				return std::make_unique<TitleScene>();
			}
		}

		if (thankflag == true && doFlag == true)
		{
			if (CheckHitKey(KEY_INPUT_Z))
			{
				MapCtl::Destroy();
				return std::make_unique<TitleScene>();
			}
		}

		if (effFlag == true)
		{
			if (effcnt == EFF_MAXTIME)
			{
				clearflag = true;
			}
			else
			{
				effcnt++;
			}
			if (effcnt == 10)
			{
				for (auto itotherobj = objList_.begin(); itotherobj != objList_.end();)
				{
					auto enemyobj = *itotherobj;
					if (enemyobj->GetObjType() == ObjType::Player)
					{
						itotherobj++;
						continue;
					}
					itotherobj = objList_.erase(itotherobj);
					//itotherobj++;
				}
			}
			if (effcnt == 1)
			{
				PlaySoundFile("music/lightning.mp3", DX_PLAYTYPE_BACK);
			}
			if (effcnt == 140)
			{
				PlaySoundFile("music/lightning2.mp3", DX_PLAYTYPE_BACK);
			}
		}

		if (clearflag == true)
		{
			if (dodoflag == true)
			{
				if (CheckHitKey(KEY_INPUT_Z))
				{
					thankflag = true;
					doFlag = false;
				}
				else
				{
					doFlag = true;
				}
			}
		}

		if ((CheckHitKeyAll(DX_CHECKINPUT_KEY) || CheckHitKeyAll(DX_CHECKINPUT_PAD)))
		{
			dodoflag = false;
		}
		else
		{
			dodoflag = true;
		}
		/*if (gameoverflag == true && doFlag == true)
		{
			MapCtl::Destroy();
			return std::make_unique<TitleScene>();
		}*/

		DrawOwnScreen();
	}
	return std::move(own);
}

SceneID GameScene::GetSceneID()
{
	return SceneID::GAME;
}

bool GameScene::Init()
{
	PlayMusic("music/gamebattle.mp3", DX_PLAYTYPE_LOOP);
	MapCtl::Create(SceneID::GAME);
	if ((BGimage_ = LoadGraph("image/Forest.png")) == -1)
	{
		TRACE("îwåiÇÃì«Ç›çûÇ›é∏îs\n");
	}
	camera_->InitArea();
	lpMapCtl.SetCamera(camera_);

	count_ = 0;
	chapimg_ = MakeScreen(lpSceneMng.viewArea_.x, lpSceneMng.viewArea_.y, false);
	gameoverimg_ = LoadGraph("image/gameover.png");
	dethFlag = false;
	thankflag = false;
	clearflag = false;
	clearimg_= LoadGraph("image/clear.png");
	thankimg_ = LoadGraph("image/thank.png");

	filterimg_ = LoadGraph("image/filter.png");

	descimg_ = LoadGraph("image/desc.png");
	descflag = false;

	LoadDivGraph("image/blackhall.png", 20, 5, 4, 96, 96, blackhallimg_, true);

	LoadDivGraph("image/lighteff.png", 15, 5, 3, 640, 480, lighteffimg_, true);

	doFlag = false;
	dodoflag = false;

	animcnt = 0;
	effcnt = 0;

	auto GameScreenSize = lpSceneMng.viewArea_;

	//+1ÇÕâEí[Ç∆â∫í[ÇÃårê¸ï`âÊÇÃÇΩÇﬂ
	gameScrID_ = MakeScreen(GameScreenSize.x + 1, GameScreenSize.y + 1, 255);

	lpMapCtl.MapLoad(*this);
	DrawOwnScreen();

	auto Plobj = AddobjList(std::make_shared<Player>(SceneID::GAME, camera_));
	camera_->SetPos((*Plobj)->GetPos());
	camera_->SetTarget((*Plobj));
	return true;
}

void GameScene::DrawOwnScreen(void)
{
	SetDrawScreen(gameScrID_);
	ClsDrawScreen();
	//ÉVÅ[ÉìÇÃì‡óeÇÃï`âÊ
	auto camoffset = camera_->GetDrawOffset();
	DrawModiGraph(
		camoffset.x, 0,														//ç∂è„
		lpSceneMng.viewArea_.x + camoffset.x, 0,							//âEè„
		lpSceneMng.viewArea_.x + camoffset.x, lpSceneMng.viewArea_.y,		//âEâ∫
		camoffset.x, lpSceneMng.viewArea_.y,								//ç∂â∫
		BGimage_, false);
	DrawModiGraph(
		camoffset.x + lpSceneMng.viewArea_.x, 0,													//ç∂è„
		lpSceneMng.viewArea_.x + camoffset.x + lpSceneMng.viewArea_.x, 0,							//âEè„
		lpSceneMng.viewArea_.x + camoffset.x + lpSceneMng.viewArea_.x, lpSceneMng.viewArea_.y,		//âEâ∫
		camoffset.x + lpSceneMng.viewArea_.x, lpSceneMng.viewArea_.y,								//ç∂â∫
		BGimage_, false);
	lpMapCtl.DrawFunc();
	
	DrawBlackHall();
	
	DrawObj();
	
	//DrawExtendGraph(lpSceneMng.viewArea_.x - 80, -10, lpSceneMng.viewArea_.x - 80+50, 35, filterimg_, true);
	if (effFlag)
	{
		DrawFormatString(lpSceneMng.viewArea_.x - 80, 10, GetColor(0, 0, 0), "%d", 0);
	}
	else
	{
		DrawFormatString(lpSceneMng.viewArea_.x - 80, 10, GetColor(0, 0, 0), "%d", MAX_TIME - (count_ / 60));
	}

	if (effFlag == true)
	{
		int efftime = (effcnt / 10) % 15;
		DrawExtendGraph(0, 0, lpSceneMng.viewArea_.x, lpSceneMng.viewArea_.y, lighteffimg_[efftime], true);
	}

	if ((count_ / 60) >= 61)
	{
		//DrawGraph(0, 0, chapimg_, true);
		if (clearflag == true && thankflag == false)
		{
			DrawExtendGraph(10, 0, lpSceneMng.viewArea_.x-10, lpSceneMng.viewArea_.y, clearimg_, true);
		}
		if (thankflag == true)
		{
			DrawExtendGraph(10, 0, lpSceneMng.viewArea_.x-10, lpSceneMng.viewArea_.y, thankimg_, true);
		}
	}

	if (dethFlag == true)
	{
		count_ = 0;
		DrawExtendGraph(0, 0, lpSceneMng.viewArea_.x, lpSceneMng.viewArea_.y, gameoverimg_, false);
	}

	if (descflag == false)
	{
		DrawGraph(0, 0, descimg_, true);
	}

	SetDrawScreen(screenScrID_);
	ClsDrawScreen();

	//ÉVÅ[ÉìÇÃì‡óeÇÃï`âÊ
	DrawGraph(0,0, gameScrID_, true);
}

void GameScene::DrawBlackHall()
{
	auto camoffset = camera_->GetDrawOffset();
	int cnttimer = (count_ / 60);
	if (cnttimer >= 8 && cnttimer <= 10)
	{
		animcnt++;
		int changecnt = (animcnt / 9) % 20;
		DrawGraph(lpSceneMng.worldArea_.x + camoffset.x - 120, lpSceneMng.viewArea_.y - 32 * 5, blackhallimg_[changecnt], true);
		DrawGraph(80 + camoffset.x, lpSceneMng.viewArea_.y - 32 * 5, blackhallimg_[changecnt], true);
	}
	if (cnttimer == 11)
	{
		animcnt = 0;
	}
	if (cnttimer >= 18 && cnttimer <= 20)
	{
		animcnt++;
		int changecnt = (animcnt / 9) % 20;
		DrawGraph(lpSceneMng.worldArea_.x + camoffset.x - 120, lpSceneMng.viewArea_.y - 32 * 5, blackhallimg_[changecnt], true);
		DrawGraph(80 + camoffset.x, lpSceneMng.viewArea_.y - 32 * 5, blackhallimg_[changecnt], true);
	}
	if (cnttimer == 21)
	{
		animcnt = 0;
	}
	if (cnttimer >= 33 && cnttimer <= 35)
	{
		animcnt++;
		int changecnt = (animcnt / 9) % 20;
		DrawGraph(lpSceneMng.worldArea_.x + camoffset.x - 120, lpSceneMng.viewArea_.y - 32 * 5, blackhallimg_[changecnt], true);
		DrawGraph(lpSceneMng.worldArea_.x + camoffset.x - 120, lpSceneMng.viewArea_.y - 32 * 10, blackhallimg_[changecnt], true);
		DrawGraph(80 + camoffset.x, lpSceneMng.viewArea_.y - 32 * 5, blackhallimg_[changecnt], true);
		DrawGraph(80 + camoffset.x, lpSceneMng.viewArea_.y - 32 * 10, blackhallimg_[changecnt], true);
	}
	if (cnttimer == 36)
	{
		animcnt = 0;
	}
	if (cnttimer >= 43 && cnttimer <= 45)
	{
		animcnt++;
		int changecnt = (animcnt / 9) % 20;
		DrawGraph(lpSceneMng.worldArea_.x + camoffset.x - 120, lpSceneMng.viewArea_.y - 32 * 5, blackhallimg_[changecnt], true);
		DrawGraph(lpSceneMng.worldArea_.x + camoffset.x - 120, lpSceneMng.viewArea_.y - 32 * 10, blackhallimg_[changecnt], true);
		DrawGraph(lpSceneMng.worldArea_.x - 32 * 8 + camoffset.x, 0, blackhallimg_[changecnt], true);
		DrawGraph(80 + camoffset.x, lpSceneMng.viewArea_.y - 32 * 5, blackhallimg_[changecnt], true);
		DrawGraph(80 + camoffset.x, lpSceneMng.viewArea_.y - 32 * 10, blackhallimg_[changecnt], true);
		DrawGraph(32 * 15 + camoffset.x, 0, blackhallimg_[changecnt], true);
	}
	if (cnttimer == 46)
	{
		animcnt = 0;
	}
	if (cnttimer >= 50 && cnttimer <= 52)
	{
		animcnt++;
		int changecnt = (animcnt / 9) % 20;
		DrawGraph(lpSceneMng.worldArea_.x - 32 * 8 + camoffset.x, 0, blackhallimg_[changecnt], true);
		DrawGraph(32 * 15 + camoffset.x, 0, blackhallimg_[changecnt], true);
		DrawGraph(32 * 28 + camoffset.x, 32 * 5, blackhallimg_[changecnt], true);
	}
}

void GameScene::enemypopfre()
{
	if ((count_ / 60) == 10)
	{
		auto checkcnt = count_ % 60;
		if (checkcnt < POP_COUNT)
		{
			enemypop(0);
		}
	}

	if ((count_ / 60) == 20)
	{
		auto checkcnt = count_ % 60;
		if (checkcnt < POP_COUNT * 2)
		{
			enemypop(0);
		}
	}
	if ((count_ / 60) == 35)
	{
		auto checkcnt = count_ % 60;
		if (checkcnt < (POP_COUNT * 2))
		{
			enemypop(1);
		}
	}
	if ((count_ / 60) == 45)
	{
		auto checkcnt = count_ % 60;
		if (checkcnt < (POP_COUNT * 2))
		{
			enemypop(2);
		}
	}
	if ((count_ / 60) == 52)
	{
		auto checkcnt = count_ % 60;
		if (checkcnt < (POP_COUNT * 2))
		{
			enemypop(3);
		}
	}
}

void GameScene::enemypop(int poptype)
{
	auto ramdam = rand() % 3;
	auto LRramdam = rand() % 2;
	if (poptype == 0)
	{
		if (ramdam == 0)
		{
			auto Eneobj = AddobjList(std::make_shared<Slime>(GetSceneID(), camera_));
			Vector2 EnePos;
			if (LRramdam == 0)
			{
				EnePos.x = lpSceneMng.worldArea_.x - 120;
			}
			else
			{
				EnePos.x = 100;
			}
			EnePos.y = lpSceneMng.viewArea_.y - 32 * 3;
			(*Eneobj)->SetPos(EnePos);
			(*Eneobj)->SetDefPos(EnePos);
			(*Eneobj)->SetDirLR();
		}
		else if (ramdam == 1)
		{
			auto Eneobj = AddobjList(std::make_shared<Flour>(GetSceneID(), camera_));
			Vector2 EnePos;
			if (LRramdam == 0)
			{
				EnePos.x = lpSceneMng.worldArea_.x - 120;
			}
			else
			{
				EnePos.x = 100;
			}
			EnePos.y = lpSceneMng.viewArea_.y - 32 * 4;
			(*Eneobj)->SetPos(EnePos);
			(*Eneobj)->SetDefPos(EnePos);
			(*Eneobj)->SetDirLR();
		}
		else
		{
			auto Eneobj = AddobjList(std::make_shared<Warwolf>(GetSceneID(), camera_));
			Vector2 EnePos;
			if (LRramdam == 0)
			{
				EnePos.x = lpSceneMng.worldArea_.x - 120;
			}
			else
			{
				EnePos.x = 100;
			}
			EnePos.y = lpSceneMng.viewArea_.y - 32 * 5;
			(*Eneobj)->SetPos(EnePos);
			(*Eneobj)->SetDefPos(EnePos);
			(*Eneobj)->SetDirLR();
		}
	}
	if (poptype == 1)
	{
		auto TBramdam = rand() % 2;
		if (ramdam == 0)
		{
			auto Eneobj = AddobjList(std::make_shared<Slime>(GetSceneID(), camera_));
			Vector2 EnePos;
			if (LRramdam == 0)
			{
				EnePos.x = lpSceneMng.worldArea_.x - 120;
			}
			else
			{
				EnePos.x = 100;
			}
			if (TBramdam == 0)
			{
				EnePos.y = lpSceneMng.viewArea_.y - 32 * 3;
			}
			else
			{
				EnePos.y = lpSceneMng.viewArea_.y - 32 * 8;
			}
			(*Eneobj)->SetPos(EnePos);
			(*Eneobj)->SetDefPos(EnePos);
			(*Eneobj)->SetDirLR();
		}
		else if (ramdam == 1)
		{
			auto Eneobj = AddobjList(std::make_shared<Flour>(GetSceneID(), camera_));
			Vector2 EnePos;
			if (LRramdam == 0)
			{
				EnePos.x = lpSceneMng.worldArea_.x - 120;
			}
			else
			{
				EnePos.x = 100;
			}
			if (TBramdam == 0)
			{
				EnePos.y = lpSceneMng.viewArea_.y - 32 * 4;
			}
			else
			{
				EnePos.y = lpSceneMng.viewArea_.y - 32 * 9;
			}
			(*Eneobj)->SetPos(EnePos);
			(*Eneobj)->SetDefPos(EnePos);
			(*Eneobj)->SetDirLR();
		}
		else
		{
			auto Eneobj = AddobjList(std::make_shared<Warwolf>(GetSceneID(), camera_));
			Vector2 EnePos;
			if (LRramdam == 0)
			{
				EnePos.x = lpSceneMng.worldArea_.x - 120;
			}
			else
			{
				EnePos.x = 100;
			}
			if (TBramdam == 0)
			{
				EnePos.y = lpSceneMng.viewArea_.y - 32 * 5;
			}
			else
			{
				EnePos.y = lpSceneMng.viewArea_.y - 32 * 10;
			}
			(*Eneobj)->SetPos(EnePos);
			(*Eneobj)->SetDefPos(EnePos);
			(*Eneobj)->SetDirLR();
		}
	}
	if (poptype == 2)
	{
		auto TBramdam = rand() % 2;
		auto LRTopram = rand() % 4;
		if (LRTopram == 0 || LRTopram == 1)
		{
			if (ramdam == 0)
			{
				auto Eneobj = AddobjList(std::make_shared<Slime>(GetSceneID(), camera_));
				Vector2 EnePos;
				if (LRramdam == 0)
				{
					EnePos.x = lpSceneMng.worldArea_.x - 120;
				}
				else
				{
					EnePos.x = 100;
				}
				if (TBramdam == 0)
				{
					EnePos.y = lpSceneMng.viewArea_.y - 32 * 3;
				}
				else
				{
					EnePos.y = lpSceneMng.viewArea_.y - 32 * 8;
				}
				(*Eneobj)->SetPos(EnePos);
				(*Eneobj)->SetDefPos(EnePos);
				(*Eneobj)->SetDirLR();
			}
			else if (ramdam == 1)
			{
				auto Eneobj = AddobjList(std::make_shared<Flour>(GetSceneID(), camera_));
				Vector2 EnePos;
				if (LRramdam == 0)
				{
					EnePos.x = lpSceneMng.worldArea_.x - 120;
				}
				else
				{
					EnePos.x = 100;
				}
				if (TBramdam == 0)
				{
					EnePos.y = lpSceneMng.viewArea_.y - 32 * 4;
				}
				else
				{
					EnePos.y = lpSceneMng.viewArea_.y - 32 * 9;
				}
				(*Eneobj)->SetPos(EnePos);
				(*Eneobj)->SetDefPos(EnePos);
				(*Eneobj)->SetDirLR();
			}
			else
			{
				auto Eneobj = AddobjList(std::make_shared<Warwolf>(GetSceneID(), camera_));
				Vector2 EnePos;
				if (LRramdam == 0)
				{
					EnePos.x = lpSceneMng.worldArea_.x - 120;
				}
				else
				{
					EnePos.x = 100;
				}
				if (TBramdam == 0)
				{
					EnePos.y = lpSceneMng.viewArea_.y - 32 * 5;
				}
				else
				{
					EnePos.y = lpSceneMng.viewArea_.y - 32 * 10;
				}
				(*Eneobj)->SetPos(EnePos);
				(*Eneobj)->SetDefPos(EnePos);
				(*Eneobj)->SetDirLR();
			}
		}
		if (LRTopram == 2)
		{
			Vector2 EnePos;
			EnePos.x = 32 * 15;
			EnePos.y = 32;
			if (ramdam == 0)
			{
				auto Eneobj = AddobjList(std::make_shared<Slime>(GetSceneID(), camera_));
				(*Eneobj)->SetPos(EnePos);
				(*Eneobj)->SetDefPos(EnePos);
				(*Eneobj)->SetDirLR();
			}
			else if (ramdam == 1)
			{
				auto Eneobj = AddobjList(std::make_shared<Flour>(GetSceneID(), camera_));
				(*Eneobj)->SetPos(EnePos);
				(*Eneobj)->SetDefPos(EnePos);
				(*Eneobj)->SetDirLR();
			}
			else
			{
				EnePos.y = 0;
				auto Eneobj = AddobjList(std::make_shared<Warwolf>(GetSceneID(), camera_));
				(*Eneobj)->SetPos(EnePos);
				(*Eneobj)->SetDefPos(EnePos);
				(*Eneobj)->SetDirLR();
			}
		}
		if (LRTopram == 3)
		{
			Vector2 EnePos;
			EnePos.x = lpSceneMng.worldArea_.x - 32 * 8;
			EnePos.y = 32;
			if (ramdam == 0)
			{
				auto Eneobj = AddobjList(std::make_shared<Slime>(GetSceneID(), camera_));
				(*Eneobj)->SetPos(EnePos);
				(*Eneobj)->SetDefPos(EnePos);
				(*Eneobj)->SetDirLR();
			}
			else if (ramdam == 1)
			{
				auto Eneobj = AddobjList(std::make_shared<Flour>(GetSceneID(), camera_));
				(*Eneobj)->SetPos(EnePos);
				(*Eneobj)->SetDefPos(EnePos);
				(*Eneobj)->SetDirLR();
			}
			else
			{
				EnePos.y = 0;
				auto Eneobj = AddobjList(std::make_shared<Warwolf>(GetSceneID(), camera_));
				(*Eneobj)->SetPos(EnePos);
				(*Eneobj)->SetDefPos(EnePos);
				(*Eneobj)->SetDirLR();
			}
		}
	}
	if (poptype == 3)
	{
		auto TBramdam = rand() % 2;
		auto LRTopram = rand() % 3;
		ramdam = 0;
		if (LRTopram == 0)
		{
			Vector2 EnePos;
			EnePos.x = 32 * 15;
			EnePos.y = 32;
			if (ramdam == 0)
			{
				EnePos.y = 0;
				auto Eneobj = AddobjList(std::make_shared<Warwolf>(GetSceneID(), camera_));
				(*Eneobj)->SetPos(EnePos);
				(*Eneobj)->SetDefPos(EnePos);
				(*Eneobj)->SetDirLR();
			}
		}
		if (LRTopram == 1)
		{
			Vector2 EnePos;
			EnePos.x = lpSceneMng.worldArea_.x - 32 * 8;
			EnePos.y = 32;
			if (ramdam == 0)
			{
				EnePos.y = 0;
				auto Eneobj = AddobjList(std::make_shared<Warwolf>(GetSceneID(), camera_));
				(*Eneobj)->SetPos(EnePos);
				(*Eneobj)->SetDefPos(EnePos);
				(*Eneobj)->SetDirLR();
			}
		}
		if (LRTopram == 2)
		{
			Vector2 EnePos;
			EnePos.x = 32 * 28;
			EnePos.y = 32 * 6;
			if (ramdam == 0)
			{
				EnePos.y = 32 * 5;
				auto Eneobj = AddobjList(std::make_shared<Warwolf>(GetSceneID(), camera_));
				(*Eneobj)->SetPos(EnePos);
				(*Eneobj)->SetDefPos(EnePos);
				(*Eneobj)->SetDirLR();
			}
		}
	}
}
