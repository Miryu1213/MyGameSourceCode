#pragma once
#include <string>
#include <map>
#include <vector>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "../Obj/Type/ObjType.h"
#include "../../common/Vector2.h"

#define lptmxobj TmxObj::GetInstance()

using MapData = std::map<std::string, std::vector<int>>;	//vectorはTMXのマップ

class TmxObj
{
public:

	static TmxObj& GetInstance(void)
	{
		static TmxObj s_Instance;
		return s_Instance;
	}

	//TMXのロード　引数は読み込むファイル
	bool LoadTMX(std::string fileName);

	//TMXのマップ情報をセット
	bool SetMap(void);

	//リセット
	void ResetTMX(void);

	const std::vector<int> GetfirstGID(void);
	const unsigned int GetlayerSize(void);
	const Vector2& GetworldArea(void);
	const Vector2& GettileSize(void);

	//マップデータの取得
	//通常
	const MapData& GetMapData(void);
	//layerのposの位置(座標)のIDを返す
	const int GetMapData(std::string layer,Vector2 pos);
	//ブロック座標のIDを返す
	const int GetMapData(const std::string& layer, int x, int y);

	//プラントの座標をブロック値で返す
	const std::vector<std::pair<ObjType, Vector2>> GetAllyPlantPos(void);
	const std::vector<std::pair<ObjType, Vector2>> GetEnemyPlantPos(void);
	//コアの座標をブロック値で返す
	const Vector2 GetAllyCorePos(void);
	const Vector2 GetEnemyCorePos(void);

	//座標が範囲外か判定
	bool IsOutOfRange(Vector2 blockPos);

	//マップの描画
	void DrawMap(void);

private:
	TmxObj();
	~TmxObj();

	//引数 versionがあるノード
	bool CheckTiledVersion(rapidxml::xml_node<>* node, const char* string);

	//IDをオブジェクトタイプへ変換
	ObjType ConvertObjType(int id);

	//味方チェック
	bool IsAlly(std::string str);

	//プラントチェック
	bool IsPlant(std::string str);
	//コアチェック
	bool IsCore(std::string str);
	//プラントかコアの探索
	void SearcePlantorCoreAndSetPos(std::stringstream& datastream, std::string str);
	//プラントかコアの座標のセット
	void SetPlantorCorePos(std::string str ,int cnt, ObjType objtype);

	//Tiledのバージョン管理用
	std::map<std::string, int> version_;

	//TMX
	//TMXのドキュメント保存用
	rapidxml::xml_document<> tmxDoc_;
	//タイルセットの最初(左上)のグローバルタイルID (firstgid)
	std::vector<int> firstGID_;
	//次のレイヤーまでのID
	unsigned int nextLayerId_;
	//次の画像までのID
	unsigned int nextObjectId_;
	//全体のサイズ(map width height)
	Vector2 worldArea_;
	//タイルのサイズ(tile width height)
	Vector2 tileSize_;

	//TMXのマップデータ
	MapData mapData_;

	//施設の座標と種類
	//味方
	std::vector<std::pair<ObjType,Vector2>> allyPlantVArray_;
	//敵
	std::vector<std::pair<ObjType, Vector2>> enemyPlantVArray_;

	//コアの座標
	//味方
	Vector2 allyCorePos_;
	//敵
	Vector2 enemyCorePos_;
};

