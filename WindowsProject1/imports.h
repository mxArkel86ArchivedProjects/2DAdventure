#pragma once
#include <windows.h>
#include <WinBase.h>
#include <objidl.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <D2D1.h>
#include <shlwapi.h>
#include <stdlib.h>

#include <fstream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <codecvt>
#include <locale>
#include <string>
#include <vector>
#include <math.h>

#include "boost/filesystem.hpp"
#include "boost/range/iterator_range.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/predicate.hpp"

#pragma comment(lib, "Dwrite")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "Xinput9_1_0.lib")


#define PLAYER_SIZE_ 100
#define PLAYER_SIZE_DEBUG 60
#define GRIDSIZE_ 40
#define GRIDSIZE_DEBUG 30
#define PLAYER_SPEED 2.8
#define PLAYER_SPEED_DEBUG 4
#define GRAV_CONST 0.18
#define JUMP_INTENSITY 6
#define CONSOLE_TEXT_SIZE 16
#define CONSOLE_TEXT_SPACING 4
#define RESOURCE_SIZE 1000
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define TIMER1 1
#define TIMER2 2
