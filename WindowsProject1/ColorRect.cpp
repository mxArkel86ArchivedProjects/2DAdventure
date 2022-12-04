#include "ColorRect.h"

ColorRect::ColorRect(string color, X::Point topleft, X::Point bottomright, bool foreground):X::Rect(topleft, bottomright){
    this->colorname = color;
    this->foreground = foreground;
}
ColorRect* ColorRect::operator&(){
    return this->id;
}
string ColorRect::getColor(){
    return colorname;
}

bool ColorRect::isForeground(){
    return foreground;
}
ColorRect::operator Rect() const{
        return (Rect)*this;
}