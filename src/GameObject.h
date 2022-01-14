#pragma once
#include "X.h"

using X::Rect;
using X::Point;

class GameObject{
    GameObject* id;
    bool collider;
    Rect rect;
    public:
    GameObject(Point topleft, Point bottomright, bool collider);
    GameObject* operator&();
    Rect getRect();
};