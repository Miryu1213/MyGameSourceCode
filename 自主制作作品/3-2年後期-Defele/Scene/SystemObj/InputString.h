#pragma once
#include <string>
#include "../UI/Place.h"

#define lpInputString InputString::GetInstance()

class InputString
{
public:
	static InputString& GetInstance(void)
	{
		static InputString s_Instance_;
		return s_Instance_;
	}

	std::string GetString(void)
	{
		return string_;
	}
	void SetString(std::string str)
	{
		string_ = str;
	}

	PLACE GetNowPlace(void)
	{
		return curPlace_;
	}
	void SetNowPlace(PLACE place)
	{
		curPlace_ = place;
	}

	void CreateFadeOutString(const char* str);

	void CreateString(PLACE place, std::string str, bool input);

	//入力後消滅する描画
	void DissapperInputDraw(void);

	//徐々にフェードアウトしていく描画
	void FedeOutDraw(void);

private:
	InputString();
	~InputString();

	//文字列
	std::string string_;

	//入力時
	bool inputFlag_;

	//現在の表示物
	PLACE curPlace_;

	//経過時間カウント
	double totalTimeCnt_;
	//α
	int alpha_;
};

