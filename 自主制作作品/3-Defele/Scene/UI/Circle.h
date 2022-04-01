#pragma once
#include "UI.h"

class Circle :
	public UI
{
public:
	Circle(Vector2&& pos, Vector2&& size, MouseInputID mouseid = MouseInputID::MAX, const char* str = "");
	Circle(Vector2&& pos, Vector2&& size, std::function<bool(void)> function, MouseInputID mouseid, const char* str = "");
	~Circle();
private:

	void Update(void) override;
	void Draw(bool frame = false, int fontsize = 20, std::string = "") override;

	bool CheckColition(Vector2 otherpos) override;
};

