#pragma once
#include<vector>
#include<map>
#include<functional>
#include<list>
#include"Obj/Obj.h"
#include"Obj/ChipID.h"
#include"Obj/ChipGP.h"
#include"../common/vector2.h"

#include"BaseScene.h"

using MapChipGP = std::map<ChipID, ChipGP>;			//IDからGPを
using MapChipEneGP= std::map<ChipID_ENEMY, ChipGP>;

//メンバ参照用
#define lpMapCtl MapCtl::GetInstance()

#define LDR_ID_NAME "LDR_MAP_DATA"
#define LDR_ID_VER 0x01

struct DataHeader
{
	char fileID[13];			//ファイルのID情報
	unsigned char verID;		//ファイルのバージョン情報
	unsigned char Reserve1[2];	//アライメント分予約領域とする
	int sizeX;					//マップの横サイズ
	int sizeY;					//マップの縦サイズ
	unsigned char Reserve2[3];	//アライメント分予約領域とする
	unsigned char sum;			//SUM値
};

class MapCtl
{
public:
	static void Create(SceneID sceneID)
	{
		if (s_instance == nullptr)
		{
			s_instance = new MapCtl(sceneID);
		}
	}
	static void Destroy(void)
	{
		if (s_instance != nullptr)
		{
			delete s_instance;
		}
		s_instance = nullptr;
	}
	static MapCtl& GetInstance()
	{
		//Create();
		return (*s_instance);
	}

	bool SetMapData(Vector2 pos, ChipID id, ChipGP chipgp);	//マップデータのセット pos-座標（マス目ではない）
	bool SetMapData(Vector2 pos, ChipID id);				//マップデータのセット pos-座標（マス目ではない）
	bool SetMapData(Vector2 pos, ChipID_ENEMY eneid, ChipGP chipgp);		//敵のマップデータのセット
	bool SetMapData(Vector2 pos, ChipID_ENEMY eneid);

	ChipID GetMapDataGame(ChipGP gpID, unsigned int data);
	ChipID GetMapDataEdit(ChipGP gpID, unsigned int data);		//指定データからCHIPIDを取り出す。gpID-取得したいブロックのグループ。data-指定したマス目情報,ChipID-そのマスの情報
	ChipID GetMapData(ChipGP gpID, int x, int y, bool dig);	//指定マス目からデータを取り出してからCHIPIDを取り出す dig-digチェックをするか否か
	ChipID GetMapData(ChipGP gpID, Vector2 pos);			//指定座標からデータを取り出してから、CHIPIDを取り出す

	ChipID_ENEMY GetMapDataEne(ChipGP gpID, int x, int y);

	void SetCamera(std::weak_ptr<Camera> camera);		//カメラの座標をセット

	bool MapSave();						//マップのセーブ
	bool MapLoad();						//マップのロード(エディットシーン用)
	bool MapLoad(BaseScene& scene);		//マップのロード(ゲームシーン用)

	std::function<ChipID(ChipGP, unsigned int)> GetMapDataFunc;		//どちらのGetMapDataを使うか
	std::function<void(void)> DrawFunc;		//シーンごとの描画変数

private:
	void DrawEdit();		//エディットシーン用のdraw関数
	void DrawGame();		//ゲームシーン用のDraw関数
	void DrawMap();			//ゲームシーンもしくはdrawedit関数で内部的に呼び出す
	void DrawEditLine();	//エディットシーン用罫線

	MapCtl(SceneID sceneID);
	~MapCtl();

	bool Init();
	bool InitMap();
	void InitDrawChipEdit();		//DrawChipの初期化（エディットシーン用）
	void InitDrawChippGame();		//DrawChipの初期化（ゲームシーン用）

	static MapCtl* s_instance;
	SceneID sceneID_;

	Vector2 viewArea_;		//一画面に表示されるマップの広さ(チップ数)
	Vector2 worldArea_;		//ゲームの広さ（チップ数）
	Vector2 chipSize_;		//チップサイズ（座標）

	//idの指定で関数を呼び出せる
	std::map<ChipID, std::function<void(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)>> DrawChip_;
	std::map<ChipID_ENEMY, std::function<void(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)>> DrawChipEne_;
	//std::vector< std::vector<unsigned int>> mapID_;

	std::vector<unsigned int> basemapID_;		//間接参照配列すべて
	std::vector<unsigned int*> mapID_;			//間接参照用(二次元配列の縦)

	std::weak_ptr<Camera> camera_;

	MapChipGP chipGP_;		//idのブロックのグループを判別
	MapChipEneGP chipeneGP_;

	int animCnt_;			//アニメーションカウント

	friend class Camera;
	friend class GameScene;
};
