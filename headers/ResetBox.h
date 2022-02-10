#pragma once

#include "X.h"
#include <string>

using std::string;
using X::Point;
using X::Rect;

class ResetBox :public X::Rect {
	ResetBox* id;
	string checkpoint;
public:
	ResetBox(X::Point, X::Point, string);
	ResetBox* operator&();
	operator X::Rect() const;
	string getCheckpoint();
};