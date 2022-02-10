#pragma once
#include "X.h"

using X::Rect;
using X::Point;

struct CollisionReturn {
	bool x_collision;
	bool y_collision;
	bool collision();
	char intent_x;
	char intent_y;
	double disp_x;
	double disp_y;
};

class CollisionUtil{
	public:
	static CollisionReturn DynamicCollision(Rect a, Rect b, double dx, double dy);
	static bool staticCollision(Rect a, Rect b);
};