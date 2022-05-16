#include <DxLib.h>
#include <cstdlib>
#include <sstream>
#include "TmxObj.h"
#include "../../common/ImageMng.h"

#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"

//パス用画像名追加
const std::string IMAGEPASS = "Scene/tmx/";

//ブロックサイズ
constexpr int DEF_BLOCK_SIZE = 64;

//同一タイル状の次の画像までのID数
constexpr int NEXT_IMAGE_TILE_ID = 32;

//プラントの種類数
constexpr int PLANT_TYPE_NUM = 3;

TmxObj::TmxObj()
{
	version_["1.6.0"] = 1;
	version_["1.7.0"] = 1;
	version_["1.7.2"] = 1;
}

TmxObj::~TmxObj()
{
}

bool TmxObj::LoadTMX(std::string fileName)
{
	//ファイルネームの代入
	rapidxml::file<char> xmlFileName = fileName.c_str();
	//ファイルをパーサーへ
	tmxDoc_.parse<0>(xmlFileName.data());									
	//ノード（今回はmap）を取得
	rapidxml::xml_node<char>* mapnode = tmxDoc_.first_node("map");			
	
	//バージョンチェック
	if (CheckTiledVersion(mapnode, "TMX") == false)
	{
		return false;
	}

	//各ノードの取得
	auto tileset = mapnode->first_node("tileset");

	//画像読み込みに必要なデータの読み込み
	Vector2 tilesize = { atoi(mapnode->first_attribute("tilewidth")->value()),atoi(mapnode->first_attribute("tileheight")->value()) };

	//列
	std::vector<int> columns;
	//行
	std::vector<int> row;
	//ファイル名
	std::string source;

	//300は絶対にたどり着かない数字
	for (int i = 0; i < 300; i++)
	{
		source = tileset->first_node("image")->first_attribute("source")->value();
		source = IMAGEPASS + source;

		//firstidの保存
		firstGID_.push_back(atoi(tileset->first_attribute("firstgid")->value()));

		std::string imageName = tileset->first_attribute("name")->value();
		if (IsPlant(imageName))
		{
			//施設とコアはfirstIdだけ読み込み画像は別に読み込む
			tileset = tileset->next_sibling();
			continue;
		}
		if (IsCore(imageName))
		{
			break;
		}

		//横（列）の要素数を取得-x
		columns.push_back(atoi(tileset->first_attribute("columns")->value()));
		//縦（行）の要素数の取得-y
		row.push_back(atoi(tileset->first_attribute("tilecount")->value()) / columns[i]);

		lpImageMng.GetID(source, firstGID_[i], tilesize, Vector2{ columns[i],row[i] });

		tileset = tileset->next_sibling();
	}

	//TMXの値の取り出し
	nextLayerId_ = atoi(mapnode->first_attribute("nextlayerid")->value());
	nextObjectId_ = atoi(mapnode->first_attribute("nextobjectid")->value());
	worldArea_ = { atoi(mapnode->first_attribute("width")->value()) ,atoi(mapnode->first_attribute("height")->value()) };
	tileSize_ = { atoi(mapnode->first_attribute("tilewidth")->value()) ,atoi(mapnode->first_attribute("tileheight")->value()) };

	return SetMap();
}

bool TmxObj::SetMap(void)
{
	//ノード（今回はmap）を取得
	rapidxml::xml_node<char>* mapnode = tmxDoc_.first_node("map");
	if (!mapnode)
	{
		TRACE("mapが存在しません\n");
		return false;
	}

	//データの取得しmapData_へ
	for (auto layernode = mapnode->first_node("layer"); layernode != nullptr; layernode = layernode->next_sibling())
	{
		std::string imageName = layernode->first_attribute("name")->value();

		auto datanode = layernode->first_node("data")->first_node();

		//ストリングストリーム型に変え、mapData_に代入
		std::stringstream dataStream;		        //マップデータのストリーム型
		dataStream << datanode->value();

		if (IsPlant(imageName) || IsCore(imageName))
		{
			//座標のセット
			SearcePlantorCoreAndSetPos(dataStream, imageName);
			continue;
		}

		auto layername = layernode->first_attribute("name")->value();
		//キーの代入
		if (mapData_.try_emplace(layername).second == true)
		{
			//データ部のリサイズ・代入
			mapData_[layername].resize(worldArea_.x_ * worldArea_.y_);
		}

		std::string numStr;
		for (auto& data : mapData_[layername])
		{
			numStr.clear();
			std::getline(dataStream, numStr, ',');	//取得（コンマ抜きの一つのマップの数字）
			data = atoi(numStr.c_str());
		}
	}
	return true;
}

