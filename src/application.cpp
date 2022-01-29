#include "../headers/application.h"
#include "../headers/imports.h"
#include "../headers/Peripherals.h"

#include "../headers/Collider.h"
#include "../headers/CollisionUtil.h"

#include "../headers/LevelProp.h"
#include "../headers/ImageUtil.h"
#include "../headers/ColorRect.h"
#include "../headers/util.h"

using convert_type = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_type, wchar_t> converter;

IDWriteTextFormat *console_txt;
IDWriteTextFormat *debug_txt;


X::Point location;
double x1 = 0;
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
bool showprops = true;


std::string get_stem(const fs::path& p) { return (p.stem().string()); }

inline bool strcompare(const wchar_t* a, const wchar_t* b)
{
	return _wcsnicmp(a, b, max(wcslen(a), wcslen(b))) == 0;
}

void Application::InitColor(string name, D2D1_COLOR_F color)
{
	ID2D1SolidColorBrush *br = NULL;
	const D2D1_COLOR_F c(color);
	pRT->CreateSolidColorBrush(c, &br);
	colors.insert_or_assign(name, br);
}

void Application::InitBitmap(string name, wstring path, int size)
{
	ID2D1Bitmap *bitmap;
	LoadBitmapFromFile(pRT, pWICFactory, PCWSTR(wstring(INFO::APP_PATH + path).c_str()), size, size, &bitmap);
	resources.insert_or_assign(name, bitmap);
}


void InputProcessing();



