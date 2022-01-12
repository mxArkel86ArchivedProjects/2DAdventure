#include "Peripherals.h"

std::unordered_map<WPARAM, bool> Peripherals::key_reg = std::unordered_map<WPARAM, bool>();
std::wstringstream Peripherals::characterqueue = std::wstringstream();

bool Peripherals::queuestate = false;
bool Peripherals::queueupdate = false;
int Peripherals::mousex = 0;
int Peripherals::mousey = 0;
int Peripherals::mouseclick_xl = 0;
int Peripherals::mouseclick_yl = 0;
bool Peripherals::mouseclicked_l = false;

int Peripherals::mouseclick_xr = 0;
int Peripherals::mouseclick_yr = 0;
bool Peripherals::mouseclicked_r = false;

void Peripherals::KeyPressIn(WPARAM p, bool type) {
	if(queuestate&&type&&p!=0xC0 &&p!=VK_BACK){
		if(p>='A' && p<='Z')
			characterqueue.put(p-'A'+'a');
		else
		characterqueue.put(p);
		queueupdate = true;
	}
	key_reg[p] = type;
}

bool Peripherals::keyPressed(WPARAM p) {
	return key_reg[p]==NULL?0:key_reg[p];
}

const wchar_t * empty_wstr = L"";
std::wstring Peripherals::unloadQueue(){
	std::wstring str = characterqueue.str();
	characterqueue.str(empty_wstr);
	queueupdate = false;
	return str;
}
void Peripherals::queueState(bool b){
	queuestate = b;
}

bool Peripherals::queueUpdate(){
	return queueupdate;
}

void Peripherals::MouseMoveIn(int x, int y){
	Peripherals::mousex = x;
	Peripherals::mousey = y;
}

void Peripherals::mouseClickLeftIn(int x, int y){
	mouseclick_xl = x;
	mouseclick_yl = y;
	mouseclicked_l = true;
}

bool Peripherals::mouseClickedLeft(){
	if(mouseclicked_l){
		mouseclicked_l = false;
		return true;
	}
	return false;
}

void Peripherals::mouseClickRightIn(int x, int y){
	mouseclick_xr = x;
	mouseclick_yr = y;
	mouseclicked_r = true;
}

bool Peripherals::mouseClickedRight(){
	if(mouseclicked_r){
		mouseclicked_r = false;
		return true;
	}
	return false;
}


X::Point Peripherals::mousePos(){
	return X::Point(Peripherals::mousex, Peripherals::mousey);
}

X::Point Peripherals::mouseClickLeftPos(){
	return X::Point(mouseclick_xl, mouseclick_yl);
}

X::Point Peripherals::mouseClickRightPos(){
	return X::Point(mouseclick_xr, mouseclick_yr);
}