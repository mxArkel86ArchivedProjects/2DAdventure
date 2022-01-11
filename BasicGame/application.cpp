#include "application.h"
#include "X.h"
#include "util.h"
#include "Peripherals.h"

ID2D1SolidColorBrush* BLACK_b = NULL;
ID2D1SolidColorBrush* WHITE_b = NULL;
ID2D1SolidColorBrush* RED_b = NULL;
ID2D1SolidColorBrush* GREEN_b = NULL;

#define PLAYER_SIZE 38
#define GRIDSIZE 40

X::Point location;
double x1 = 0;
void Application::Paint(ID2D1HwndRenderTarget* pRT) {
	LPRECT WINSIZE = (LPRECT)malloc(sizeof(LPRECT));
	GetClientRect(hWnd, WINSIZE);

	pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

	
	pRT->FillRectangle(D2D1::RectF(WINSIZE->right/2-PLAYER_SIZE/2, WINSIZE->bottom / 2 - PLAYER_SIZE / 2, WINSIZE->right / 2 + PLAYER_SIZE / 2, WINSIZE->bottom / 2 + PLAYER_SIZE / 2), RED_b);

	for (int y = 0; y < 20; y++) {
		for (int x = 0; x < 20; x++) {
			X::Point coord(x, y);
			coord.multiplySelf(GRIDSIZE);
			coord.addDotSelf(location.multiply(-1));

			pRT->DrawLine(D2D1::Point2F(coord.getIX(), coord.getIY()), D2D1::Point2F(coord.getIX(), coord.getIY() + GRIDSIZE), GREEN_b, 1);
			pRT->DrawLine(D2D1::Point2F(coord.getIX(), coord.getIY()), D2D1::Point2F(coord.getIX()+GRIDSIZE, coord.getIY()), GREEN_b, 1);
			
		}
	}
	pRT->DrawRectangle(D2D1::RectF(x1, 80, x1+50, 130), BLACK_b, 4);
}

void Application::tick(long tick) {

	bool pressed = Peripherals::keyPressed(0x41);
	if(pressed)
	x1 += 4;

	/*int intent_x = 0;
	int intent_y = 0;
	if (Peripherals.PERI.keyPressed(KeyEvent.VK_A))
		intent_x--;
	if (Peripherals.PERI.keyPressed(KeyEvent.VK_D))
		intent_x++;
	if (clip) {
		if (Peripherals.PERI.keyPressed(KeyEvent.VK_W))
			intent_y++;
		if (Peripherals.PERI.keyPressed(KeyEvent.VK_S))
			intent_y--;
	}

	if (intent_x != 0 || intent_y != 0) {
		double angle = Math.atan2(intent_y, intent_x);
		double x_ = Math.cos(angle) * P_SPEED;
		movex += x_;

		if (clip) {
			double y_ = Math.sin(angle) * P_SPEED;
			movey += y_;
		}

	}

	if (!clip) {
		vert_velocity -= GRAV_CONST;
		movey += vert_velocity;

		// double dist = Double.MAX_VALUE;

		boolean grounded = false;
		boolean colliding = false;
		Point offset = new Point(camx, camy);
		Rect p2 = new Rect(CANW / 2 - psize, CANH / 2 - psize, psize, psize);

		for (List<Rect> rects : re) {
			for (Rect r : rects) {
				Rect obj = r.multiply(GRIDSIZE).subtract(offset);
				CollisionReturn ret = dynamicCollision(p2, obj, movex, movey);
				if (ret.collidey) {
					colliding = true;
					if (Math.sin(ret.angle) <= 0) {
						grounded = true;
					}
				}

				// CollisionReturn ret2 = dynamicCollision(p2, obj, movex, 0);
				if (ret.collidex) {
					movex = 0;
				}
			}
		}

		if (colliding) {
			// camy += dist;
			vert_velocity = 0;
			if (grounded) {
				if (Peripherals.PERI.keyPressed(KeyEvent.VK_W))
					vert_velocity = jumpintensity;
			}

		}
		else {
			camy -= movey;
		}
	}
	else {
		camy -= movey;
	}

	camx += movex;*/

}

#define BRUSH(y, x)pRT->CreateSolidColorBrush(D2D1::ColorF(x),y);
void Application::InitResources(ID2D1HwndRenderTarget* pRT) {
	BRUSH(&BLACK_b, D2D1::ColorF::Black);
	BRUSH(&WHITE_b, D2D1::ColorF::White);
	BRUSH(&RED_b, D2D1::ColorF::Red);
	BRUSH(&GREEN_b, D2D1::ColorF::Green);
	//pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &GREEN_b);
}
void Application::DeinitResources() {
	SafeRelease(BLACK_b);
	SafeRelease(WHITE_b);
	SafeRelease(RED_b);
	SafeRelease(GREEN_b);
}

Application::Application(HWND hWnd) {
	this->hWnd = hWnd;
}
