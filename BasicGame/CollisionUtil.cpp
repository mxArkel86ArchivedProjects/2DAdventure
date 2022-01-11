#include "CollisionUtil.h"
#include <math.h>

CollisionReturn DynamicCollision(Rect a, Rect b, double dx, double dy) {
	CollisionReturn ret =  CollisionReturn();

	char intent_x = dx > 0.01 ? 1 : dx < -0.01 ? -1 : 0;
	char intent_y = dy > 0.01 ? 1 : dy < -0.01 ? -1 : 0;

	ret.intent_x = intent_x;
	ret.intent_y = intent_y;

	if (intent_x == 1 && intent_y == 1) {// quadrant I
		Point top_left = Point(a.left(), a.top());
		Point top_right =  Point(a.right(), a.top());
		Point bottom_right =  Point(a.right(), a.bottom());

		Point object_bottom_left = Point(b.left(), b.bottom());
		Point object_bottom_right = Point(b.right(), b.bottom());
		Point object_top_left = Point(b.left(), b.top());

		bool left_intersect = top_right.getX() >= object_bottom_left.getX() && top_left.getX() < object_bottom_left.getX();
		bool right_intersect = top_left.getX() <= object_bottom_right.getX() && top_right.getX() > object_bottom_right.getX();
		bool center_intersect_x = top_left.getX() >= object_bottom_left.getX() && top_right.getX() <= object_bottom_right.getX();
		bool pass_by_x = top_right.getX() <= object_bottom_left.getX() && top_left.getX() + dx >= object_bottom_right.getX();

		bool top_intersect = bottom_right.getY() >= object_top_left.getY() && top_left.getY() < object_top_left.getY();
		bool bottom_intersect = top_left.getY() <= object_bottom_right.getY() && bottom_right.getY() > object_bottom_right.getY();
		bool center_intersect_y = bottom_right.getY() <= object_bottom_right.getY() && top_left.getY() >= object_top_left.getY();
		bool pass_by_y = bottom_right.getY() <= object_top_left.getY() && top_left.getY() - dy >= object_bottom_right.getY();

		bool inline_y = left_intersect || right_intersect || center_intersect_x;
		bool inline_x = top_intersect || bottom_intersect || center_intersect_y;
		// check if object is valid before move
		if ((inline_y || pass_by_x) && a.top() <= b.bottom() && a.top() + dy > b.bottom()) {
			ret.y_collision = true;
			ret.disp_y = ceil(a.top() - b.bottom());
			return ret;
		}
		if ((inline_x || pass_by_y) && a.right() <= b.left() && a.right() + dx > b.left()) {
			ret.x_collision = true;
			ret.disp_x = floor(b.left() - a.right());
		}
	}
	else if (intent_x == 0 && intent_y == 1) {// y axis up
		Point top_left =  Point(a.left(), a.top());
		Point top_right =  Point(a.right(), a.top());

		Point object_bottom_left =  Point(b.left(), b.bottom());
		Point object_bottom_right =  Point(b.right(), b.bottom());

		bool left_intersect = top_right.getX() >= object_bottom_left.getX() && top_left.getX() < object_bottom_left.getX();
		bool right_intersect = top_left.getX() <= object_bottom_right.getX() && top_right.getX() > object_bottom_right.getX();
		bool center_intersect_x = top_left.getX() >= object_bottom_left.getX() && top_right.getX() <= object_bottom_right.getX();
		bool pass_by_x = top_right.getX() <= object_bottom_left.getX() && top_left.getX() + dx >= object_bottom_right.getX();

		bool inline_y = left_intersect || right_intersect || center_intersect_x;
		// check if object is valid before move
		if ((inline_y || pass_by_x) && a.top() <= b.bottom() && a.top() + dy > b.bottom()) {
			ret.y_collision = true;
			ret.disp_y = ceil(a.top() - b.bottom());
		}
	}
	else if (intent_x == -1 && intent_y == 1) {// quadrant II
		Point top_left =  Point(a.left(), a.top());
		Point top_right =  Point(a.right(), a.top());
		Point bottom_left =  Point(a.left(), a.bottom());

		Point object_bottom_left =  Point(b.left(), b.bottom());
		Point object_bottom_right =  Point(b.right(), b.bottom());
		Point object_top_right =  Point(b.right(), b.top());

		bool left_intersect = top_right.getX() >= object_bottom_left.getX() && top_left.getX() < object_bottom_left.getX();
		bool right_intersect = top_left.getX() <= object_bottom_right.getX() && top_right.getX() > object_bottom_right.getX();
		bool center_intersect_x = top_left.getX() >= object_bottom_left.getX() && top_right.getX() <= object_bottom_right.getX();
		bool pass_by_x = top_right.getX() <= object_bottom_left.getX() && top_left.getX() + dx >= object_bottom_right.getX();

		bool top_intersect = bottom_left.getY() >= object_top_right.getY() && top_left.getY() < object_top_right.getY();
		bool bottom_intersect = top_left.getY() <= object_bottom_right.getY() && bottom_left.getY() > object_bottom_right.getY();
		bool center_intersect_y = bottom_left.getY() <= object_bottom_right.getY() && top_left.getY() >= object_top_right.getY();
		bool pass_by_y = bottom_left.getY() <= object_top_right.getY() && top_left.getY() - dy >= object_bottom_right.getY();

		bool inline_y = left_intersect || right_intersect || center_intersect_x;
		bool inline_x = top_intersect || bottom_intersect || center_intersect_y;
		// check if object is valid before move
		if ((inline_y || pass_by_x) && a.top() <= b.bottom() && a.top() + dy > b.bottom()) {
			ret.y_collision = true;
			ret.disp_y = ceil(a.top() - b.bottom());
		}
		if ((inline_x || pass_by_y) && a.left() >= b.right() && a.left() + dx < b.right()) {
			ret.x_collision = true;
			ret.disp_x = floor(a.left() - b.right());
		}
	}
	else if (intent_x == -1 && intent_y == 0) {// x axis left
		Point top_left =  Point(a.left(), a.top());
		Point bottom_left =  Point(a.left(), a.bottom());

		Point object_top_right =  Point(b.right(), b.top());
		Point object_bottom_right =  Point(b.right(), b.bottom());

		bool top_intersect = bottom_left.getY() >= object_top_right.getY() && top_left.getY() < object_top_right.getY();
		bool bottom_intersect = top_left.getY() <= object_bottom_right.getY() && bottom_left.getY() > object_bottom_right.getY();
		bool center_intersect_y = bottom_left.getY() <= object_bottom_right.getY() && top_left.getY() >= object_top_right.getY();
		bool pass_by_y = bottom_left.getY() <= object_top_right.getY() && top_left.getY() - dy >= object_bottom_right.getY();

		bool inline_x = top_intersect || bottom_intersect || center_intersect_y;
		// check if object is valid before move
		if ((inline_x || pass_by_y) && a.left() >= b.right() && a.left() + dx < b.right()) {
			ret.x_collision = true;
			ret.disp_x = ceil(b.right() - a.left());
		}
	}
	else if (intent_x == -1 && intent_y == -1) {// quadrant III
		Point top_left =  Point(a.left(), a.top());
		Point bottom_right =  Point(a.right(), a.bottom());
		Point bottom_left =  Point(a.left(), a.bottom());

		Point object_top_left =  Point(b.left(), b.top());
		Point object_top_right =  Point(b.right(), b.top());
		Point object_bottom_right =  Point(b.right(), b.bottom());

		bool left_intersect = bottom_right.getX() >= object_top_left.getX() && top_left.getX() < object_top_left.getX();
		bool right_intersect = top_left.getX() <= object_bottom_right.getX() && bottom_right.getX() > object_bottom_right.getX();
		bool center_intersect_x = top_left.getX() >= object_top_left.getX() && bottom_right.getX() <= object_bottom_right.getX();
		bool pass_by_x = bottom_right.getX() <= object_top_left.getX() && top_left.getX() + dx >= object_bottom_right.getX();

		bool top_intersect = bottom_left.getY() >= object_top_right.getY() && top_left.getY() < object_top_right.getY();
		bool bottom_intersect = top_left.getY() <= object_bottom_right.getY() && bottom_left.getY() > object_bottom_right.getY();
		bool center_intersect_y = bottom_left.getY() <= object_bottom_right.getY() && top_left.getY() >= object_top_right.getY();
		bool pass_by_y = bottom_left.getY() <= object_top_right.getY() && top_left.getY() - dy >= object_bottom_right.getY();

		bool inline_y = left_intersect || right_intersect || center_intersect_x;
		bool inline_x = top_intersect || bottom_intersect || center_intersect_y;
		// check if object is valid before move
		if ((inline_y || pass_by_x) && a.bottom() <= b.top() && a.bottom() - dy > b.top()) {
			ret.y_collision = true;
			ret.disp_y = ceil(a.bottom() - b.top());
		}
		if ((inline_x || pass_by_y) && a.left() >= b.right() && a.left() + dx < b.right()) {
			ret.x_collision = true;
			ret.disp_x = floor(a.left() - b.right());
		}
	}
	else if (intent_x == 0 && intent_y == -1) {// y axis down
		Point bottom_left =  Point(a.left(), a.bottom());
		Point bottom_right =  Point(a.right(), a.bottom());

		Point object_top_left =  Point(b.left(), b.top());
		Point object_top_right =  Point(b.right(), b.top());

		bool left_intersect = bottom_right.getX() >= object_top_left.getX() && bottom_left.getX() < object_top_left.getX();
		bool right_intersect = bottom_left.getX() <= object_top_right.getX() && bottom_right.getX() > object_top_right.getX();
		bool center_intersect_x = bottom_left.getX() >= object_top_left.getX() && bottom_right.getX() <= object_top_right.getX();
		bool pass_by_x = bottom_right.getX() <= object_top_left.getX() && bottom_left.getX() + dx >= object_top_right.getX();

		bool inline_y = left_intersect || right_intersect || center_intersect_x;
		// check if object is valid before move
		if ((inline_y || pass_by_x) && a.bottom() <= b.top() && a.bottom() - dy > b.top()) {
			ret.y_collision = true;
			ret.disp_y = floor(b.top() - a.bottom());
		}
	}
	else if (intent_x == 1 && intent_y == -1) {// quadrant IV
		Point top_right =  Point(a.right(), a.top());
		Point bottom_right =  Point(a.right(), a.bottom());
		Point bottom_left =  Point(a.left(), a.bottom());

		Point object_top_left =  Point(b.left(), b.top());
		Point object_top_right =  Point(b.right(), b.top());
		Point object_bottom_left =  Point(b.left(), b.bottom());

		bool left_intersect = bottom_right.getX() >= object_top_left.getX() && bottom_left.getX() < object_top_left.getX();
		bool right_intersect = bottom_left.getX() <= object_top_right.getX() && bottom_right.getX() > object_top_right.getX();
		bool center_intersect_x = bottom_left.getX() >= object_top_left.getX() && bottom_right.getX() <= object_top_right.getX();
		bool pass_by_x = bottom_right.getX() <= object_top_left.getX() && bottom_left.getX() + dx >= object_top_right.getX();

		bool top_intersect = bottom_left.getY() >= object_top_right.getY() && top_right.getY() < object_top_right.getY();
		bool bottom_intersect = top_right.getY() <= object_bottom_left.getY() && bottom_left.getY() > object_bottom_left.getY();
		bool center_intersect_y = bottom_left.getY() <= object_bottom_left.getY() && top_right.getY() >= object_top_right.getY();
		bool pass_by_y = bottom_left.getY() <= object_top_right.getY() && top_right.getY() - dy >= object_bottom_left.getY();

		bool inline_y = left_intersect || right_intersect || center_intersect_x;
		bool inline_x = top_intersect || bottom_intersect || center_intersect_y;
		// check if object is valid before move
		if ((inline_y || pass_by_x) && a.bottom() <= b.top() && a.bottom() - dy > b.top()) {
			ret.y_collision = true;
			ret.disp_y = ceil(a.bottom() - b.top());
		}
		if ((inline_x || pass_by_y) && a.right() <= b.left() && a.right() + dx > b.left()) {
			ret.x_collision = true;
			ret.disp_x = floor(b.left() - a.right());
		}

	}
	else if (intent_x == 1 && intent_y == 0) {// x axis right
		Point top_right =  Point(a.right(), a.top());
		Point bottom_right =  Point(a.right(), a.bottom());

		Point object_top_left =  Point(b.left(), b.top());
		Point object_bottom_left =  Point(b.left(), b.bottom());

		bool top_intersect = bottom_right.getY() >= object_top_left.getY() && top_right.getY() < object_top_left.getY();
		bool bottom_intersect = top_right.getY() <= object_bottom_left.getY() && bottom_right.getY() > object_bottom_left.getY();
		bool center_intersect_y = bottom_right.getY() <= object_bottom_left.getY() && top_right.getY() >= object_top_left.getY();
		bool pass_by_y = bottom_right.getY() <= object_top_left.getY() && top_right.getY() - dy >= object_bottom_left.getY();

		bool inline_x = top_intersect || bottom_intersect || center_intersect_y;
		// check if object is valid before move
		if ((inline_x || pass_by_y) && a.right() <= b.left() && a.right() + dx > b.left()) {
			ret.x_collision = true;
			ret.disp_x = floor(b.left() - a.right());
		}
	}
	return ret;
}

bool staticCollision(Rect a, Rect b) {
	bool inline_x = (a.top() < b.bottom() && a.top() > b.top())
		|| (a.bottom() < b.bottom() && a.bottom() > b.top()) || (a.top() < b.top() && a.bottom() > b.bottom());
	bool inline_y = (a.left() < b.right() && a.left() > b.left())
		|| (a.right() < b.right() && a.right() > b.left()) || (a.left() < b.left() && a.right() > b.right());
	if (inline_x && inline_y) {
		return true;
	}
	return false;
}

bool CollisionReturn::collision() {
	return x_collision || y_collision;
}