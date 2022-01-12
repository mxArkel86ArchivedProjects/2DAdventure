#pragma once
#include "imports.h"
#include <unordered_map>
#include <sstream>
#include <string>

class Peripherals
{
	static bool queuestate;
	static bool queueupdate;
	static std::wstringstream characterqueue;
	static std::unordered_map<WPARAM, bool> key_reg;
	static int mousex;
	static int mousey;
	static bool mouseclicked_l;
	static bool mouseclicked_r;
	static int mouseclick_xr;
	static int mouseclick_yr;
	static int mouseclick_xl;
	static int mouseclick_yl;
public:
	
	static bool keyPressed(WPARAM);
	static void KeyPressIn(WPARAM p, bool type);
	static void MouseMoveIn(int, int);

	static std::wstring unloadQueue();
	static void queueState(bool);
	static bool queueUpdate();
	static X::Point mousePos();
	static void mouseClickLeftIn(int x, int y);
	static bool mouseClickedLeft();
	static X::Point mouseClickLeftPos();
	static void mouseClickRightIn(int x, int y);
	static bool mouseClickedRight();
	static X::Point mouseClickRightPos();
};
