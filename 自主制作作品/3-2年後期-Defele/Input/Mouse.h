#pragma once
#include <map>
#include "../common/Vector2.h"
#include "MouseInputID.h"

//行動制限許容範囲
constexpr int Tole = 18;

//マウス入力データと座標
using MouseInputData = std::map<MouseInputID, int>;

class Mouse
{
public:
	Mouse();
	~Mouse();

	bool Update(void);
	void Init(void);
	
	void SetMouseInputPos(Vector2 pos);
	MouseInputData GetMouseData(InputType inputtype);

private:
	std::map<MouseInputID, int> MouseTbl_;	//マウステーブル

	MouseInputData MouseInputdata_;			//マウス情報
	MouseInputData oldMouseInputdata_;		//前フレームのマウス情報

	Vector2 Screensize_;					//画面サイズ
};

