#pragma once

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
	};
	class Rect {
		Point topleft;
		Point bottomright;
	public:
		Rect(Point topleft, Point bottomright);
		double bottom();
		double top();
		double left();
		double right();
	};
}

