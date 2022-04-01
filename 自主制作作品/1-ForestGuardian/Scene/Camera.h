#pragma once
#include<DxLib.h>
#include<memory>
#include"../common/vector2.h"

class obj;

class Camera
{
public:
	Camera();
	~Camera();
	bool InitArea();
	bool SetTarget(std::weak_ptr<obj> target);		//カメラの対象をセット
	void UpDate();

	void Draw();

	const Vector2& GetPos();
	void SetPos(const Vector2& pos);
	const Vector2 GetDrawOffset();		//描画とカメラ位置によってずれる量の取得
private:
	std::weak_ptr<obj> target_;
	Vector2 pos_;
	Vector2 oldpos_;

	Vector2 defpos_;			//初期座標
	RECT antiMoveRect_;			//カメラの移動範囲
};


