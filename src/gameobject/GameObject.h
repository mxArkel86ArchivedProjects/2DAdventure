#pragma once
#include "../util/X.h"

using X::Rect;
using X::Point;

class GameObject : public X::Rect{
    GameObject* id;
    bool vis;
    public:
    GameObject(Point topleft, Point bottomright, bool visible);
    GameObject* operator&();
    bool visible();
    operator X::Rect() const;
};