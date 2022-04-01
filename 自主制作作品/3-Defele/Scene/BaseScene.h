#pragma once
#include <memory>
#include "Obj/Obj.h"
#include "../common/Vector2.h"
#include "SystemObj/Cursol.h"
#include "SystemObj/Camera.h"

class BaseScene;
using UniqueScene = std::unique_ptr<BaseScene>;

//ƒV[ƒ“ˆê——
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
		//Šî’ê‚Å‚Í‰½‚à‚µ‚È‚¢
		return;
	}
	virtual void InitMusic(void)
	{
		//Šî’ê‚Å‚Í‰½‚à‚µ‚È‚¢
		return;
	}

	virtual UniqueScene UpDate(UniqueScene ownScene) = 0;
	virtual void Draw(void) = 0;						//‰æ–Ê•`‰æ—p

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
		//Šî’ê‚Å‚Í‰½‚à‚µ‚È‚¢
	}
	virtual void SetAlpha(double alpha) 
	{
		//Šî’ê‚Å‚Í‰½‚à‚µ‚È‚¢
	}

	virtual const std::string& GetMusicName(void)
	{
		//Šî’ê‚Å‚Í‰½‚à‚µ‚È‚¢
		return "";
	}

protected:

	SharedCamera camera_;
private:
};

