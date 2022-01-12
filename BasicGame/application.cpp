#include "application.h"
#include "X.h"
#include "util.h"
#include "Peripherals.h"
#include <vector>
#include "GameObject.h"
#include "CollisionUtil.h"
#include <string>

using std::vector;
using std::string;

ID2D1SolidColorBrush* BLACK_b = NULL;
ID2D1SolidColorBrush* WHITE_b = NULL;
ID2D1SolidColorBrush* RED_b = NULL;
ID2D1SolidColorBrush* GREEN_b = NULL;
ID2D1SolidColorBrush* ORANGE_b = NULL;
ID2D1SolidColorBrush* debug_background = NULL;
IDWriteTextFormat* console_txt;

#define PLAYER_SIZE 38
#define GRIDSIZE 40
#define PLAYER_SPEED 2
#define GRAV_CONST 0.1
#define JUMP_INTENSITY 4
#define CONSOLE_TEXT_SIZE 16
#define CONSOLE_TEXT_SPACING 4

X::Point location;
double x1 = 0;
bool noclip = true;
double vertical_velocity = 0;
bool console_up = false;
bool console_toggle = true;
bool backspace_toggle = true;
wstring current_command = L"some text1";

X::Rect schemToLocal(X::Rect r);
class ConsoleLine {
	wstring text;
	int owner;
	public:
	ConsoleLine(int owner, wstring text);
	wstring getText();
	int getOwner();
};

vector<ConsoleLine> console_history = vector<ConsoleLine>();
vector<GameObject*> colliders = vector<GameObject*>();

X::Rect PLAYER_SCREEN_LOC;
X::Rect SCREEN_BOUNDS;

inline bool strcompare(const wchar_t * a, const wchar_t* b){
	return _wcsnicmp(a, b, max(wcslen(a), wcslen(b)))==0;
}

wstring runConsoleCommand(wstring cmd){
	if(strcompare(cmd.c_str(), L"clip")){
		noclip = !noclip;
		wstring resp = noclip?L"Successfully enabled noclip":L"Successfully disabled noclip";
		return resp;
	}else if(strcompare(cmd.c_str(), L"clear")){
		console_history.clear();
		return L"Successfully cleared console";
	}else
	return L"";
}

