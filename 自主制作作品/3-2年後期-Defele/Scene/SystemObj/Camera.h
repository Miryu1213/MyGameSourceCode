#pragma once
#include <DxLib.h>
#include <memory>
#include "../../common/vector2.h"

class Cursol;

class Camera
{
public:
	Camera();
	~Camera();

	//描画位置設定
	bool InitArea(void);

	//カメラの対象をセット
	bool SetTarget(std::weak_ptr<Cursol> target);

	void UpDate(void);
	//チュートリアル時の更新
	void UpDateTutorial(void);

	void Draw(void);

	const Vector2& GetPos();
	void SetPos(const Vector2& pos);

	void SetCoreBreakedMode(Vector2 targetPos);
	void UpDateCoreBreaked(void);

	//描画とカメラ位置によってずれる量の取得
	const Vector2 GetDrawOffset();
private:
	std::weak_ptr<Cursol> target_;
	Vector2 pos_;

	Vector2 defpos_;			//初期座標
	RECT antiMoveRect_;			//カメラの移動範囲

	//範囲外
	bool outofRangeFlag_;

	//コア破壊時
	bool breakedCoreFlag_;
	Vector2 breakedCorePos_;
	Vector2 beforePos_;

	//時間
	double time_;
};
