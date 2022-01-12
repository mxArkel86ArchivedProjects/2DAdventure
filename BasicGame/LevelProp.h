#include "X.h"


class LevelProp{
    double z;
    public:
        X::Rect rect;
        LevelProp();
        LevelProp(X::Point topleft, X::Point bottomright, double z);
        double getZ();
};