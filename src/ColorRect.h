#pragma once
#include "X.h"

class ColorRect{
    ColorRect* id;
    X::Rect rect_;
    string colorname;
    public:
    ColorRect(string color, X::Point topleft, X::Point bottomright);
    ColorRect* operator&();
    X::Rect rect();
    string getColor();
};