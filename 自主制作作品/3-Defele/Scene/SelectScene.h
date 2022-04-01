#pragma once
#include <memory>
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
	void DrawDefault(void);
	void DrawStgSelect(void);
	//マップの拡大
	bool MagniMap(void);
	void DrawMagniMap(void);
	//マップの縮小
	void DrawDownUI(void);
	//マップ描画画面
	void DrawMapScreen(void);

	//最終確認画面
	bool ComfirmUi(void);
	void DrawComfirmUi(void);

	//通常
	bool DefMapUI(void);
	//マップの通常描画
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

	//マップ縮小処理UI
	bool MapDownUI(void);
	//キャンセルUI
	bool CancelUI(void);
	//出撃UI
	bool SetoffUI(void);

	//最終確認
	//キャンセル
	bool CancelUI2(void);
	//出撃
	bool GoUI(void);

	//初期画面UIリスト
	std::list<std::unique_ptr<UI>> defBtnList_;
	std::list<std::unique_ptr<UI>> defStageList_;

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

	//マップオブジェクト一覧
	DeployObjSum mapObj_;
	//現在のマップネーム
	std::string mapName_;

	std::vector<Vector2> stgSelectPoses_;
	bool moveEndFlag_;
	std::vector<double> moveTimes_;

	bool stgSelectFlag_;
	bool toTitleFlag_;

	//決定入力フラグ
	bool defScrEndFlag_;

	//マップ拡大秒数
	double mapMagniTime_;
	//マップ拡大フラグ
	bool mapMagniUpFlag_;
	//マップ縮小フラグ
	bool mapMagniDownFlag_;
	//最終確認フラグ
	bool comScrFlag_;
	bool comEndFlag_;

	//マップハンドル
	int mapGh_;

	//背景グラフィックハンドル
	int backGroundGh_;
};

