#include "X.h"
X::Point::Point(double x, double y) {
	this->x = x;
	this->y = y;
}
X::Point::Point() {
	Point(0, 0);
}
double X::Point::getX() {
	return x;
}
double X::Point::getY() {
	return y;
}
int X::Point::getIX() {
	return (int)x;
}
int X::Point::getIY() {
	return (int)y;
}

void X::Point::addDotSelf(Point p) {
	this->x += p.x;
	this->y += p.y;
}
void X::Point::multiplySelf(double d) {
	this->x *= d;
	this->y *= d;
}

X::Point X::Point::multiply(double d) {
	return X::Point(this->x * d, this->y * d);
}
X::Point X::Point::add(Point p) {
	return X::Point(this->x + p.x, this->y * p.y);
}

double X::Rect::bottom() {
	return bottomright.getY();
}
double X::Rect::top() {
	return topleft.getY();
}
double X::Rect::left() {
	return topleft.getX();
}
double X::Rect::right() {
	return bottomright.getX();
}

X::Rect::Rect(Point topleft, Point bottomright) {
	this->topleft = topleft;
	this->bottomright = bottomright;
}