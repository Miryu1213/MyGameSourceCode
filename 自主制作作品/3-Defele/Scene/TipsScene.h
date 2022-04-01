#pragma once
#include <thread>
#include "BaseScene.h"
#include "SystemObj/Tips.h"

class TipsScene :
    public BaseScene
{
public:
	TipsScene(std::string mapName);
	~TipsScene();
	
	bool Init(void) override;
	UniqueScene UpDate(UniqueScene ownScene) override;
	void Draw(void) override;

	Scene GetSceneID(void) override
	{
		return Scene::TIPS;
	};

	void SetIsFade(bool isfade) override
	{
		IsFade_ = isfade;
	}
	void SetAlpha(double alpha) override
	{
		alpha_ = alpha;
	}

private:
	bool IsFade_;
	double alpha_;

	std::string mapName_;
	int mapNumber_;

	double loadingTime_;
	double backLightTime_;
	int backLightAnimId_;

	bool oneLightFlag_;
	double oneLightTime_;
	int oneLightAnimId_;

	std::thread routeThread_;

	bool endLoadingFlag_;
	bool oneRingFlag_;

	//次のシーン一時保存
	UniqueScene nextScene_;

	//移動不可領域の設定
	void SetCantMovePoint(void);
	//コストマップの作成
	void CreateCostMap(void);
	void InitByThread(void);
	//ルートの作成
	void InitRoute(void);
};

