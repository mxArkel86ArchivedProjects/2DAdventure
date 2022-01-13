#include "LevelProp.h"

LevelProp::LevelProp(){

}
LevelProp::LevelProp(string res, X::Point topleft, X::Point bottomright, double z){
    rect_ = X::Rect(topleft, bottomright);
    this->z = z;
    this->res_ = res;
}
string LevelProp::res(){
    return res_;
}

double LevelProp::getZ(){
    return z;
}

X::Rect LevelProp::rect(){
    return rect_;
}