void Application::Paint()
{
	pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

	for (ColorRect *p : colorrect)
	{
		ID2D1SolidColorBrush *br2 = colors[p->getColor()];
		if (br2 == nullptr)
			continue;
		X::Rect obj = schemToLocal(*p);

		if (!CollisionUtil::staticCollision(obj, SCREEN_BOUNDS))
			continue;
		pRT->FillRectangle(obj.expanded(1).toRectF(), br2);
		//pRT->DrawRectangle(obj.toRectF(), BLACK_b, 6);
	}
	const int z = 2;
	if (debugview)
	{
		for (int y = LEVEL_BOUNDS.top() - z; y < LEVEL_BOUNDS.bottom() + z; y++)
		{
			for (int x = LEVEL_BOUNDS.left() - z; x < LEVEL_BOUNDS.right() + z; x++)
			{
				X::Point coord(x, y);
				coord.multiplySelf(gridsize);
				coord.addDotSelf(location.multiply(-1));

				if (CollisionUtil::staticCollision(X::Rect(coord, X::Point(coord.getX() + gridsize, coord.getY() + gridsize)), SCREEN_BOUNDS))
				{
					pRT->DrawLine(D2D1::Point2F(coord.getIX(), coord.getIY()), D2D1::Point2F(coord.getIX(), coord.getIY() + gridsize), colors["green"], 1);
					pRT->DrawLine(D2D1::Point2F(coord.getIX(), coord.getIY()), D2D1::Point2F(coord.getIX() + gridsize, coord.getIY()), colors["green"], 1);
				}
			}
		}
	}

	if (firstpoint_b == false)
	{
		Point p1 = X::Point(round((Peripherals::mousePos().getX() + location.getX()) / gridsize), round((Peripherals::mousePos().getY() + location.getY()) / gridsize));
		Point p2 = schemToLocalPoint(p1);
		if (editmode == 2)
		{
			X::Rect loc = X::Rect(schemToLocalPoint(firstpoint), p2);

			pRT->FillRectangle(loc.toRectF(), colors[currentcolor]);

			wstring str = L"color=" + converter.from_bytes(currentcolor);
			pRT->DrawText(str.c_str(), wcslen(str.c_str()), debug_txt, D2D1::RectF(20, 20, 400, 40), colors["black"], D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
		}
	}

	for (Collider *collider : colliders)
	{
		if (!collider->visible() && !debugview)
			continue;
		X::Rect obj = schemToLocal(*collider);
		if (!CollisionUtil::staticCollision(obj, SCREEN_BOUNDS))
			continue;

		pRT->DrawRectangle(obj.toRectF(), colors["black"], 4);
	}

	for (LevelProp *p : props)
	{
		if (resources[p->res()] != NULL && p->getZ() <= 1.000)
		{
			X::Rect obj = schemToLocalZ(*p, p->getZ(), (int)SCREEN_BOUNDS.right(), (int)SCREEN_BOUNDS.bottom());
			if (!CollisionUtil::staticCollision(obj, SCREEN_BOUNDS))
				continue;
			pRT->DrawBitmap(resources[p->res()], obj.expanded(1).toRectF(), FLOAT(1.0f));
		}
		//pRT->DrawRectangle(obj.toRectF(), BLACK_b, 6);
	}

	pRT->FillRectangle(PLAYER_SCREEN_LOC.toRectF(), colors["red"]);

	if (showprops)
	{
		for (LevelProp *p : props)
		{
			if (resources[p->res()] != NULL && p->getZ() > 1.000)
			{
				X::Rect obj = schemToLocalZ(*p, p->getZ()== 1.001?1.000:p->getZ(), (int)SCREEN_BOUNDS.right(), (int)SCREEN_BOUNDS.bottom());
				if (!CollisionUtil::staticCollision(obj, SCREEN_BOUNDS))
					continue;
				pRT->DrawBitmap(resources[p->res()], obj.toRectF(), FLOAT(1.0f));
			}
			//pRT->DrawRectangle(obj.toRectF(), BLACK_b, 6);
		}
	}

	//draw console
	if (console_up)
	{
		pRT->FillRectangle(SCREEN_BOUNDS.toRectF(), colors["debug_background"]);

		for (int i = 0; i < console_history.size() && i * (CONSOLE_TEXT_SIZE + CONSOLE_TEXT_SPACING) + 25 < SCREEN_BOUNDS.bottom() - 50; i++)
		{
			ConsoleLine line = console_history[i];
			ID2D1SolidColorBrush *color = line.getOwner() == 0 ? colors["orange"] : colors["white"];

			pRT->DrawText(line.getText().c_str(), wcslen(line.getText().c_str()), console_txt, D2D1::RectF(20, i * (CONSOLE_TEXT_SIZE + CONSOLE_TEXT_SPACING) + 20, SCREEN_BOUNDS.right() - 40, i * (CONSOLE_TEXT_SIZE + CONSOLE_TEXT_SPACING) + CONSOLE_TEXT_SIZE + 20), colors["black"], D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
		}

		pRT->DrawText(current_command.c_str(), wcslen(current_command.c_str()), console_txt, D2D1::RectF(20, SCREEN_BOUNDS.bottom() - 20 - CONSOLE_TEXT_SIZE, SCREEN_BOUNDS.right() - 40, SCREEN_BOUNDS.bottom() - 20), colors["black"], D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
	}

	//draw pointer and level designer stuff
	if (debugview)
	{
		if (editmode == 0)
			pRT->DrawEllipse(D2D1::Ellipse(Peripherals::mousePos().P2F(), 6, 6), colors["red"], 2);
		else if (editmode == 1)
			pRT->DrawEllipse(D2D1::Ellipse(Peripherals::mousePos().P2F(), 6, 6), colors["black"], 2);
		else if (editmode == 2)
			pRT->DrawEllipse(D2D1::Ellipse(Peripherals::mousePos().P2F(), 6, 6), colors["orange"], 2);
	}

	Point p1 = X::Point(round((Peripherals::mousePos().getX() + location.getX()) / gridsize), round((Peripherals::mousePos().getY() + location.getY()) / gridsize));
	if (firstpoint_b == false)
	{

		Point p2 = schemToLocalPoint(p1);
		if (editmode == 0)
		{
			pRT->DrawLine(schemToLocalPoint(firstpoint).P2F(), p2.P2F(), colors["red"], 4);
		}
		else if (editmode == 1)
		{
			X::Rect loc = X::Rect(schemToLocalPoint(firstpoint), p2);
			ID2D1Bitmap *asset = resources[currentasset];
			if (asset != nullptr)
			{
				pRT->DrawBitmap(asset, loc.toRectF(), FLOAT(0.6f));
			}
			pRT->DrawRectangle(loc.toRectF(), colors["red"], 4);

			X::Rect r1 = X::Rect(firstpoint, p1);
			wstring str = L"z=" + std::to_wstring(select_z) + L"  size=(" + to_wstring((int)(r1.right() - r1.left())) + L"," + to_wstring((int)(r1.bottom() - r1.top())) + L")";
			pRT->DrawText(str.c_str(), wcslen(str.c_str()), debug_txt, D2D1::RectF(20, 20, 400, 40), colors["black"], D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
		}
	}

	wstring str = L"coord=(" + to_wstring((int)p1.getX()) + L"," + to_wstring((int)p1.getY()) + L")";
	pRT->DrawText(str.c_str(), wcslen(str.c_str()), debug_txt, D2D1::RectF(SCREEN_BOUNDS.right() - 100, 20, SCREEN_BOUNDS.right(), 50), colors["black"], D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
	if (debugview)
	{
		//pRT->DrawRectangle(D2D1::RectF(SCREEN_BOUNDS.right() * 0.2, SCREEN_BOUNDS.bottom() * 0.2, SCREEN_BOUNDS.right() * 0.8, SCREEN_BOUNDS.bottom() * 0.8), getColor("red"), 6);
	}
}

int t = 0;
void Application::tick(long tick)
{

	LEVEL_BOUNDS = X::Rect(9999, 9999, -9999, -9999);
	for (Collider *o : colliders)
	{
		if ((*o).left() < LEVEL_BOUNDS.left())
			LEVEL_BOUNDS.setLeft((*o).left());
		if ((*o).right() > LEVEL_BOUNDS.right())
			LEVEL_BOUNDS.setRight((*o).right());
		if ((*o).top() < LEVEL_BOUNDS.top())
			LEVEL_BOUNDS.setTop((*o).top());
		if ((*o).bottom() > LEVEL_BOUNDS.bottom())
			LEVEL_BOUNDS.setBottom((*o).bottom());
	}
	for (LevelProp *o : props)
	{
		if ((*o).left() < LEVEL_BOUNDS.left())
			LEVEL_BOUNDS.setLeft((*o).left());
		if ((*o).right() > LEVEL_BOUNDS.right())
			LEVEL_BOUNDS.setRight((*o).right());
		if ((*o).top() < LEVEL_BOUNDS.top())
			LEVEL_BOUNDS.setTop((*o).top());
		if ((*o).bottom() > LEVEL_BOUNDS.bottom())
			LEVEL_BOUNDS.setBottom((*o).bottom());
	}
	for (ColorRect *o : colorrect)
	{
		if ((*o).left() < LEVEL_BOUNDS.left())
			LEVEL_BOUNDS.setLeft((*o).left());
		if ((*o).right() > LEVEL_BOUNDS.right())
			LEVEL_BOUNDS.setRight((*o).right());
		if ((*o).top() < LEVEL_BOUNDS.top())
			LEVEL_BOUNDS.setTop((*o).top());
		if ((*o).bottom() > LEVEL_BOUNDS.bottom())
			LEVEL_BOUNDS.setBottom((*o).bottom());
	}

	if (!firstpoint_b)
	{
		Point p1 = X::Point(round((Peripherals::mousePos().getX() + location.getX()) / gridsize), round((Peripherals::mousePos().getY() + location.getY()) / gridsize));
		if (p1.getX() < LEVEL_BOUNDS.left())
			LEVEL_BOUNDS.setLeft(p1.getX());
		if (p1.getX() > LEVEL_BOUNDS.right())
			LEVEL_BOUNDS.setRight(p1.getX());
		if (p1.getY() < LEVEL_BOUNDS.top())
			LEVEL_BOUNDS.setTop(p1.getY());
		if (p1.getY() > LEVEL_BOUNDS.bottom())
			LEVEL_BOUNDS.setBottom(p1.getY());
	}
	// bool pressed = Peripherals::keyPressed(0x41);
	// if(pressed)
	// x1 += 4;
	InputProcessing();

	if (console_up)
		return;

	double movex = 0;
	double movey = 0;
	const double maxspeed = debugview ? PLAYER_SPEED_DEBUG : PLAYER_SPEED;

	{
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
		double angle = atan2(intent_y, intent_x);

		if (intent_x != 0 || intent_y != 0)
		{
			double x_ = cos(angle) * maxspeed;
			movex += x_;

			if (noclip)
			{
				double y_ = sin(angle) * maxspeed;
				movey += y_;
			}
		}
	}

	X::PolarPoint polp = Peripherals::controllerOutput(0);
	if(polp!=NULL){
		//std::cout<<polp.getAngle()<<std::endl;
		double x_ = cos(polp.getAngle()) * maxspeed*polp.getMagnitude();
		movex += x_;

		if (noclip)
		{
			double y_ = sin(polp.getAngle()) * maxspeed*polp.getMagnitude();
			movey += y_;
		}
		
	}else{
		// if(t>=40){
		// printf(".");
		// t=0;
		// }
		// else
		// t++;
	}

	if (!noclip)
	{
		vertical_velocity -= GRAV_CONST;
		movey += vertical_velocity;

		// double dist = Double.MAX_VALUE;

		bool grounded = false;
		bool colliding = false;

		for (Collider *r : colliders)
		{
			X::Rect obj = schemToLocal(*r);
			if (!CollisionUtil::staticCollision(obj, SCREEN_BOUNDS))
				continue;
			CollisionReturn ret = CollisionUtil::DynamicCollision(PLAYER_SCREEN_LOC, obj, movex, movey);
			if (ret.y_collision)
			{
				colliding = true;
				if (ret.disp_y <= 0 && ret.intent_y < 0)
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
	if (debugview)
	{
		if (Peripherals::mouseClickedLeft())
		{
			X::Point m = Peripherals::mouseClickLeftPos();
			if (firstpoint_b)
			{
				firstpoint = X::Point(round((m.getX() + location.getX()) / gridsize), round((m.getY() + location.getY()) / gridsize));
				firstpoint_b = false;
			}
			else
			{
				secondpoint = X::Point(round((m.getX() + location.getX()) / gridsize), round((m.getY() + location.getY()) / gridsize));

				X::Point topleft = X::Point(min(firstpoint.getX(), secondpoint.getX()), min(firstpoint.getY(), secondpoint.getY()));
				X::Point bottomright = X::Point(max(firstpoint.getX(), secondpoint.getX()), max(firstpoint.getY(), secondpoint.getY()));

				if (editmode == 0)
				{
					Collider *r = new Collider(topleft, bottomright, true);
					newcolliders.push_back(r);
					colliders.push_back(r);
				}
				else if (editmode == 1)
				{
					//X::Rect((r.left()*gridsize-location.getX()*z), r.top()*gridsize-location.getY()*z,(r.left()*gridsize-location.getX()*z)+((r.right()-r.left())*gridsize),r.top()*gridsize-location.getY()*z+((r.bottom()-r.top())*gridsize));
					LevelProp *p = new LevelProp(currentasset, X::Point(topleft.getX(), topleft.getY()), X::Point(bottomright.getX(), bottomright.getY()), select_z);
					newprops.push_back(p);
					props.push_back(p);
				}
				else if (editmode == 2)
				{
					//X::Rect((r.left()*gridsize-location.getX()*z), r.top()*gridsize-location.getY()*z,(r.left()*gridsize-location.getX()*z)+((r.right()-r.left())*gridsize),r.top()*gridsize-location.getY()*z+((r.bottom()-r.top())*gridsize));
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
}

#define BRUSH(y, x) pRT->CreateSolidColorBrush(D2D1::ColorF(x), y);
void Application::InitResources(IDWriteFactory *pDWriteFactory)
{

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

	wstring p = INFO::INFO::APP_PATH + wstring(L"\\res\\");
	std::cout << "RES PATH=";
	std::wcout << p;
	std::cout << std::endl;
	if (fs::is_directory(p))
	{

		for (auto &entry : boost::make_iterator_range(fs::directory_iterator(p), {}))
		{
			string fpath1 = get_stem(entry.path());

			wstring s;
			s += L"\\res\\";
			s += converter.from_bytes(fpath1);
			s += L".png";
			InitBitmap(fpath1, s, RESOURCE_SIZE);
			std::cout << fpath1 << std::endl;
		}
	}

	InitColor("black", D2D1::ColorF(D2D1::ColorF::Black));
	InitColor("white", D2D1::ColorF(D2D1::ColorF::White));
	InitColor("red", D2D1::ColorF(D2D1::ColorF::Red));
	InitColor("green", D2D1::ColorF(D2D1::ColorF::Green));
	InitColor("orange", D2D1::ColorF(D2D1::ColorF::Orange));
	InitColor("debug_background", D2D1::ColorF(90, 90, 90, 0.6));
}
void Application::DeinitResources()
{
	for (Collider *obj : colliders)
	{
		delete obj;
	}
	for (LevelProp *p : props)
	{
		delete p;
	}
	for (ColorRect *p : colorrect)
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

Application::Application(HWND hWnd, ID2D1HwndRenderTarget *pRT, IWICImagingFactory *pWICFactory)
{
	this->hWnd = hWnd;
	this->pRT = pRT;
	this->pWICFactory = pWICFactory;

	init();
}

void Application::init()
{
	colliders.clear();
	props.clear();
	colorrect.clear();
	newcolliders.clear();
	newprops.clear();
	newcolorrect.clear();
	readconfig(INFO::APP_PATH + L"\\level.txt");
}

void Application::onResize(int width, int height)
{
	PLAYER_SCREEN_LOC = X::Rect(X::Point((width - player_size) / 2, height * 0.60 - player_size / 2), X::Point((width + player_size) / 2, height * 0.60 + player_size / 2));
	SCREEN_BOUNDS = X::Rect(X::Point(0, 0), X::Point(width, height));
}

X::Rect Application::schemToLocal(X::Rect r)
{
	///multiply by gridsize, subtract camera location
	return X::Rect(r.left() * gridsize - location.getX(), r.top() * gridsize - location.getY(), r.right() * gridsize - location.getX(), r.bottom() * gridsize - location.getY());
}

X::Rect Application::schemToLocalZ(X::Rect r, double z, int width, int height)
{
	X::Point p = X::Point(
		r.left() * gridsize * z - (location.getX() + PLAYER_SCREEN_LOC.left()) * z + PLAYER_SCREEN_LOC.left() + (z - 1) * player_size / 2,
		r.top() * gridsize - location.getY());
	return X::Rect(p.getX(),
				   p.getY(),
				   p.getX() + (r.right() - r.left()) * gridsize,
				   p.getY() + (r.bottom() - r.top()) * gridsize);
}

X::Point Application::schemToLocalPoint(X::Point r)
{
	X::Rect rect = X::Rect(r.getX() * gridsize - location.getX(), r.getY() * gridsize - location.getY(), 0, 0);
	return X::Point(rect.left(), rect.top());
}

Application::Application()
{
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