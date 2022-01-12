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

X::Point X::Rect::topLeft(){
			return topleft;
		}
X::Point X::Rect::bottomRight(){
			return bottomright;
}
X::Rect::Rect(){

}
D2D1_RECT_F X::Rect::toRectF(){
	return D2D1::RectF(topleft.getX(), topleft.getY(), bottomright.getX(), bottomright.getY());
}

X::Rect::Rect(double left, double top, double right, double bottom){
	this->topleft = X::Point(left, top);
	this->bottomright = X::Point(right, bottom);
}

D2D1_POINT_2F X::Point::P2F(){
	return D2D1::Point2F(x, y);
}
static X::Point Empty(){
	return X::Point(0,0);
}