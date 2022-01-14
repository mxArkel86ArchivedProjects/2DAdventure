#include "ColorRect.h"

ColorRect::ColorRect(string color, X::Point topleft, X::Point bottomright){
    this->colorname = color;
    this->rect_ = X::Rect(topleft, bottomright);
}
ColorRect* ColorRect::operator&(){
    return this->id;
}
X::Rect ColorRect::rect(){
    return rect_;
}
string ColorRect::getColor(){
    return colorname;
}