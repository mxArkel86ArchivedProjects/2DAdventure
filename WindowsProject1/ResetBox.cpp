#include "ResetBox.h"

ResetBox::ResetBox(Point topleft, Point bottomright, string checkpoint):
Rect(topleft, bottomright){
this->checkpoint = checkpoint;
this->id = this;
}

ResetBox* ResetBox::operator&(){
    return id;
}

ResetBox::operator Rect() const{
    return (Rect)*this;
}

string ResetBox::getCheckpoint(){
    return checkpoint;
}