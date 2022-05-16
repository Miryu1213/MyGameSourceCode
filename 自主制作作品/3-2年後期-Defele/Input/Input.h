#pragma once
#include <map>
#include <string>
#include "InputID.h"
#include "Mouse.h"

enum class InputClass
{
	KETBOARD,
	PAD
};

//キー入力データ情報
using InputData = std::map<InputID, int>;

//マウス入力データと座標
using MouseInputData = std::map<MouseInputID, int>;

class Input
{
public:
	Input();
	virtual ~Input();

	virtual bool Init(void) = 0;
	virtual bool Update(void) = 0;

	//入力情報の取得
	const InputData& GetInputData(InputType type);

	//マウス情報の取得
	const MouseInputData GetMouseData(InputType inputtype);
	//マウスへ座標をセット
	void SetMousePos(Vector2 pos);

	virtual InputClass GetInputClass(void) = 0;

protected:
	InputData Inputdata_;			//入力キー
	InputData oldInputdata_;		//ひとつ前の入力キー

	Mouse mouse_;
};


