#pragma once

template<typename T>
static inline void SafeRelease(T*& res)
{
    if (res)
        res->Release();
    res = nullptr;
}
