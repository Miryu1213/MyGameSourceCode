#pragma once
#include"BaseScene.h"

//lp　LongPtr
#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	//静的なシングルトン
	static SceneMng& GetInstance(void)
	{
		static SceneMng s_Instance;
		return s_Instance;
	};

	//色
	constexpr static UINT32 Blue = 0x0000ff;
	constexpr static UINT32 Red = 0xff0000;
	constexpr static UINT32 Black = 0x000000;
	constexpr static UINT32 Purple = 0xff00ff;

	void Run(void);

	//経過時間の取得
	double GetDeltaTime(void)
	{
		return delta_ * doubleValue_;
	}

	//合計経過時間の取得
	double GetTotalDeltaTime(void)
	{
		return totaldelta_;
	}

	//表示エリア
	Vector2 GetViewArea(void) 
	{ 
		return viewArea_;
	}

	//全体サイズ
	Vector2 GetWorldSize(void) 
	{ 
		return worldSize_;
	}
	void SetWorldSize(Vector2 worldsize);

	//オフセット
	int GetOffset(void) 
	{
		return offset_;
	}
	//オフセット加算済みカーソルの座標を返す(カメラのオフセットではない)
	Vector2 GetCurPosAddOffset(void);

	void FinishGame(void)
	{
		gameEndFlag_ = true;
	}

	//カーソル(入力機器まとめ)
	std::shared_ptr<Cursol> cursol_;

	//倍速
	double doubleValue_;
	int alpha_;

	//マップ状況
	std::map<std::string, bool> clearMap_;
	
private:
	SceneMng();
	~SceneMng();

	//システム系初期化
	bool SysInit(void);

	bool Init(void);
	void UpDate(void);
	void Draw(void);

	//描画範囲
	Vector2 viewArea_;
	//マップ全体のサイズ
	Vector2 worldSize_;
	//画面ずらし用オフセット
	int offset_;

	//シーンの宣言
	UniqueScene scene_;

	//経過時間
	double delta_;

	//シーン開始後からの経過時間
	double totaldelta_;

	bool gameEndFlag_;
};
