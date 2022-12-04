#pragma once
#include "imports.h"
#include "ColorRect.h"
#include "LevelProp.h"
#include "Collider.h"
#include "ResetBox.h"
#include "X.h"

using namespace X;
using namespace std;

class ConsoleLine
{
	wstring text;
	int owner;

public:
	ConsoleLine(int owner, wstring text);
	wstring getText();
	int getOwner();
};

class Application
{
	HWND hWnd;
	ID2D1HwndRenderTarget* pRT = NULL;
	IWICImagingFactory *pWICFactory = NULL;
	IDWriteFactory* pDWriteFactory = NULL;
public:
	bool showprops = true;
	bool debugview = true;
	bool noclip = true;
	int player_size = PLAYER_SIZE_DEBUG;
	int gridsize = GRIDSIZE_DEBUG;
	string currentcolor = "black";
	string currentasset = "void";
	
	X::Point location = X::Point(0,0);
	Rect PLAYER_SCREEN_LOC;
	Rect SCREEN_BOUNDS;
	Rect LEVEL_BOUNDS;

	vector<ConsoleLine> console_history = vector<ConsoleLine>();

	std::unordered_map<string, ID2D1Bitmap*> resources = std::unordered_map<string, ID2D1Bitmap*>();
	std::unordered_map<string, ID2D1SolidColorBrush*> colors = std::unordered_map<string, ID2D1SolidColorBrush*>();
	std::unordered_map<string, Point*> checkpoints = std::unordered_map<string, Point*>();

	vector<Collider*> colliders = vector<Collider*>();
	vector<ColorRect*> colorrect = vector<ColorRect*>();
	vector<LevelProp*> props = vector<LevelProp*>();
	vector<ResetBox*> resetboxes = vector<ResetBox*>();
	
	vector<LevelProp*> newprops = vector<LevelProp*>();
	vector<ColorRect*> newcolorrect = vector<ColorRect*>();
	vector<Collider*> newcolliders = vector<Collider*>();
	


	
	
	Application();
	Application(HWND hWnd, ID2D1HwndRenderTarget* pRT, IWICImagingFactory *pWICFactory, IDWriteFactory* pDWriteFactory);
	void Paint();
	void InitResources();
	void DeinitResources();
	void tick(long tick);
	void onResize(int width, int height);
	void InitBitmap(string name, wstring path, int size);
	void InitColor(string name, D2D1_COLOR_F color);
	void saveconfig(wstring path);
	void readconfig(wstring path);
	void setPlayerLocFromSchemPoint(X::Point p);
	wstring runConsoleCommand(wstring cmd);
	void InputProcessing();
};

