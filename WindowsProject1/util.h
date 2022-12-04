#pragma once
#include "imports.h"


using namespace boost;
namespace fs = boost::filesystem;

#define SafeRelease(res){if (res)res->Release();res = nullptr;}

struct INFO {
	static wstring APP_PATH;
};

