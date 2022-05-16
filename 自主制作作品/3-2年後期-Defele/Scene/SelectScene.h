#pragma once
#include <memory>
#include <functional>
#include <list>
#include <map>
#include "BaseScene.h"
#include "GameScene.h"
#include "UI/UI.h"
#include "tmx/MapObject.h"

class SelectScene :
	public BaseScene
{
public:
	SelectScene();
	~SelectScene();

	bool Init(void) override;
	UniqueScene UpDate(UniqueScene ownScene) override;
	void Draw(void) override;

	Scene GetSceneID(void) override
	{
		return Scene::SELECT;
	};

private:

	//現在の画面
	enum class CURSCREEN
	{
		NONE,
		START,
		STARTEND,
		STAGESELECT,
		MAP,
		MAPMAGNI,
		MAPSHRINK,
		CONFIRM
	};

	//初期画面
	void UpdateStartScreen(void);
	void DrawStartScreen(void);
	//移動終了
	void UpdateStartEndScreen(void);
	void DrawStartEndScreen(void);
	//ステージセレクト画面
	void UpdateStgSelect(void);
	void DrawStgSelect(void);
	//マップ詳細画面
	void UpdateDetailedMap(void);
	void DrawDetailedMap(void);
	//マップ拡大
	void UpdateMagniMap(void);
	void DrawMagniMap(void);
	//マップ縮小
	void UpdateShrinkMap(void);
	void DrawShrinkMap(void);
	//最終確認画面
	void UpdateConfirm(void);
	void DrawConfirm(void);

	//マップ描画
	void DrawMap(void);

	void CreatePlantMenu(void);
	void DrawStatus(void);
	void DrawByObjType(Vector2 pos, ObjType objType);
	//能力描画用にIDをセット
	void SetUnitActiveID(void);
	void SetID(ObjType objType, int startIdX, int startIdY, int divX, int frameSum);
	int CaliculateId(ObjType objType);

	//全ての画像ロード
	void LoadImageAll(void);

	//初期画面UIリスト
	std::list<std::unique_ptr<UI>> startBtnList_;
	std::list<std::unique_ptr<UI>> stgSelectList_;

	//マップ表示時UIリスト
	std::list<std::unique_ptr<UI>> mapUiList_;
	std::list<std::unique_ptr<UI>> plantList_;
	int plantSum_;
	std::map<ObjType, std::vector<int>> unitIdMap_;
	double animTime_;

	//マップ拡大画面UIリスト
	std::list<std::unique_ptr<UI>> mapMagniUiList_;

	//最終確認画面UIリスト
	std::list<std::unique_ptr<UI>> confirmUiList_;

	//各画面関数
	std::map<CURSCREEN, std::function<void(void)>> update_;
	std::map<CURSCREEN, std::function<void(void)>> draw_;

	//マップオブジェクト一覧
	DeployObjSum mapObj_;
	//現在のマップネーム
	std::string mapName_;

	//ステージセレクト
	std::vector<Vector2> stgSelectPoses_;
	std::vector<double> moveTimes_;

	//現在のスクリーン
	CURSCREEN curScreen_;
	bool toTitleFlag_;
	bool toNextSceneFlag_;

	//マップ拡大秒数
	double mapMagniTime_;

	//マップハンドル
	int mapGh_;
	//背景グラフィックハンドル
	int backGroundGh_;
};

