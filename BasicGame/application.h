#pragma once
#include "imports.h"
#include "vector"

using namespace std;


class Application
{
	HWND hWnd;
public:
	LPRECT WINSIZE;
	Application(HWND hWnd);
	void Paint(ID2D1HwndRenderTarget* pRT);
	void InitResources(ID2D1HwndRenderTarget* pRT, IDWriteFactory* pDWriteFactory);
	void DeinitResources();
	void tick(long tick);
	void onResize(int width, int height);
};

