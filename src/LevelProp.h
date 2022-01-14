#pragma once
#include "X.h"


class LevelProp : public X::Rect{
    double z;
    string res_;
    public:
        LevelProp();
        LevelProp(string res, X::Point topleft, X::Point bottomright, double z);
        double getZ();
        string res();
        operator X::Rect() const;
};