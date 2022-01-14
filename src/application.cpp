#include "application.h"
#include "X.h"
#include "util.h"
#include "Peripherals.h"
#include <vector>
#include "GameObject.h"
#include "CollisionUtil.h"
#include <string>
#include "LevelProp.h"
#include "ImageUtil.h"
#include "ConfigMgr.h"
#include "ColorRect.h"
#include <iostream>

#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/predicate.hpp"
using namespace boost;
using namespace std;

#include <codecvt>
#include <locale>

IDWriteTextFormat *console_txt;
IDWriteTextFormat *debug_txt;

using convert_type = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_type, wchar_t> converter;

#define PLAYER_SIZE_ 100
#define PLAYER_SIZE_DEBUG 60
#define GRIDSIZE_ 40
#define GRIDSIZE_DEBUG 30
#define PLAYER_SPEED 2.2
#define GRAV_CONST 0.11
#define JUMP_INTENSITY 3.8
#define CONSOLE_TEXT_SIZE 16
#define CONSOLE_TEXT_SPACING 4
#define RESOURCE_SIZE 512

X::Point location;
double x1 = 0;
bool noclip = true;
double vertical_velocity = 0;
bool console_up = false;
bool console_toggle = true;
bool backspace_toggle = true;
wstring current_command = L"";
X::Point firstpoint = X::Point();
X::Point secondpoint = X::Point();
bool firstpoint_b = true;
int editmode = 0;
double select_z = 1;
bool select_z_toggle = true;
wstring APP_PATH = L"C:\\Users\\nicho\\source\\repos\\BasicGame\\build\\BasicGame\\Debug";
string currentasset = "pat";
string currentcolor = "black";
bool DEBUGVIEW = true;
double GRIDSIZE = GRIDSIZE_DEBUG;
double PLAYER_SIZE = PLAYER_SIZE_DEBUG;
bool SHOWPROPS = true;

std::unordered_map<string, ID2D1Bitmap *> resources = std::unordered_map<string, ID2D1Bitmap *>();
std::unordered_map<string, ID2D1SolidColorBrush *> colors = std::unordered_map<string, ID2D1SolidColorBrush *>();

void addResource(string name, ID2D1Bitmap *&ptr)
{
	resources.insert_or_assign(name, ptr);
}
void removeResource(string name)
{
	resources.erase(name);
}
ID2D1Bitmap *getResource(string name)
{
	return resources[name];
}

void addColor(string name, ID2D1SolidColorBrush *&ptr)
{
	colors.insert_or_assign(name, ptr);
}
void removeColor(string name)
{
	colors.erase(name);
}
ID2D1SolidColorBrush *getColor(string name)
{
	return colors[name];
}

void Application::InitColor(string name, D2D1_COLOR_F color)
{
	ID2D1SolidColorBrush *br = NULL;
	const D2D1_COLOR_F c(color);
	pRT->CreateSolidColorBrush(c, &br);
	addColor(name, br);
}

X::Rect schemToLocalZ(X::Rect r, double z, int width, int height);
X::Rect schemToLocal(X::Rect r);
X::Point schemToLocalPoint(X::Point r);
class ConsoleLine
{
	wstring text;
	int owner;

public:
	ConsoleLine(int owner, wstring text);
	wstring getText();
	int getOwner();
};
void InputProcessing();

vector<ConsoleLine> console_history = vector<ConsoleLine>();
vector<GameObject *> colliders = vector<GameObject *>();
vector<GameObject *> newcolliders = vector<GameObject *>();
vector<LevelProp *> props = vector<LevelProp *>();
vector<LevelProp *> newprops = vector<LevelProp *>();
vector<ColorRect *> colorrect = vector<ColorRect *>();
vector<ColorRect *> newcolorrect = vector<ColorRect *>();

X::Rect PLAYER_SCREEN_LOC;
X::Rect SCREEN_BOUNDS;

inline bool strcompare(const wchar_t *a, const wchar_t *b)
{
	return _wcsnicmp(a, b, max(wcslen(a), wcslen(b))) == 0;
}

