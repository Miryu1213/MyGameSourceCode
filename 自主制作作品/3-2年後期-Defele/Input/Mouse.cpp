#include <DxLib.h>
#include "Mouse.h"

Mouse::Mouse()
{
	Init();
}

Mouse::~Mouse()
{
}

void Mouse::SetMouseInputPos(Vector2 pos)
{
	MouseInputdata_[MouseInputID::POSX] = pos.x_;
	MouseInputdata_[MouseInputID::POSY] = pos.y_;
}

MouseInputData Mouse::GetMouseData(InputType inputtype)
{
	if (inputtype == InputType::NOW)
	{
		return MouseInputdata_;
	}
	else
	{
		return oldMouseInputdata_;
	}
}

bool Mouse::Update(void)
{
	//oldへコピー
	for (auto& data : MouseInputdata_)
	{
		oldMouseInputdata_[data.first] = data.second;
	}

	//マウス座標の取得
	GetMousePoint(&MouseTbl_[MouseInputID::POSX], &MouseTbl_[MouseInputID::POSY]);

	//マウスが画面内時更新
	if (0 <= MouseTbl_[MouseInputID::POSX] && MouseTbl_[MouseInputID::POSX] <= Screensize_.x_ - Tole)
	{
		if (0 <= MouseTbl_[MouseInputID::POSY] && MouseTbl_[MouseInputID::POSY] <= Screensize_.y_ - Tole)
		{
			MouseInputdata_[MouseInputID::POSX] = MouseTbl_[MouseInputID::POSX];
			MouseInputdata_[MouseInputID::POSY] = MouseTbl_[MouseInputID::POSY];
		}
	}
	
	//マウスボタンの取得と代入
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		MouseInputdata_[MouseInputID::LEFT] = 1;
	}
	else
	{
		MouseInputdata_[MouseInputID::LEFT] = 0;
	}

	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
	{
		MouseInputdata_[MouseInputID::RIGHT] = 1;
	}
	else
	{
		MouseInputdata_[MouseInputID::RIGHT] = 0;
	}

	//ホイールの回転量の取得
	MouseInputdata_[MouseInputID::MIDDLE] = GetMouseWheelRotVol();
	
	return true;
}

void Mouse::Init(void)
{
	//入力情報登録と初期化
	MouseTbl_[MouseInputID::POSX] = 0;
	MouseTbl_[MouseInputID::POSY] = 0;
	MouseTbl_[MouseInputID::LEFT] = MOUSE_INPUT_LEFT;
	MouseTbl_[MouseInputID::RIGHT] = MOUSE_INPUT_RIGHT;
	MouseTbl_[MouseInputID::MIDDLE] = 0;

	//ボタン用　マウス側では使用しない
	MouseTbl_[MouseInputID::BUTTON_X] = 0;
	MouseTbl_[MouseInputID::BUTTON_X] = 0;

	//外部アクセス用のキーデータの初期化
	for (auto& tableData : MouseTbl_)
	{
		MouseInputdata_.emplace(tableData.first, 0);
	}

	GetScreenState(&Screensize_.x_, &Screensize_.y_, nullptr);
}
