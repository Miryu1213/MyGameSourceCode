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

	//現在の画面
	enum class CURSCREEN
	{
		NONE,
		START,
		END
	};

	//移動不可領域の設定
	void SetCantMovePoint(void);
	//コストマップの作成
	void CreateCostMap(void);
	void InitByThread(void);
	//ルートの作成
	void InitRoute(void);

	CURSCREEN curScreen_;

	bool IsFade_;
	double alpha_;

	std::string mapName_;
	int mapNumber_;

	//アニメーション用カウント
	double loadingTime_;
	double backLightTime_;
	double clickLightTime_;

	//スレッド
	std::thread routeThread_;
	bool endLoadingFlag_;

	bool oneRingFlag_;
	bool clickLightFlag_;

	//次のシーン一時保存
	UniqueScene nextScene_;
};

