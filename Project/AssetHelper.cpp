#include "pch.h"
#include "AssetHelper.h"

#include <sys/types.h>
#include <sys/stat.h>

#include "Game.h"

using namespace DirectX;

AssetHelper::AssetHelper()
{
}


AssetHelper::~AssetHelper()
{
}

std::wstring AssetHelper::GetModelPath(const wchar_t* file)
{
	std::wstring file_name = file;
	if (FileExists(wc_s(file_name.c_str())))
		return file_name;

	std::wstring path = L".\\Assets\\Models\\" + file_name;;

	if (!FileExists(wc_s(path.c_str())))
	{
		path = L"..\\Assets\\Models\\" + file_name;
		if (!FileExists(wc_s(path.c_str())))
			return L"ERROR";
	}
	return path;
}

std::wstring AssetHelper::GetSpritePath(const wchar_t* file)
{
	std::wstring file_name = file;
	std::wstring path = L".\\Assets\\Sprites\\" + file_name;;

	if (!FileExists(wc_s(path.c_str())))
	{
		path = L"..\\Assets\\Sprites\\" + file_name;
		if (!FileExists(wc_s(path.c_str())))
			return false;
	}
	return path;
}

std::wstring AssetHelper::GetFontPath(const wchar_t* file)
{
	std::wstring file_name = file;
	std::wstring path = L".\\Assets\\Fonts\\" + file_name;;

	if (!FileExists(wc_s(path.c_str())))
	{
		path = L"..\\Assets\\Fonts\\" + file_name;
		if (!FileExists(wc_s(path.c_str())))
			return false;
	}
	return path;
}

bool AssetHelper::FileExists(const std::string& file)
{
	struct  stat buffer;
	return (stat(file.c_str(), &buffer) == 0);
}
