#include <DxLib.h>
#include "InputString.h"
#include "../SceneMng.h"
#include "../../common/FontMng.h"

constexpr double TOTALFADETIME = 1.5;

constexpr int INPUTSTRING_FONT_SIZE = 50;

void InputString::CreateFadeOutString(const char* str)
{
	SetString(str);
	totalTimeCnt_ = 0.0;
	alpha_ = 255;

	inputFlag_ = false;
}

void InputString::CreateString(PLACE place, std::string str, bool input)
{
	//入力があったときのみ
	if (input)
	{
		SetString(str);
		totalTimeCnt_ = 1000.0;
		alpha_ = 255;

		inputFlag_ = false;

		curPlace_ = place;
	}
}

void InputString::DissapperInputDraw(void)
{
	if (inputFlag_)
	{
		//入力時
		return;
	}

	auto fontHandle = lpFontMng.GetFontSizeHandle(FontType::DEFAULT, INPUTSTRING_FONT_SIZE);
	//フォントサイズの取得
	int fontSize = 0;
	GetFontStateToHandle(NULL, &fontSize, NULL, fontHandle);

	//文字の長さ
	int strWidth = GetDrawStringWidthToHandle(string_.c_str(), static_cast<int>(strlen(string_.c_str())),fontHandle);

	//|が見つかると改行
	if (static_cast<int>(string_.find_first_of("|")) != string_.npos)
	{
		//|の一つ前まで描画
		std::string beforestring = string_;
		beforestring.erase(static_cast<int>(beforestring.find_first_of("|")), static_cast<int>(beforestring.size()));

		strWidth = GetDrawStringWidthToHandle(beforestring.c_str(), static_cast<int>(strlen(beforestring.c_str())), fontHandle);;

		DrawFormatStringToHandle((lpSceneMng.GetViewArea().x_ - strWidth) / 2, (lpSceneMng.GetViewArea().y_ - fontSize * 2) / 2, 0xffffff, fontHandle, beforestring.c_str());

		//|の後描画
		std::string afterstring = string_;
		afterstring.erase(0, static_cast<int>(afterstring.find_first_of("|") + 1));

		strWidth = GetDrawStringWidthToHandle(afterstring.c_str(), static_cast<int>(strlen(afterstring.c_str())), fontHandle);;

		DrawFormatStringToHandle((lpSceneMng.GetViewArea().x_ - strWidth) / 2, (lpSceneMng.GetViewArea().y_) / 2, 0xffffff, fontHandle, afterstring.c_str());
	}
	else
	{
		DrawFormatStringToHandle((lpSceneMng.GetViewArea().x_ - strWidth) / 2, (lpSceneMng.GetViewArea().y_ - fontSize) / 2, 0xffffff, fontHandle, string_.c_str());
	}
}

void InputString::FedeOutDraw(void)
{
	if (TOTALFADETIME < totalTimeCnt_)
	{
		return;
	}

	auto fontHandle = lpFontMng.GetFontSizeHandle(FontType::DEFAULT, INPUTSTRING_FONT_SIZE);
	//フォントサイズの取得
	int fontSize = 0;
	GetFontStateToHandle(NULL, &fontSize, NULL, fontHandle);

	//α値減算
	alpha_ -= static_cast<int>(255 * lpSceneMng.GetDeltaTime());

	//画面中央にフェードアウトしながら表示
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
	
	//文字の長さ
	int strWidth = GetDrawStringWidthToHandle(string_.c_str(), static_cast<int>(strlen(string_.c_str())), fontHandle);

	DrawFormatStringToHandle((lpSceneMng.GetViewArea().x_ - strWidth) / 2, (lpSceneMng.GetViewArea().y_ - fontSize) / 2, 0xffffff, fontHandle, string_.c_str());
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	totalTimeCnt_ += lpSceneMng.GetDeltaTime();
}

InputString::InputString()
{
	totalTimeCnt_ = 100.0;
	alpha_ = 255;

	curPlace_ = PLACE::MAX;
}

InputString::~InputString()
{
}
