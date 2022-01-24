#include "ColorRect.h"

ColorRect::ColorRect(string color, X::Point topleft, X::Point bottomright):X::Rect(topleft, bottomright){
    this->colorname = color;
}
ColorRect* ColorRect::operator&(){
    return this->id;
}
string ColorRect::getColor(){
    return colorname;
}
ColorRect::operator Rect() const{
        return (Rect)*this;
}