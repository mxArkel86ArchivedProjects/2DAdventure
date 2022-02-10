#pragma once
#include "X.h"

class ColorRect: public X::Rect{
    ColorRect* id;
    string colorname;
    bool foreground;
    public:
    ColorRect(string color, X::Point topleft, X::Point bottomright, bool foreground);
    ColorRect* operator&();
    string getColor();
    bool isForeground();
    operator X::Rect() const;
};