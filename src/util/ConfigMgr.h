#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/predicate.hpp"
#include <fstream>
#include "../gameobject/GameObject.h"
#include "../gameobject/LevelProp.h"
#include "../gameobject/ColorRect.h"
#include <cstdio>
#include <vector>
#include <unordered_map>
#include "../imports.h"

using namespace std;
using namespace boost;
using std::ifstream;
using std::ofstream;
using std::vector;

void saveconfig(wstring path, vector<GameObject *>& newcolliders, vector<LevelProp *>& newprops, vector<ColorRect *> newcolorrect, vector<GameObject *>& colliders, vector<LevelProp *>& props, vector<ColorRect *>& colorrect);
void readconfig(wstring path, ID2D1HwndRenderTarget *pRT, vector<GameObject *>& colliders, vector<LevelProp *>& props, vector<ColorRect *>& colorrect, std::unordered_map<string, ID2D1SolidColorBrush *>&colors);