void TmxObj::ResetTMX(void)
{
	for (auto& id : firstGID_)
	{
		lpImageMng.DeleteID(id);
	}

	firstGID_.clear();
	//次のレイヤーまでのID
	nextLayerId_ = 0;
	//次の画像までのID
	nextObjectId_ = 0;

	//TMXのマップデータ
	mapData_.clear();

	//施設の座標と種類
	//味方
	allyPlantVArray_.clear();
	//敵
	enemyPlantVArray_.clear();

	//コアの座標
	//味方
	allyCorePos_ = { 0,0 };
	//敵
	enemyCorePos_ = { 0,0 };
}

const std::vector<int> TmxObj::GetfirstGID(void)
{
	return firstGID_;
}

const unsigned int TmxObj::GetlayerSize(void)
{
	return nextLayerId_;
}

const Vector2& TmxObj::GetworldArea(void)
{
	return worldArea_;
}

const Vector2& TmxObj::GettileSize(void)
{
	return tileSize_;
}

const MapData& TmxObj::GetMapData(void)
{
	return mapData_;
}

const int TmxObj::GetMapData(std::string layer, Vector2 pos)
{
	//下の座標版
	return GetMapData(layer, pos.x_ / tileSize_.x_, pos.y_ / tileSize_.y_);
}

const int TmxObj::GetMapData(const std::string& layer, int x, int y)
{
	if (mapData_.count(layer) == 0)
	{
		//そのlayerが存在しないとき
		TRACE("レイヤー%sが存在しません\n", layer.c_str());
		return 0;
	}
	unsigned int point = y * worldArea_.x_ + x;
	if (0 <= point && point < mapData_[layer].size())
	{
		//範囲内の時
		return mapData_[layer][point];
	}
	else
	{
		TRACE("%d,%dは範囲外です\n", x, y);
		return 0;
	}
}

const std::vector<std::pair<ObjType, Vector2>> TmxObj::GetAllyPlantPos(void)
{
	std::vector<std::pair<ObjType, Vector2>> tmpAllyPlantVec;
	for (auto& plantPos : allyPlantVArray_)
	{
		tmpAllyPlantVec.push_back({ plantPos.first,plantPos.second * tileSize_ / Vector2{ DEF_BLOCK_SIZE,DEF_BLOCK_SIZE } });
	}

	return tmpAllyPlantVec;
}

const std::vector<std::pair<ObjType, Vector2>> TmxObj::GetEnemyPlantPos(void)
{
	std::vector<std::pair<ObjType, Vector2>> tmpEnemyPlantVec;
	for (auto& plantPos : enemyPlantVArray_)
	{
		tmpEnemyPlantVec.push_back({ plantPos.first,plantPos.second * tileSize_ / Vector2{ DEF_BLOCK_SIZE,DEF_BLOCK_SIZE } });
	}

	return tmpEnemyPlantVec;
}

const Vector2 TmxObj::GetAllyCorePos(void)
{
	return allyCorePos_ * tileSize_ / Vector2{ DEF_BLOCK_SIZE,DEF_BLOCK_SIZE };
}

const Vector2 TmxObj::GetEnemyCorePos(void)
{
	return enemyCorePos_ * tileSize_ / Vector2{ DEF_BLOCK_SIZE,DEF_BLOCK_SIZE };
}

