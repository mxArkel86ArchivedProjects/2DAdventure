#include "LevelProp.h"

LevelProp::LevelProp(){

}
LevelProp::LevelProp(string res, X::Point topleft, X::Point bottomright, double z):Rect(topleft, bottomright){
    this->z = z;
    this->res_ = res;
}
string LevelProp::res(){
    return res_;
}

double LevelProp::getZ(){
    return z;
}
LevelProp::operator Rect() const{
        return (Rect)*this;
}