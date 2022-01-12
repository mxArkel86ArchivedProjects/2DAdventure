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
	static bool mouseclicked;
	static int mouseclick_x;
	static int mouseclick_y;
public:
	
	static bool keyPressed(WPARAM);
	static void KeyPressIn(WPARAM p, bool type);
	static void MouseMoveIn(int, int);

	static std::wstring unloadQueue();
	static void queueState(bool);
	static bool queueUpdate();
	static X::Point mousePos();
	static void mouseClickIn(int x, int y);
	static bool mouseClicked();
	static X::Point mouseClickPos();
};
