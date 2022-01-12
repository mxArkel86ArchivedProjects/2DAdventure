#include "LevelProp.h"

LevelProp::LevelProp(){

}
LevelProp::LevelProp(X::Point topleft, X::Point bottomright, double z){
    rect = X::Rect(topleft, bottomright);
    this->z = z;
}

double LevelProp::getZ(){
    return z;
}