wstring Application::runConsoleCommand(wstring cmd)
{
	vector<wstring> args; // #2: Search for tokens
	split(args, cmd, is_any_of(" "), token_compress_on);
	if (strcompare(cmd.c_str(), L"clip"))
	{
		noclip = !noclip;
		wstring resp = noclip ? L"Successfully enabled noclip" : L"Successfully disabled noclip";
		return resp;
	}if (strcompare(cmd.c_str(), L"props"))
	{
		SHOWPROPS = !SHOWPROPS;
		wstring resp = SHOWPROPS ? L"Successfully enabled props" : L"Successfully disabled props";
		return resp;
	}else if (strcompare(cmd.c_str(), L"debug"))
	{
		DEBUGVIEW = !DEBUGVIEW;
		if(DEBUGVIEW){
			PLAYER_SIZE = PLAYER_SIZE_DEBUG;
			GRIDSIZE = GRIDSIZE_DEBUG;
			onResize(SCREEN_BOUNDS.right(), SCREEN_BOUNDS.bottom());
			return L"Successfully enabled debugview";
		}else{
			PLAYER_SIZE = PLAYER_SIZE_;
			GRIDSIZE = GRIDSIZE_;
			onResize(SCREEN_BOUNDS.right(), SCREEN_BOUNDS.bottom());
			return L"Successfully disabled debugview";
		}
	}
	else if (strcompare(cmd.c_str(), L"clear"))
	{
		console_history.clear();
		return L"Successfully cleared console";
	}
	else if (strcompare(cmd.c_str(), L"save"))
	{
		saveconfig(APP_PATH + L"\\output.txt", newcolliders, newprops, newcolorrect, colliders, props, colorrect);
		return L"Successfully saved assets";
	}
	else if (starts_with(cmd, "asset"))
	{
		wstring ws = args[1];
		currentasset = converter.to_bytes(ws);
		wstring str = L"Set current asset to " + args[1];
		return str;
	}else if (starts_with(cmd, "color"))
	{
		wstring ws = args[1];
		currentcolor = converter.to_bytes(ws);
		wstring str = L"Set current color to " + args[1];
		return str;
	}
	else
		return L"";
}

