#pragma once
#include "imports.h"
#include <unordered_map>
#include <sstream>
#include <string>
#include <Xinput.h>

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
	static bool Peripherals::controllerConnected(int i, XINPUT_STATE& state);
public:
	static void KeyPressIn(WPARAM p, bool type);
	static void MouseMoveIn(int, int);
	static void mouseClickLeftIn(int x, int y);
	static void mouseClickRightIn(int x, int y);
	static void queueState(bool);

	static bool keyPressed(WPARAM);
	static std::wstring unloadQueue();
	static bool queueUpdate();
	static X::Point mousePos();
	static bool mouseClickedLeft();
	static X::Point mouseClickLeftPos();
	static bool mouseClickedRight();
	static X::Point mouseClickRightPos();
	static X::PolarPoint controllerOutput(int i);
};
