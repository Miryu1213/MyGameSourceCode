#pragma once
#include "UI.h"

class Box :
	public UI
{
public:
	Box(Vector2&& pos, Vector2&& size, MouseInputID mouseid = MouseInputID::MAX, std::string str = "");
	Box(Vector2&& pos, Vector2&& size, std::function<bool(void)> function, MouseInputID mouseid, std::string str = "");
	Box(Vector2&& pos, Vector2&& size, MouseInputID mouseid, std::function<bool(int)> function, ObjType objType);
	Box(Vector2&& pos, Vector2&& size, const char* mapname, MouseInputID mouseid, std::string str = "");

	~Box();
private:

	void Draw(bool frame = false, int fontsize = 20, std::string str = "") override;

	bool CheckColition(Vector2 otherpos) override;

	char* GetMapName(void) override
	{
		return mapName_;
	}
	char* mapName_;
};

