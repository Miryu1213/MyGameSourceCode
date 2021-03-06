#pragma once
#include <memory>
#include <list>
#include "Obj/Obj.h"
#include "../common/Vector2.h"
#include "SystemObj/Cursol.h"
#include "SystemObj/Camera.h"
#include "SystemObj/Signal.h"

class BaseScene;
using UniqueScene = std::unique_ptr<BaseScene>;

//シーン一覧
enum class Scene
{
	TITLE,
	SELECT,
	GAME,
	RESULT,
	TUTORIAL,
	TIPS,
	TRANSITION,
	COUNTDOWN,
	MAX
};

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene() {};

	virtual bool Init(void) = 0;
	virtual void InitFromTutorial(void)
	{
		//基底では何もしない
		return;
	}
	virtual void InitMusic(void)
	{
		//基底では何もしない
		return;
	}

	virtual UniqueScene UpDate(UniqueScene ownScene) = 0;
	virtual void Draw(void) = 0;						//画面描画用

	virtual Scene GetSceneID(void) = 0;

	void SetCamera(SharedCamera& camera)
	{
		camera_.reset();
		camera_ = camera;
	}

	SharedCamera& GetCamera(void)
	{
		return camera_;
	}

	virtual void SetIsFade(bool isfade)
	{
		//基底では何もしない
	}
	virtual void SetAlpha(double alpha) 
	{
		//基底では何もしない
	}

	virtual const std::string& GetMusicName(void)
	{
		//基底では何もしない
		return "";
	}

	virtual const std::list<std::unique_ptr<Signal>>& GetSigNalList(void)
	{
		//基底では何もしない
		return std::list<std::unique_ptr<Signal>>();
	}

protected:

	SharedCamera camera_;
private:
};

