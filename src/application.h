#pragma once
#include "imports.h"
#include "vector"

using namespace std;


class Application
{
	HWND hWnd;
	ID2D1HwndRenderTarget* pRT = NULL;
	IWICImagingFactory *pWICFactory = NULL;
public:
	Application();
	Application(HWND hWnd, ID2D1HwndRenderTarget* pRT, IWICImagingFactory *pWICFactory);
	void Paint();
	void InitResources(IDWriteFactory* pDWriteFactory);
	void DeinitResources();
	void tick(long tick);
	void onResize(int width, int height);
	void InitBitmap(string name, wstring path, int size);
	void InitColor(string name, D2D1_COLOR_F color);
	wstring runConsoleCommand(wstring cmd);
	void InputProcessing();
	void init();
};

