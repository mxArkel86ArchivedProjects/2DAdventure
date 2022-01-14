#pragma once
#include "imports.h"
#include "vector"

using namespace std;


class Application
{
	HWND hWnd;
	ID2D1HwndRenderTarget* pRT = NULL;
public:
	Application();
	Application(HWND hWnd, ID2D1HwndRenderTarget* pRT);
	void Paint();
	void InitResources(IDWriteFactory* pDWriteFactory, IWICImagingFactory *pFactory);
	void DeinitResources();
	void tick(long tick);
	void onResize(int width, int height);
	void InitColor(string name, D2D1_COLOR_F color);
	wstring runConsoleCommand(wstring cmd);
	void InputProcessing();
};

