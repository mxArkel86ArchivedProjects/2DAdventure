#pragma once
#include "X.h"

using X::Rect;
using X::Point;

class GameObject : public X::Rect{
    GameObject* id;
    bool collider;
    public:
    GameObject(Point topleft, Point bottomright, bool collider);
    GameObject* operator&();
    operator X::Rect() const;
};