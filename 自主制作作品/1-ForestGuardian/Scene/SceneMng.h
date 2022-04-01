#pragma once
#include<memory>
#include"BaseScene.h"
#include"../common/vector2.h"

#define lpSceneMng SceneMng::GetInstance()
class SceneMng		//全体のシーンの管理。無限ループ。各サイズ等
{
public:
	static SceneMng& GetInstance(void)
	{
		//その時だけstaticでグローバル化し,すぐリターンで返すことでインスタンスを局所で抑えられる。
		static SceneMng s_Instance;
		return s_Instance;
	}

	void Run();
	void Draw();

	bool outkey;

	const Vector2 scnArea_;			//画面解像度
	const Vector2 worldArea_;		//ゲーム世界全体ドット
	const Vector2 viewArea_;		//表示ドット数
	const Vector2 PlayerSize_;		//プレイヤーサイズ
private:
	uniqueBaseScene scene_;		//各シーンの管理変数

	void Init();
	bool SysInit(void);		//DXlibとかその辺の初期化
	SceneMng();
	~SceneMng();
};


