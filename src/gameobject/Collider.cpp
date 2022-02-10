#include "../headers/Collider.h"

Collider::Collider(Point topleft_, Point bottomright_, bool visible_):
Rect(topleft_, bottomright_),
vis(visible_),
id(this)
{
    
}
Collider::operator Rect() const{
        return (Rect)*this;
}
bool Collider::visible(){
        return vis;
}
Collider* Collider::operator&(){
        return id;
}