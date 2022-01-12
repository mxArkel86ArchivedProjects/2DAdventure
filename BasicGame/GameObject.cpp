#include "GameObject.h"

GameObject::GameObject(Point topleft_, Point bottomright_, bool collider_):
rect(topleft_, bottomright_),
collider(collider_),
id(this)
{
    
}

Rect GameObject::getRect(){
    return rect;
}