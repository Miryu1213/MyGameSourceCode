#pragma once
#include <string.>
#include <memory>
#include <list>
#include "../Obj/Obj.h"
#include "Gimmick/BaseGimmick.h"
#include "../UI/Place.h"

#define lpDeployMng DeployMng::GetInstance()

class UI;

//動的なシングルトン
class DeployMng
{
public:
	static void Create(std::list<std::unique_ptr<Obj>>* objlist,int offset)
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new DeployMng(objlist,offset);
		}
	}
	static void Destroy(void)
	{
		if (s_Instance != nullptr)
		{
			delete s_Instance;
			s_Instance = nullptr;
		}
	}
	static DeployMng& GetInstance(void)
	{
		return (*s_Instance);
	}

	const UI* GetBox(void)
	{
		return box_;
	}

	const UI* GetCircle(void)
	{
		return circle_;
	}

	bool GetEntryFlag(void)
	{
		return entryPlantFlag_;
	}
	void SetEntryFlag(bool flag);

	ObjType GetPlantType(void)
	{
		return plantType_;
	}

	bool GetShowBoxFlag(void)
	{
		return showboxFlag_;
	}
	void SetShowBoxFlag(bool flag)
	{
		showboxFlag_ = flag;
	}

	Vector2 GetEntryPlantPos(void)
	{
		return entryPlantPos_;
	}

	//手持ち施設全体の取得
	const std::map<ObjType, int>& GetDeployPlant(void)
	{
		return plantSum_;
	}


	//持ち物数の初期化
	bool Init(std::string mapname);

	void UpDate(Vector2 camoffset);
	void Draw(void);

	//UIの描画
	void DrawDeploy(SharedCamera& camera, bool isTutorial);
	//持っているものを描画
	void DrawWhatPlayerHas(Vector2 camOffset,bool isTutorial);
	//ボックス上の描画
	void DrawinBox(void);
	//特定位置に特定サイズで描画
	void DrawindentGraph(Vector2 pos, Vector2 size, int gh);

	//ギミックのセット
	void SetGimmick(GimmickType gimmicktype, Vector2 pos, Vector2 size);

	//ギミック所持の処理
	void GrabFunction(Vector2 camoffset);

	//白丸移動
	void CircleMoveRight(void);
	void CircleMoveLeft(void);

	//オブジェクトリスト
	std::list<std::unique_ptr<Obj>>* objList_;

	//ギミックと画像マップ
	std::map<GimmickType, std::vector<int>> gimmickGhMap_;

protected:
	//プラント画像マップ
	std::map<ObjType, int> plantGhMap_;
private:
	DeployMng(std::list<std::unique_ptr<Obj>>* objlist, int offset);
	~DeployMng();

	//唯一のインスタンス
	static DeployMng* s_Instance;

	//下UIの処理
	void DeployUIFunction(void);

	//ギミック管理
	std::list<std::unique_ptr<BaseGimmick>> gimList_;

	//表示系
	//現在の時間
	double nowTime_;
	//移動終了フラグ
	bool moveFinishFlag_;
	//遅延用フラグ
	bool delayFlag_;

	//UI
	UI* circle_;
	UI* box_;

	//設置座標
	Vector2 entryPlantPos_;
	//プラント設置フラグ
	bool entryPlantFlag_;
	//ギミックつかみ
	bool grabGimmickFlag_;
	//Box表示フラグ
	bool showboxFlag_;

	//現在の選択タイプ
	GimmickType gimType_;
	ObjType plantType_;

	//配置できる数
	std::map<ObjType, int> plantSum_;
	std::map<GimmickType, int> gimmickSum_;

	//施設配置位置合計
	std::list<Vector2> plantPosList;
};
