#pragma once
#include "X.h"

using X::Rect;
using X::Point;

class Collider : public X::Rect{
    Collider* id;
    bool vis;
    public:
    Collider(Point topleft, Point bottomright, bool visible);
    Collider* operator&();
    bool visible();
    operator X::Rect() const;
};