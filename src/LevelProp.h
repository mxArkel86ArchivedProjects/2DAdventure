#pragma once
#include "X.h"


class LevelProp{
    double z;
    string res_;
    public:
        X::Rect rect_;
        LevelProp();
        LevelProp(string res, X::Point topleft, X::Point bottomright, double z);
        double getZ();
        string res();
        X::Rect rect();
};