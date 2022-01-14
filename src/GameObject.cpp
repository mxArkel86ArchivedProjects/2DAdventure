#include "GameObject.h"

GameObject::GameObject(Point topleft_, Point bottomright_, bool collider_):
Rect(topleft_, bottomright_),
collider(collider_),
id(this)
{
    
}
GameObject::operator Rect() const{
        return (Rect)*this;
}
GameObject* GameObject::operator&(){
        return id;
}