bool TmxObj::IsOutOfRange(Vector2 blockPos)
{
	//一次元に変更
	auto point = blockPos.x_ + blockPos.y_ * worldArea_.x_;

	for (auto& layer : mapData_)
	{
		if (0 <= point && point < mapData_[layer.first].size())
		{
			//範囲内の時
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void TmxObj::DrawMap(void)
{
	//現在のレイヤーID
	int id = 0;
	//id - layerIdの最小ナンバー
	int minlayerid = 5000;
	//決定レイヤー
	int decLayer = 0;
	
	//マップデータの描画
	for (auto layer = --mapData_.end(); layer != --mapData_.begin(); layer--)
	{
		for (int y = 0; y < worldArea_.y_; y++)
		{
			for (int x = 0; x < worldArea_.x_; x++)
			{
				id = GetMapData(layer->first, x, y);

				//idが0の場合
				if (id == 0)
				{
					continue;
				}

				minlayerid = 5000;

				for (auto layerId : firstGID_)
				{
					//id-layeridがもっとも小さいものをlayeridとする
					if (id - layerId < minlayerid && 0 <= id - layerId)
					{
						minlayerid = id - layerId;
						decLayer = layerId;
					}
				}
				//ImageMng側が0から始まっているためfistid分引く
				id = id - decLayer;

				DrawGraph(x * tileSize_.x_, y * tileSize_.y_, lpImageMng.GetID(decLayer)[id], true);
			}
		}
	}
}

bool TmxObj::CheckTiledVersion(rapidxml::xml_node<>* node, const char* string)
{
	//NULLチェック
	if (!node)
	{
		TRACE("%s:nodeが存在しません\n", string);
		return false;
	}
	auto tiledversion = node->first_attribute("tiledversion");
	if (!tiledversion)
	{
		TRACE("%s:tiledversionが存在しません\n", string);
		return false;
	}
	//バージョンチェック
	if (!(version_.count(tiledversion->value())))
	{
		//一致するものがないとき
		TRACE("%s非対応バージョン\n", string);
		return false;
	}
	if (!version_[tiledversion->value()])
	{
		//二重チェック
		TRACE("%s非対応バージョン\n",string);
		return false;
	}
	TRACE("%s対応バージョン\n", string);
	return true;
}

ObjType TmxObj::ConvertObjType(int id)
{
	//施設のIDの場合
	int plantId = *(--(--firstGID_.end()));
	int plantNextId = firstGID_.back();

	if (!(plantId <= id && id < plantNextId))
	{
		//コアの場合
		return ObjType::CORE;
	}

	//IDの計算
	id -= plantId;
	int objId = id / NEXT_IMAGE_TILE_ID;

	//敵の場合もあるため
	
	ObjType tmpobjtype;

	switch (objId)
	{
	case 0:
		tmpobjtype = ObjType::KNIGHTPLANT;
		break;
	case 1:
		tmpobjtype = ObjType::ARCHERPLANT;
		break;
	case 2:
		tmpobjtype = ObjType::WARRIORPLANT;
		break;
	default:
		assert(!"不正なIDです");
		break;
	}

	return tmpobjtype;
}

bool TmxObj::IsAlly(std::string str)
{
	return str.find("ally") != str.npos;
}

bool TmxObj::IsPlant(std::string str)
{
	return str.find("plant") != str.npos;
}

bool TmxObj::IsCore(std::string str)
{
	return str.find("core") != str.npos;
}

void TmxObj::SearcePlantorCoreAndSetPos(std::stringstream& datastream, std::string str)
{
	std::string numStr;

	int cnt = 0;
	while (std::getline(datastream, numStr, ','))
	{
		//第一条件と第三条件は初めのリテラルに記載されている文字　
		if ((numStr != "\r\n0") && (numStr != "0") && (numStr != "0\r\n"))
		{
			//施設かコアを見つけた場合
			SetPlantorCorePos(str, cnt, ConvertObjType(atoi(numStr.c_str())));
		}
		cnt++;
	}
}

void TmxObj::SetPlantorCorePos(std::string str ,int cnt , ObjType objtype)
{
	Vector2 pos = Vector2(cnt % worldArea_.x_, cnt / worldArea_.x_);
	Vector2 blockPos = pos * tileSize_ / Vector2{ DEF_BLOCK_SIZE,DEF_BLOCK_SIZE };

	//その値がすでに格納されている場合弾く
	if (std::find_if(allyPlantVArray_.begin(), allyPlantVArray_.end(),[&](std::pair<ObjType,Vector2> vec) { return vec.second * tileSize_ / Vector2{ DEF_BLOCK_SIZE,DEF_BLOCK_SIZE } == blockPos; }) != allyPlantVArray_.end())
	{
		return;
	}
	if (std::find_if(enemyPlantVArray_.begin(), enemyPlantVArray_.end(), [&](std::pair<ObjType, Vector2> vec) { return vec.second * tileSize_ / Vector2{ DEF_BLOCK_SIZE,DEF_BLOCK_SIZE } == blockPos; }) != enemyPlantVArray_.end())
	{
		return;
	}


	if (IsPlant(str))
	{
		//施設
		if (IsAlly(str))
		{
			//味方
			allyPlantVArray_.push_back({ objtype , pos });
		}
		else
		{
			//敵
			enemyPlantVArray_.push_back({ objtype , pos });
		}
	}
	else
	{
		//コア
		if (IsAlly(str))
		{
			//味方
			allyCorePos_ = pos;
		}
		else
		{
			//敵
			enemyCorePos_ = pos;
		}
	}
}
