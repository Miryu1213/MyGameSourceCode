#pragma once
#include <DxLib.h>
#include <memory>
#include <list>
#include <vector>
#include "../../Input/Mouse.h"
#include "../../common/Vector2.h"

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

	//水面
	std::vector<Vector2> waterPosVec_;
	std::vector<double> waterTimeVec_;
};

