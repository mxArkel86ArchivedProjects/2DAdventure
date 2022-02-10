#include "../headers/X.h"
X::Point::Point(double x, double y) {
	this->x = x;
	this->y = y;
}
X::Point::Point() {
	Point(0, 0);
}
double X::Point::getX() {
	if(null_)
	throw "POINT IS NULL";
	return x;
}
double X::Point::getY() {
	if(null_)
	throw "POINT IS NULL";
	return y;
}
int X::Point::getIX() {
	if(null_)
	throw "POINT IS NULL";
	return (int)x;
}
int X::Point::getIY() {
	if(null_)
	throw "POINT IS NULL";
	return (int)y;
}

void X::Point::addDotSelf(Point p) {
	null_ = false;
	this->x += p.x;
	this->y += p.y;
}
void X::Point::multiplySelf(double d) {
	this->x *= d;
	this->y *= d;
}
X::Point::operator int(){
	return !null_;
}

X::Point X::Point::multiply(double d) {
	null_ = false;
	return X::Point(this->x * d, this->y * d);
}
X::Point X::Point::add(Point p) {
	null_ = false;
	return X::Point(this->x + p.x, this->y * p.y);
}
X::Point::Point(int i){
	if(i==0)
		null_ = true;
}

X::PolarPoint::PolarPoint(){

}
X::PolarPoint::operator int(){
	return !null_;
}

X::PolarPoint::PolarPoint(double angle, double mag){
	this->angle = angle;
	this->magnitude = mag;
}
void X::PolarPoint::setAngle(double a){
	null_ = false;
	this->angle = a;
}

void X::PolarPoint::setMagnitude(double b){
	null_ = false;
	this->magnitude = b;
}
double X::PolarPoint::getAngle(){
	if(null_)
		throw "POINT IS NULL";
	return angle;
}
double X::PolarPoint::getMagnitude(){
	if(null_)
		throw "POINT IS NULL";
	return magnitude;
}
X::PolarPoint::PolarPoint(int i){
	if(i==0)
		null_ = true;
}

void X::Rect::shift(double l, double t, double r, double b) {
	this->topleft.addDotSelf(X::Point(l, t));
	this->bottomright.addDotSelf(X::Point(r, b));
}
void X::Rect::move(double l, double t) {
	this->topleft.addDotSelf(X::Point(l, t));
	this->bottomright.addDotSelf(X::Point(l, t));
}

double X::Rect::width() {
	return abs(right() - left());
}

double X::Rect::height() {
	return abs(bottom() - top());
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
X::Rect X::Rect::expanded(double amount){
	return X::Rect(left()-amount, top()-amount,right()+amount,bottom()+amount);
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
void X::Rect::setLeft(double d){
	this->topleft.setX(d);
}
void X::Rect::setRight(double d){
	this->bottomright.setX(d);
}
void X::Rect::setTop(double d){
	this->topleft.setY(d);
}
void X::Rect::setBottom(double d){
	this->bottomright.setY(d);
}
void X::Point::setX(double d){
	x =d;
}
void X::Point::setY(double d){
	y = d;
}