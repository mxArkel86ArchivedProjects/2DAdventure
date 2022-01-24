#pragma once
#include "../util/X.h"

class ColorRect: public X::Rect{
    ColorRect* id;
    string colorname;
    public:
    ColorRect(string color, X::Point topleft, X::Point bottomright);
    ColorRect* operator&();
    string getColor();
    operator X::Rect() const;
};