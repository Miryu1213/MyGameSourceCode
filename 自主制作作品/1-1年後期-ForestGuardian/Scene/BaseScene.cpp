#include<DxLib.h>
#include"SceneMng.h"
#include "BaseScene.h"
#include"obj/obj.h"
#include"../_debug/_DebugConOut.h"

BaseScene::BaseScene()
{
	TRACE("BaseSceneÇÃê∂ê¨\n");
	if ((screenScrID_ = MakeScreen(lpSceneMng.scnArea_.x, lpSceneMng.scnArea_.y, true)) == -1)
	{
		TRACE("makescreené∏îs\n");
	}
	camera_ = std::make_shared<Camera>();
}

BaseScene::~BaseScene()
{
	TRACE("BaseSceneÇÃçÌèú\n");
}

ObjList::iterator BaseScene::AddobjList(ObjPtr&& objptr)
{
	objList_.push_back(std::move(objptr));
	auto itr = objList_.end();
	--itr;
	return itr;
}

void BaseScene::Draw()
{
	DrawGraph(0, 0, screenScrID_, true);
}

void BaseScene::ExtendDraw(int firstX, int firstY, int lastX, int lastY, bool inv)
{
	DrawExtendGraph(firstX, firstY, lastX, lastY, screenScrID_, inv);
}

bool BaseScene::UpDateObj(void)
{
	//îÕàÕforï∂
	bool Flag = false;
	for (auto& data : objList_)
	{
		/*Flag = (data->UpDate());
		if (Flag == true)
		{
			break;
		}*/
		Flag |= data->UpDate(objList_);
	}
	if (camera_)
	{
		camera_->UpDate();
	}
	return Flag;
}

bool BaseScene::DrawObj(void)
{
	for (auto& data : objList_)
	{
		data->Draw();
	}
	return true;
}
