//
// pch.cpp
// Include the standard header and generate the precompiled header.
//

#include "pch.h"

std::string wc_s(const wchar_t* wcp)
{
	std::wstring ws(wcp);
	std::string str(ws.begin(), ws.end());
	return str;
}