void Application::Paint()
{
	pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

	
	
	for (ColorRect *p : colorrect)
	{
		Rect r = p->rect();
			X::Rect obj = schemToLocal(r);
			if (!CollisionUtil::staticCollision(obj, SCREEN_BOUNDS))
				continue;
		pRT->FillRectangle(obj.toRectF(), getColor(p->getColor()));
		//pRT->DrawRectangle(obj.toRectF(), BLACK_b, 6);
	}

	if(DEBUGVIEW){
	for (int y = 0; y < 100; y++)
	{
		for (int x = 0; x < 100; x++)
		{
			X::Point coord(x, y);
			coord.multiplySelf(GRIDSIZE);
			coord.addDotSelf(location.multiply(-1));

			if (CollisionUtil::staticCollision(X::Rect(coord, X::Point(coord.getX() + GRIDSIZE, coord.getY() + GRIDSIZE)), SCREEN_BOUNDS))
			{
				pRT->DrawLine(D2D1::Point2F(coord.getIX(), coord.getIY()), D2D1::Point2F(coord.getIX(), coord.getIY() + GRIDSIZE), getColor("green"), 1);
				pRT->DrawLine(D2D1::Point2F(coord.getIX(), coord.getIY()), D2D1::Point2F(coord.getIX() + GRIDSIZE, coord.getIY()), getColor("green"), 1);
			}
		}
	}
	}

	if (firstpoint_b == false)
	{
		Point p1 = X::Point(round((Peripherals::mousePos().getX() + location.getX()) / GRIDSIZE), round((Peripherals::mousePos().getY() + location.getY()) / GRIDSIZE));
		Point p2 = schemToLocalPoint(p1);
		if (editmode == 2)
		{
			X::Rect loc = X::Rect(schemToLocalPoint(firstpoint), p2);

			pRT->FillRectangle(loc.toRectF(), getColor(currentcolor));

			wstring str = L"color=" + converter.from_bytes(currentcolor);
			pRT->DrawTextA(str.c_str(), wcslen(str.c_str()), debug_txt, D2D1::RectF(20, 20, 400, 40), getColor("black"), D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
		}
	}
	


	pRT->FillRectangle(PLAYER_SCREEN_LOC.toRectF(), getColor("red"));

	for (GameObject *collider : colliders)
	{
		Rect r = collider->getRect();
		X::Rect obj = schemToLocal(collider->getRect());
		if (!CollisionUtil::staticCollision(obj, SCREEN_BOUNDS))
			continue;

		pRT->DrawRectangle(obj.toRectF(), getColor("black"), 4);
	}

	if(SHOWPROPS){
	for (LevelProp *p : props)
	{
		Rect r = p->rect();
		if (getResource(p->res()) != NULL)
		{
			X::Rect obj = schemToLocalZ(r, p->getZ(), (int)SCREEN_BOUNDS.right(), (int)SCREEN_BOUNDS.bottom());
			if (!CollisionUtil::staticCollision(obj, SCREEN_BOUNDS))
				continue;
			pRT->DrawBitmap(getResource(p->res()), obj.toRectF(), FLOAT(1.0f));
		}
		//pRT->DrawRectangle(obj.toRectF(), BLACK_b, 6);
	}
	}

	//draw console
	if (console_up)
	{
		pRT->FillRectangle(SCREEN_BOUNDS.toRectF(), getColor("debug_background"));

		for (int i = 0; i < console_history.size() && i * (CONSOLE_TEXT_SIZE + CONSOLE_TEXT_SPACING) + 25 < SCREEN_BOUNDS.bottom() - 50; i++)
		{
			ConsoleLine line = console_history[i];
			ID2D1SolidColorBrush *color = line.getOwner() == 0 ? getColor("orange") : getColor("white");

			pRT->DrawTextA(line.getText().c_str(), wcslen(line.getText().c_str()), console_txt, D2D1::RectF(20, i * (CONSOLE_TEXT_SIZE + CONSOLE_TEXT_SPACING) + 20, SCREEN_BOUNDS.right() - 40, i * (CONSOLE_TEXT_SIZE + CONSOLE_TEXT_SPACING) + CONSOLE_TEXT_SIZE + 20), getColor("black"), D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
		}

		pRT->DrawTextA(current_command.c_str(), wcslen(current_command.c_str()), console_txt, D2D1::RectF(20, SCREEN_BOUNDS.bottom() - 20 - CONSOLE_TEXT_SIZE, SCREEN_BOUNDS.right() - 40, SCREEN_BOUNDS.bottom() - 20), getColor("black"), D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
	}

	//draw pointer and level designer stuff
	if(DEBUGVIEW){
	if (editmode == 0)
		pRT->DrawEllipse(D2D1::Ellipse(Peripherals::mousePos().P2F(), 6, 6), getColor("red"), 2);
	else if (editmode == 1)
		pRT->DrawEllipse(D2D1::Ellipse(Peripherals::mousePos().P2F(), 6, 6), getColor("black"), 2);
	else if (editmode == 2)
		pRT->DrawEllipse(D2D1::Ellipse(Peripherals::mousePos().P2F(), 6, 6), getColor("orange"), 2);
	}

	if (firstpoint_b == false)
	{
		Point p1 = X::Point(round((Peripherals::mousePos().getX() + location.getX()) / GRIDSIZE), round((Peripherals::mousePos().getY() + location.getY()) / GRIDSIZE));
		Point p2 = schemToLocalPoint(p1);
		if (editmode == 0)
		{
			pRT->DrawLine(schemToLocalPoint(firstpoint).P2F(), p2.P2F(), getColor("red"), 4);
		}
		else if (editmode == 1)
		{
			X::Rect loc = X::Rect(schemToLocalPoint(firstpoint), p2);
			pRT->DrawBitmap(getResource(currentasset), loc.toRectF(), FLOAT(0.6f));
			pRT->DrawRectangle(loc.toRectF(), getColor("red"), 4);

			X::Rect r1 = X::Rect(firstpoint, p1);
			wstring str = L"z=" + std::to_wstring(select_z) + L"  size=(" + to_wstring((int)(r1.right() - r1.left())) + L"," + to_wstring((int)(r1.bottom() - r1.top())) + L")";
			pRT->DrawTextA(str.c_str(), wcslen(str.c_str()), debug_txt, D2D1::RectF(20, 20, 400, 40), getColor("black"), D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
		}
	}
	if(DEBUGVIEW){
	pRT->DrawRectangle(D2D1::RectF(SCREEN_BOUNDS.right() * 0.2, SCREEN_BOUNDS.bottom() * 0.2, SCREEN_BOUNDS.right() * 0.8, SCREEN_BOUNDS.bottom() * 0.8), getColor("red"), 6);
	}
}

void Application::tick(long tick)
{

	// bool pressed = Peripherals::keyPressed(0x41);
	// if(pressed)
	// x1 += 4;
	InputProcessing();

	if (console_up)
		return;

	double movex = 0;
	double movey = 0;

	int intent_x = 0;
	int intent_y = 0;
	if (Peripherals::keyPressed(0x41)) //a
		intent_x--;
	if (Peripherals::keyPressed(0x44)) //d
		intent_x++;
	if (noclip)
	{
		if (Peripherals::keyPressed(0x57)) //w
			intent_y++;
		if (Peripherals::keyPressed(0x53)) //s
			intent_y--;
	}

	if (intent_x != 0 || intent_y != 0)
	{
		double angle = atan2(intent_y, intent_x);
		double x_ = cos(angle) * PLAYER_SPEED;
		movex += x_;

		if (noclip)
		{
			double y_ = sin(angle) * PLAYER_SPEED;
			movey += y_;
		}
	}

	if (!noclip)
	{
		vertical_velocity -= GRAV_CONST;
		movey += vertical_velocity;

		// double dist = Double.MAX_VALUE;

		bool grounded = false;
		bool colliding = false;

		for (GameObject *r : colliders)
		{
			X::Rect obj = schemToLocal(r->getRect());
			if (!CollisionUtil::staticCollision(obj, SCREEN_BOUNDS))
				continue;
			CollisionReturn ret = CollisionUtil::DynamicCollision(PLAYER_SCREEN_LOC, obj, movex, movey);
			if (ret.y_collision)
			{
				colliding = true;
				if (ret.disp_y <= 0)
				{
					grounded = true;
				}
			}

			// CollisionReturn ret2 = dynamicCollision(p2, obj, movex, 0);
			if (ret.x_collision)
			{
				movex = 0;
			}
		}

		if (colliding)
		{
			// camy += dist;
			vertical_velocity = 0;
			if (grounded)
			{
				if (Peripherals::keyPressed(0x57))
					vertical_velocity = JUMP_INTENSITY;
			}
		}
		else
		{
			location.addDotSelf(X::Point(0, -movey));
		}
	}
	else
	{
		location.addDotSelf(X::Point(0, -movey));
	}

	location.addDotSelf(X::Point(movex, 0));
}

void Application::InputProcessing()
{
	//CONSOLE PROCESSING
	if (console_toggle && Peripherals::keyPressed(0xC0))
	{ //~
		console_toggle = false;
		console_up = !console_up;
		Peripherals::queueState(console_up);

		if (!console_up)
			current_command.clear();
	}
	else if (!Peripherals::keyPressed(0xC0))
		console_toggle = true;

	if (backspace_toggle && Peripherals::keyPressed(VK_BACK) && current_command.length() > 0)
	{
		backspace_toggle = false;
		current_command.pop_back();
	}

	if (!Peripherals::keyPressed(VK_BACK))
	{
		backspace_toggle = true;
	}

	if (Peripherals::keyPressed(VK_RETURN) && current_command.length() > 0)
	{
		wstring ret = runConsoleCommand(current_command);
		console_history.push_back(ConsoleLine(1, current_command));
		if (ret.length() > 0)
			console_history.push_back(ConsoleLine(0, ret));
		current_command = L"";
		Peripherals::unloadQueue();
	}
	else if (Peripherals::queueUpdate())
	{
		current_command += Peripherals::unloadQueue();
	}

	//CLICK PROCESSING
	if (Peripherals::mouseClickedLeft())
	{
		X::Point m = Peripherals::mouseClickLeftPos();
		if (firstpoint_b)
		{
			firstpoint = X::Point(round((m.getX() + location.getX()) / GRIDSIZE), round((m.getY() + location.getY()) / GRIDSIZE));
			firstpoint_b = false;
		}
		else
		{
			secondpoint = X::Point(round((m.getX() + location.getX()) / GRIDSIZE), round((m.getY() + location.getY()) / GRIDSIZE));

			X::Point topleft = X::Point(min(firstpoint.getX(), secondpoint.getX()), min(firstpoint.getY(), secondpoint.getY()));
			X::Point bottomright = X::Point(max(firstpoint.getX(), secondpoint.getX()), max(firstpoint.getY(), secondpoint.getY()));

			if (editmode == 0)
			{
				GameObject *r = new GameObject(topleft, bottomright, true);
				newcolliders.push_back(r);
				colliders.push_back(r);
			}
			else if (editmode == 1)
			{
				//X::Rect((r.left()*GRIDSIZE-location.getX()*z), r.top()*GRIDSIZE-location.getY()*z,(r.left()*GRIDSIZE-location.getX()*z)+((r.right()-r.left())*GRIDSIZE),r.top()*GRIDSIZE-location.getY()*z+((r.bottom()-r.top())*GRIDSIZE));
				LevelProp *p = new LevelProp(currentasset, X::Point(topleft.getX(), topleft.getY()), X::Point(bottomright.getX(), bottomright.getY()), select_z);
				newprops.push_back(p);
				props.push_back(p);
			}
			else if (editmode == 2)
			{
				//X::Rect((r.left()*GRIDSIZE-location.getX()*z), r.top()*GRIDSIZE-location.getY()*z,(r.left()*GRIDSIZE-location.getX()*z)+((r.right()-r.left())*GRIDSIZE),r.top()*GRIDSIZE-location.getY()*z+((r.bottom()-r.top())*GRIDSIZE));
				ColorRect *r = new ColorRect(currentcolor, X::Point(topleft.getX(), topleft.getY()), X::Point(bottomright.getX(), bottomright.getY()));
				newcolorrect.push_back(r);
				colorrect.push_back(r);
			}
			firstpoint_b = true;
		}
	}
	if (Peripherals::mouseClickedRight())
	{
		if (editmode >= 2)
			editmode = 0;
		else
			editmode++;
	}

	//SELECT_Z
	if (select_z_toggle)
	{
		select_z_toggle = false;
		if (Peripherals::keyPressed(VK_ADD))
		{
			select_z += 0.25;
		}
		else if (Peripherals::keyPressed(VK_SUBTRACT))
		{
			select_z -= 0.25;
		}
	}

	if (!Peripherals::keyPressed(VK_ADD) && !Peripherals::keyPressed(VK_SUBTRACT))
	{
		select_z_toggle = true;
	}

	//reset placement
	if (Peripherals::keyPressed(0x58))
	{ //X
		firstpoint_b = true;
	}
}
#define BRUSH(y, x) pRT->CreateSolidColorBrush(D2D1::ColorF(x), y);
void Application::InitResources(IDWriteFactory *pDWriteFactory, IWICImagingFactory *pFactory)
{

	InitColor("black", D2D1::ColorF(D2D1::ColorF::Black));
	InitColor("white", D2D1::ColorF(D2D1::ColorF::White));
	InitColor("red", D2D1::ColorF(D2D1::ColorF::Red));
	InitColor("green", D2D1::ColorF(D2D1::ColorF::Green));
	InitColor("orange", D2D1::ColorF(D2D1::ColorF::Orange));
	InitColor("debug_background", D2D1::ColorF(90, 90, 90, 0.6));
	InitColor("cavegray", D2D1::ColorF(0x7f807f));
	InitColor("cavegraydark", D2D1::ColorF(0x2f302f));
	


	pDWriteFactory->CreateTextFormat(
		L"Arial", // Font family name.
		NULL,	  // Font collection (NULL sets it to use the system font collection).
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		CONSOLE_TEXT_SIZE,
		L"en-us",
		&console_txt);
	pDWriteFactory->CreateTextFormat(
		L"Arial", // Font family name.
		NULL,	  // Font collection (NULL sets it to use the system font collection).
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		12,
		L"en-us",
		&debug_txt);

	ID2D1Bitmap *bitmap;
	LoadBitmapFromFile(pRT, pFactory, PCWSTR(wstring(APP_PATH + L"\\res\\image1.PNG").c_str()), RESOURCE_SIZE, RESOURCE_SIZE, &bitmap);
	addResource("pat", bitmap);
	ID2D1Bitmap *bitmap1;
	LoadBitmapFromFile(pRT, pFactory, PCWSTR(wstring(APP_PATH + L"\\res\\cave_end1.png").c_str()), RESOURCE_SIZE, RESOURCE_SIZE, &bitmap1);
	addResource("caveend1", bitmap1);
	ID2D1Bitmap *bitmap2;
	LoadBitmapFromFile(pRT, pFactory, PCWSTR(wstring(APP_PATH + L"\\res\\fadingcave.png").c_str()), RESOURCE_SIZE, RESOURCE_SIZE, &bitmap2);
	addResource("fadingcave", bitmap2);
	ID2D1Bitmap *bitmap3;
	LoadBitmapFromFile(pRT, pFactory, PCWSTR(wstring(APP_PATH + L"\\res\\cave1.png").c_str()), RESOURCE_SIZE, RESOURCE_SIZE, &bitmap3);
	addResource("cave1", bitmap3);
	ID2D1Bitmap *bitmap4;
	LoadBitmapFromFile(pRT, pFactory, PCWSTR(wstring(APP_PATH + L"\\res\\rock1.png").c_str()), RESOURCE_SIZE, RESOURCE_SIZE, &bitmap4);
	addResource("rock1", bitmap4);
}
void Application::DeinitResources()
{
	for (GameObject *obj : colliders)
	{
		delete obj;
	}
	for (LevelProp *p : props)
	{
		delete p;
	}

	for (std::pair<const std::string, ID2D1Bitmap *> b : resources)
	{
		ID2D1Bitmap *a = b.second;
		SafeRelease(a);
	}

	for (std::pair<const std::string, ID2D1SolidColorBrush *> a : colors)
	{
		SafeRelease(a.second);
	}
	SafeRelease(console_txt);
	SafeRelease(debug_txt);
}

Application::Application(HWND hWnd, ID2D1HwndRenderTarget *pRT)
{
	this->hWnd = hWnd;
	this->pRT = pRT;

	readconfig(APP_PATH + L"\\level.txt", colliders, props, colorrect);
}

void Application::onResize(int width, int height)
{
	PLAYER_SCREEN_LOC = X::Rect(X::Point((width - PLAYER_SIZE) / 2, height * 0.750 - PLAYER_SIZE / 2), X::Point((width + PLAYER_SIZE) / 2, height * 0.750 + PLAYER_SIZE / 2));
	SCREEN_BOUNDS = X::Rect(X::Point(0, 0), X::Point(width, height));
}


X::Rect schemToLocal(X::Rect r)
{
	///multiply by GRIDSIZE, subtract camera location
	return X::Rect(r.left() * GRIDSIZE - location.getX(), r.top() * GRIDSIZE - location.getY(), r.right() * GRIDSIZE - location.getX(), r.bottom() * GRIDSIZE - location.getY());
}

X::Rect schemToLocalZ(X::Rect r, double z, int width, int height)
{
	X::Point p = X::Point(
		r.left() * GRIDSIZE * z - (location.getX() + PLAYER_SCREEN_LOC.left()) * z + PLAYER_SCREEN_LOC.left() + (z - 1) * PLAYER_SIZE / 2,
		r.top() * GRIDSIZE - location.getY());
	return X::Rect(p.getX(),
				   p.getY(),
				   p.getX() + (r.right() - r.left()) * GRIDSIZE,
				   p.getY() + (r.bottom() - r.top()) * GRIDSIZE);
}

Application::Application()
{
}

X::Point schemToLocalPoint(X::Point r)
{
	///multiply by GRIDSIZE, subtract camera location
	return X::Point(r.getX() * GRIDSIZE - location.getX(), r.getY() * GRIDSIZE - location.getY());
}

ConsoleLine::ConsoleLine(int owner, wstring text)
{
	this->text = text;
	this->owner = owner;
}
wstring ConsoleLine::getText()
{
	return text;
}
int ConsoleLine::getOwner()
{
	return owner;
}