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
public:
	
	static bool keyPressed(WPARAM);
	static void KeyPressIn(WPARAM p, bool type);

	static std::wstring unloadQueue();
	static void queueState(bool);
	static bool queueUpdate();
};
