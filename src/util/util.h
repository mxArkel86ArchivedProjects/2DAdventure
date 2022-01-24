#pragma once
#include "Windows.h"
#include <iostream>
#include <string>

template<typename T>
static inline void SafeRelease(T*& res)
{
    if (res)
        res->Release();
    res = nullptr;
}