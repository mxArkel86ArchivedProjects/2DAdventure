#include "GameObject.h"

GameObject::GameObject(Point topleft_, Point bottomright_, bool visible_):
Rect(topleft_, bottomright_),
vis(visible_),
id(this)
{
    
}
GameObject::operator Rect() const{
        return (Rect)*this;
}
bool GameObject::visible(){
        return vis;
}
GameObject* GameObject::operator&(){
        return id;
}