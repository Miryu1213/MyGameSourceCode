#pragma once
#include <DxLib.h>
#include <memory>
#include <list>
#include <vector>
#include "../../Input/Mouse.h"
#include "../../common/Vector2.h"

//行動制限許容範囲
constexpr int Tolerance = 20;

//トレイル限界数
constexpr int trailLimit = 15;

//トレイル関係
constexpr int TraiSize = 12;
constexpr int TrailSizeH = TraiSize / 2;

//水面
//水面最大表示時間
constexpr double WaterSurfaceTime = 0.6;

class Input;

class Cursol
{
public:
	Cursol();
	~Cursol();

	void Init(void);
	void Update(void);
	void Draw(bool isTutorial);

	Vector2 GetPos(void) { return pos_; };

	//トレイルのフルクリア
	void TrailClear(void);

	MouseInputData GetMouseInput(InputType inputtype);

private:
	Vector2 pos_;
	int speed_;

	Vector2 size_;

	//入力情報
	std::unique_ptr<Input> input_;

	//キーボード処理があるか否か
	bool keyChecker;

	//画面サイズ
	Vector2 Screensize_;

	//トレイルリスト
	std::list<Vector2Flt> trailList_;
	//トレイル用ポリゴン保存
	VERTEX2D Vert[trailLimit * 6];

	//水面
	std::vector<Vector2> waterPosVec_;
	std::vector<double> waterTimeVec_;
};

