#include "application.h"
#include "util.h"

using convert_type = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_type, wchar_t> converter_2;
using namespace boost;

inline bool strcompare(const wchar_t* a, const wchar_t* b)
{
	return _wcsnicmp(a, b, max(wcslen(a), wcslen(b))) == 0;
}

wstring Application::runConsoleCommand(wstring cmd)
{
	vector<wstring> args; // #2: Search for tokens
	split(args, cmd, is_any_of(" "), token_compress_on);
	if (strcompare(cmd.c_str(), L"clip"))
	{
		noclip = !noclip;
		wstring resp = noclip ? L"Successfully enabled noclip" : L"Successfully disabled noclip";
		return resp;
	}
	if (strcompare(cmd.c_str(), L"props"))
	{
		showprops = !showprops;
		wstring resp = showprops ? L"Successfully enabled props" : L"Successfully disabled props";
		return resp;
	}
	if (strcompare(cmd.c_str(), L"init"))
	{
		DeinitResources();
		InitResources();
		return L"Successfully reinitialized app";
	}
	else if (strcompare(cmd.c_str(), L"debug"))
	{
		debugview = !debugview;
		if (debugview)
		{
			player_size = PLAYER_SIZE_DEBUG;
			gridsize = GRIDSIZE_DEBUG;
			onResize(SCREEN_BOUNDS.right(), SCREEN_BOUNDS.bottom());
			return L"Successfully enabled debugview";
		}
		else
		{
			player_size = PLAYER_SIZE_;
			gridsize = GRIDSIZE_;
			onResize(SCREEN_BOUNDS.right(), SCREEN_BOUNDS.bottom());
			return L"Successfully disabled debugview";
		}
	}
	else if (strcompare(cmd.c_str(), L"clear"))
	{
		console_history.clear();
		return L"Successfully cleared console";
	}
	else if (strcompare(cmd.c_str(), L"save"))
	{
		saveconfig(INFO::APP_PATH + L"\\output.txt");
		return L"Successfully saved assets";
	}
	else if (starts_with(cmd, "asset"))
	{
		wstring ws = args[1];
		currentasset = converter_2.to_bytes(ws);
		wstring str = L"Set current asset to " + args[1];
		return str;
	}
	else if (starts_with(cmd, "color"))
	{
		wstring ws = args[1];
		currentcolor = converter_2.to_bytes(ws);
		wstring str = L"Set current color to " + args[1];
		return str;
	}
	else
		return L"";
}
