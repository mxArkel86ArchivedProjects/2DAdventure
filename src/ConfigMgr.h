#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/predicate.hpp"
#include <fstream>
#include "GameObject.h"
#include "LevelProp.h"
#include "ColorRect.h"
#include <cstdio>
#include <vector>

using namespace std;
using namespace boost;
using std::ifstream;
using std::ofstream;
using std::vector;

void saveconfig(wstring path, vector<GameObject *>& newcolliders, vector<LevelProp *>& newprops, vector<ColorRect *> newcolorrect, vector<GameObject *>& colliders, vector<LevelProp *>& props, vector<ColorRect *>& colorrect);
void readconfig(wstring path, vector<GameObject *>& colliders, vector<LevelProp *>& props, vector<ColorRect *>& colorrect);