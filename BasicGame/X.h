#pragma once

#include "imports.h"

namespace X {
	class Point
	{
		double x;
		double y;
	public:
		Point(double x, double y);
		Point();
		double getX();
		double getY();
		int getIX();
		int getIY();
		void addDotSelf(Point p);
		void multiplySelf(double d);
		Point multiply(double d);
		Point add(Point p);
		D2D1_POINT_2F P2F();
	};
	class Rect {
	protected:
		Point topleft;
		Point bottomright;
	public:
		Rect(Point topleft, Point bottomright);
		Rect();
		Rect(double, double, double, double);
		double bottom();
		double top();
		double left();
		double right();
		Point topLeft();
		Point bottomRight();
		D2D1_RECT_F toRectF();
	};
}

