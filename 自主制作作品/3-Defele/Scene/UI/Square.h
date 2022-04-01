#pragma once
#include "UI.h"

class Square :
	public UI
{
public:
	Square(Vector2&& leftUpPos, Vector2&& leftDownPos, Vector2&& rightUpPos, Vector2&& rightDownPos,
		Vector2&& size, const char* mapname, MouseInputID mouseid, std::string str = "");

	~Square();
private:

	void Update(void) override;
	void Draw(bool frame = false, int fontsize = 20, std::string str = "") override;

	bool CheckColition(Vector2 otherpos) override;

	char* GetMapName(void) override
	{
		return mapName_;
	}
	char* mapName_;

	Vector2 leftUpPos_;
	Vector2 leftDownPos_;
	Vector2 rightUpPos_;
	Vector2 rightDownPos_;
};

