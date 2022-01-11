#pragma once
#include "imports.h"
#include <unordered_map>

class Peripherals
{
public:
	static std::unordered_map<WPARAM, bool> key_reg;
	
	static bool keyPressed(WPARAM);
	static void KeyPressIn(WPARAM p, bool type);
};