void Application::Paint(ID2D1HwndRenderTarget* pRT) {
	pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

	
	pRT->FillRectangle(PLAYER_SCREEN_LOC.toRectF(), RED_b);

	for (int y = 0; y < 20; y++) {
		for (int x = 0; x < 20; x++) {
			X::Point coord(x, y);
			coord.multiplySelf(GRIDSIZE);
			coord.addDotSelf(location.multiply(-1));

			pRT->DrawLine(D2D1::Point2F(coord.getIX(), coord.getIY()), D2D1::Point2F(coord.getIX(), coord.getIY() + GRIDSIZE), GREEN_b, 1);
			pRT->DrawLine(D2D1::Point2F(coord.getIX(), coord.getIY()), D2D1::Point2F(coord.getIX()+GRIDSIZE, coord.getIY()), GREEN_b, 1);
			
		}
	}


	for(GameObject* collider : colliders){
		Rect r = collider->getRect();
		
		X::Rect obj = schemToLocal(collider->getRect());
		pRT->DrawRectangle(obj.toRectF(), RED_b, 4);
	}

	if(console_up){
	pRT->FillRectangle(SCREEN_BOUNDS.toRectF(), debug_background);
	
	for (int i = 0; i < console_history.size() && i*(CONSOLE_TEXT_SIZE+CONSOLE_TEXT_SPACING)+25<WINSIZE->bottom-50; i++) {
	 	ConsoleLine line = console_history[i];
		ID2D1SolidColorBrush* color = line.getOwner()==0?ORANGE_b:WHITE_b;
	 	
		pRT->DrawTextA(line.getText().c_str(), wcslen(line.getText().c_str()), console_txt, D2D1::RectF(20, i*(CONSOLE_TEXT_SIZE+CONSOLE_TEXT_SPACING)+20, WINSIZE->right-40,i*(CONSOLE_TEXT_SIZE+CONSOLE_TEXT_SPACING)+CONSOLE_TEXT_SIZE+20), BLACK_b, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
	 }
	
	pRT->DrawTextA(current_command.c_str(), wcslen(current_command.c_str()), console_txt, D2D1::RectF(20, WINSIZE->bottom-20-CONSOLE_TEXT_SIZE, WINSIZE->right-40,WINSIZE->bottom-20), BLACK_b, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
	}
}

void Application::tick(long tick) {

	// bool pressed = Peripherals::keyPressed(0x41);
	// if(pressed)
	// x1 += 4;
	if(console_toggle && Peripherals::keyPressed(0xC0)){//~
		console_toggle = false;
		console_up = !console_up;
		Peripherals::queueState(console_up);

		if(!console_up)
			current_command.clear();
	}else
	if(!Peripherals::keyPressed(0xC0))
		console_toggle = true;

	if(backspace_toggle&& Peripherals::keyPressed(VK_BACK) && current_command.length()>0){
		backspace_toggle = false;
		current_command.pop_back();
	}

	if(!Peripherals::keyPressed(VK_BACK)){
		backspace_toggle = true;
	}

	if(Peripherals::keyPressed(VK_RETURN) && current_command.length()>0){
		wstring ret = runConsoleCommand(current_command);
		console_history.push_back(ConsoleLine(1, current_command));
		if(ret.length()>0)
		console_history.push_back(ConsoleLine(0, ret));
		current_command = L"";
		Peripherals::unloadQueue();
	}else
	if(Peripherals::queueUpdate()){
		current_command+=Peripherals::unloadQueue();
	}

	if(console_up)
		return;

	double movex = 0;
	double movey =0;

	int intent_x = 0;
	int intent_y = 0;
	if (Peripherals::keyPressed(0x41))//a
		intent_x--;
	if (Peripherals::keyPressed(0x44))//d
		intent_x++;
	if (noclip) {
		if (Peripherals::keyPressed(0x57))//w
			intent_y++;
		if (Peripherals::keyPressed(0x53))//s
			intent_y--;
	}

	if (intent_x != 0 || intent_y != 0) {
		double angle = atan2(intent_y, intent_x);
		double x_ = cos(angle) * PLAYER_SPEED;
		movex += x_;

		if (noclip) {
			double y_ = sin(angle) * PLAYER_SPEED;
			movey += y_;
		}

	}


	if (!noclip) {
		vertical_velocity -= GRAV_CONST;
		movey += vertical_velocity;

		// double dist = Double.MAX_VALUE;

		bool grounded = false;
		bool colliding = false;

		for (GameObject* r : colliders) {
			X::Rect obj = schemToLocal(r->getRect());
			
			CollisionReturn ret = CollisionUtil::DynamicCollision(PLAYER_SCREEN_LOC, obj, movex, movey);
			if (ret.y_collision) {
				colliding = true;
				if (ret.disp_y<=0) {
					grounded = true;
				}
			}

			// CollisionReturn ret2 = dynamicCollision(p2, obj, movex, 0);
			if (ret.x_collision) {
				movex = 0;
			}
			
		}

		if (colliding) {
			// camy += dist;
			vertical_velocity = 0;
			if (grounded) {
				if (Peripherals::keyPressed(0x57))
					vertical_velocity = JUMP_INTENSITY;
			}

		}
		else {
			location.addDotSelf(X::Point(0,-movey));
		}
	}
	else {
		location.addDotSelf(X::Point(0,-movey));
	}

	location.addDotSelf(X::Point(movex, 0));
	
}

#define BRUSH(y, x)pRT->CreateSolidColorBrush(D2D1::ColorF(x),y);
void Application::InitResources(ID2D1HwndRenderTarget* pRT, IDWriteFactory* pDWriteFactory) {
	BRUSH(&BLACK_b, D2D1::ColorF::Black);
	BRUSH(&WHITE_b, D2D1::ColorF::White);
	BRUSH(&RED_b, D2D1::ColorF::Red);
	BRUSH(&GREEN_b, D2D1::ColorF::Green);
	BRUSH(&ORANGE_b, D2D1::ColorF(255,215,0));
	BRUSH(&debug_background, D2D1::ColorF(90,90,90,0.6));


	pDWriteFactory->CreateTextFormat(
        L"Arial",                // Font family name.
        NULL,                       // Font collection (NULL sets it to use the system font collection).
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        CONSOLE_TEXT_SIZE,
        L"en-us",
        &console_txt
        );
	//pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &GREEN_b);
}
void Application::DeinitResources() {
	SafeRelease(BLACK_b);
	SafeRelease(WHITE_b);
	SafeRelease(RED_b);
	SafeRelease(GREEN_b);
	SafeRelease(ORANGE_b);
	SafeRelease(debug_background);
	SafeRelease(console_txt);
}

Application::Application(HWND hWnd) {
	this->hWnd = hWnd;
	WINSIZE = (LPRECT)malloc(sizeof(LPRECT));
	onResize(WINSIZE->right, WINSIZE->bottom);

	GameObject* o = new GameObject(X::Point(1,1), X::Point(4,1), true);
	colliders.push_back(o);
}

void Application::onResize(int width, int height){
	PLAYER_SCREEN_LOC = X::Rect(X::Point((width-PLAYER_SIZE)/2, (height-PLAYER_SIZE)/2), X::Point((width+PLAYER_SIZE)/2, (height+PLAYER_SIZE)/2));
	SCREEN_BOUNDS = X::Rect(X::Point(0,0), X::Point(width, height));
}

X::Rect schemToLocal(X::Rect r){
	///multiply by GRIDSIZE, subtract camera location
	return X::Rect(r.left()*GRIDSIZE-location.getX(), r.top()*GRIDSIZE-location.getY(),r.right()*GRIDSIZE-location.getX(),r.bottom()*GRIDSIZE-location.getY());
}

ConsoleLine::ConsoleLine(int owner, wstring text) {
		this->text = text;
		this->owner = owner;
	}
wstring ConsoleLine::getText(){
		return text;
	}
int ConsoleLine::getOwner(){
		return owner;
}