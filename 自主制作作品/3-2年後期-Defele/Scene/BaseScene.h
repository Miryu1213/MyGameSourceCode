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

//�V�[���ꗗ
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
		//���ł͉������Ȃ�
		return;
	}
	virtual void InitMusic(void)
	{
		//���ł͉������Ȃ�
		return;
	}

	virtual UniqueScene UpDate(UniqueScene ownScene) = 0;
	virtual void Draw(void) = 0;						//��ʕ`��p

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
		//���ł͉������Ȃ�
	}
	virtual void SetAlpha(double alpha) 
	{
		//���ł͉������Ȃ�
	}

	virtual const std::string& GetMusicName(void)
	{
		//���ł͉������Ȃ�
		return "";
	}

	virtual const std::list<std::unique_ptr<Signal>>& GetSigNalList(void)
	{
		//���ł͉������Ȃ�
		return std::list<std::unique_ptr<Signal>>();
	}

protected:

	SharedCamera camera_;
private:
};

