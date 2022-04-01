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

	//���͌���ł���`��
	void DissapperInputDraw(void);

	//���X�Ƀt�F�[�h�A�E�g���Ă����`��
	void FedeOutDraw(void);

private:
	InputString();
	~InputString();

	//������
	std::string string_;

	//���͎�
	bool inputFlag_;

	//���݂̕\����
	PLACE curPlace_;

	//�o�ߎ��ԃJ�E���g
	double totalTimeCnt_;
	//��
	int alpha_;
};

