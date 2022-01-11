#pragma once
#include "imports.h"
#include "vector"

using namespace std;


class Application
{
	HWND hWnd;
public:
	Application(HWND hWnd);
	void Paint(ID2D1HwndRenderTarget* pRT);
	void InitResources(ID2D1HwndRenderTarget* pRT);
	void DeinitResources();
	void tick(long tick);
};

