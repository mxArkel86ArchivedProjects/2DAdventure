#include "Peripherals.h"

#define XINPUT_GAMEPAD_DEADZONE 7849
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD 30

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

void Peripherals::KeyPressIn(WPARAM p, bool type)
{
	if (queuestate && type && p != 0xC0 && p != VK_BACK)
	{
		if (p >= 'A' && p <= 'Z')
			characterqueue.put(p - 'A' + 'a');
		else
			characterqueue.put(p);
		queueupdate = true;
	}
	key_reg[p] = type;
}

bool Peripherals::keyPressed(WPARAM p)
{
	return key_reg[p] == NULL ? 0 : key_reg[p];
}

bool Peripherals::controllerConnected(int i, XINPUT_STATE &state)
{
	DWORD dwResult;

	ZeroMemory(&state, sizeof(XINPUT_STATE));

	// Simply get the state of the controller from XInput.
	dwResult = XInputGetState(i, &state);

	return (dwResult == ERROR_SUCCESS);
}

X::PolarPoint Peripherals::controllerOutput(int i)
{
	XINPUT_STATE state;
	bool c = controllerConnected(i, state);
	if(c==false)
		return NULL;

	double LX = state.Gamepad.sThumbLX;
	double LY = state.Gamepad.sThumbLY;

	//determine how far the controller is pushed
	double magnitude = sqrt(LX * LX + LY * LY);
	double angle = atan2(LY, LX);

	//determine the direction the controller is pushed
	double normalizedLX = LX / magnitude;
	double normalizedLY = LY / magnitude;

	double normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_DEADZONE)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767)
			magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_DEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_DEADZONE);
		return X::PolarPoint(angle, normalizedMagnitude);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}
	return NULL;
}

const wchar_t *empty_wstr = L"";
std::wstring Peripherals::unloadQueue()
{
	std::wstring str = characterqueue.str();
	characterqueue.str(empty_wstr);
	queueupdate = false;
	return str;
}
void Peripherals::queueState(bool b)
{
	queuestate = b;
}

bool Peripherals::queueUpdate()
{
	return queueupdate;
}

void Peripherals::MouseMoveIn(int x, int y)
{
	Peripherals::mousex = x;
	Peripherals::mousey = y;
}

void Peripherals::mouseClickLeftIn(int x, int y)
{
	mouseclick_xl = x;
	mouseclick_yl = y;
	mouseclicked_l = true;
}

bool Peripherals::mouseClickedLeft()
{
	if (mouseclicked_l)
	{
		mouseclicked_l = false;
		return true;
	}
	return false;
}

void Peripherals::mouseClickRightIn(int x, int y)
{
	mouseclick_xr = x;
	mouseclick_yr = y;
	mouseclicked_r = true;
}

bool Peripherals::mouseClickedRight()
{
	if (mouseclicked_r)
	{
		mouseclicked_r = false;
		return true;
	}
	return false;
}

X::Point Peripherals::mousePos()
{
	return X::Point(Peripherals::mousex, Peripherals::mousey);
}

X::Point Peripherals::mouseClickLeftPos()
{
	return X::Point(mouseclick_xl, mouseclick_yl);
}

X::Point Peripherals::mouseClickRightPos()
{
	return X::Point(mouseclick_xr, mouseclick_yr);
}