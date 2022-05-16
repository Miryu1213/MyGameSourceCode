#pragma once
#include<map>
#include<vector>
#include<string>
#include"vector2.h"

//シングルトンクラスだと他からアクセスするには必ずstaticから行う
#define lpImageMng ImageMng::GetInstance()

//vector<可変の配列の型>
using Vecint = std::vector<int>;

class ImageMng
{
public:
	static ImageMng& GetInstance()
	{
		static ImageMng s_instance;
		return s_instance;
	}
	//画像取得
	const Vecint& GetID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt);
	//一枚だけの画像取得 返り値は可変の配列変数。
	const Vecint& GetID(std::string key);

	//特殊グラフィックハンドル
	int atack1[3];
	int atack2[3];
private:
	ImageMng();
	~ImageMng();
	//std::map<読み込む型,変えたい型>
	std::map<std::string, Vecint> imgMap_;
};



