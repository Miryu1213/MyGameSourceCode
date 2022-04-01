#pragma once
#include <vector>
#include <string>
#include <map>
#include "../Scene/Obj/Type/ObjType.h"
#include "../Scene/Obj/Type/ArmyType.h"
#include "Vector2.h"

#define lpImageMng ImageMng::GetInstance()

using VecInt = std::vector<int>;

enum class TOCOLOR
{
	RED,
	BLUE,
	MAX
};

enum class CHECKCOLOR
{
	RED,
	GREEN,
	BLUE,
	MAX
};

class ImageMng
{
public:
	static ImageMng& GetInstance()
	{
		static ImageMng s_Instance;
		return s_Instance;
	}
	//画像呼び出し用。引数はファイルを呼び出すキー 返り値はVectorのint
	const VecInt& GetID(const std::string& key);
	//上記のint版
	const VecInt& GetID(int key);
	//f_nameとkeyを別にするため
	const VecInt& GetID(std::string f_name, std::string key);
	//分割用
	const VecInt& GetID(std::string f_name, std::string key, Vector2 divsize, Vector2 divcnt);
	//キーがint
	const VecInt& GetID(std::string f_name, int key, Vector2 divsize, Vector2 divcnt);
	//連続したIDのセット
	void SetID(std::string f_name, std::string key);
	//このキーがの中身が存在するか
	bool IsKey(std::string key);

	void GraphBlightFilter(const std::string& key, int CmpType, int CmpParam, int ClipFillFlag, unsigned int ClipFillColor, int ClipFillAlpha);

	//ドット単位で画像にアクセス
	const VecInt& GetDotID(std::string f_name, std::string key);
	const VecInt& GetDotID(std::string key);
	const VecInt& GetDotID(std::string f_name, std::string key, Vector2 divsize, Vector2 divcnt);

	//読み込んだ画像の色の変更
	void SetColorAndID(const int fromColor, const TOCOLOR toColor, const std::string& f_name, const std::string& key, Vector2 divsize, Vector2 divcnt);
	//読み込んだ画像の色の変更(全て)
	void SetAllColorAndID(const TOCOLOR toColor, const std::string& f_name, const std::string& key, Vector2 divsize, Vector2 divcnt);

	bool IsDotKey(std::string key);

	//指定したキーのロードを削除
	void DeleteID(const std::string& key);
	void DeleteID(int key);

	//オブジェクトタイプをキーネームへ変換
	std::string ConvertObjTypeToKey(ObjType objtype, ArmyType armytype);

private:
	ImageMng();
	~ImageMng();
	//分割する可能性があるため、Vecter<int>を用いる
	std::map<std::string, VecInt> imageMap_;

	//ドット単位での画像ハンドル
	std::map<std::string, VecInt> dotImageMap_;
};

