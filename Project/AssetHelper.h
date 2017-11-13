#pragma once
#include "pch.h"

static class AssetHelper
{
public:
	AssetHelper();
	virtual ~AssetHelper();

	static std::wstring GetModelPath(const wchar_t*);

	static std::wstring GetSpritePath(const wchar_t*);

	static std::wstring GetFontPath(const wchar_t*);

private:
	static bool FileExists(const std::string&);
};

