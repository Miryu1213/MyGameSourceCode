#pragma once
#include <map>

#define lpFontMng FontMng::GetInstance()

enum class FontType
{
	DEFAULT,
	MAX
};

class FontMng
{
public:
	static FontMng& GetInstance(void)
	{
		static FontMng s_Instance;
		return s_Instance;
	}

	const int& GetFontSizeHandle(FontType fonttype, int size);
	void DeleteFontHandle(FontType fonttype, int size);

private:
	FontMng();
	~FontMng();

	//�t�H���g�^�C�v�ƃT�C�Y�ɉ������n���h���܂Ƃ�
	std::map<FontType, std::map<int, int>> fontHandles;
};

