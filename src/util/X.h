#pragma once

#include "../imports.h"
#include <string>
using std::string;

namespace X {
	class Point
	{
		bool null_ = false;
		double x;
		double y;
	public:
		Point(double x, double y);
		Point();
		Point(int);
		operator int();
		double getX();
		double getY();
		void setX(double);
		void setY(double);
		int getIX();
		int getIY();
		void addDotSelf(Point p);
		void multiplySelf(double d);
		Point multiply(double d);
		Point add(Point p);
		D2D1_POINT_2F P2F();
		static Point Empty();
	};
	class PolarPoint
	{
		bool null_ = false;
		double angle;
		double magnitude;
	public:
		PolarPoint(double angle, double magnitude);
		PolarPoint();
		operator int();
		PolarPoint(int);
		double getAngle();
		double getMagnitude();
		void setAngle(double);
		void setMagnitude(double);
		
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
		void setLeft(double);
		void setRight(double);
		void setTop(double);
		void setBottom(double);
		Point topLeft();
		Point bottomRight();
		Rect expanded(double);
		D2D1_RECT_F toRectF();
	};
}

