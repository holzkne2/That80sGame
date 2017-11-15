//
// pch.cpp
// Include the standard header and generate the precompiled header.
//

#include "pch.h"
#include <fstream>
#include <iostream>
#include <sstream>

std::string wc_s(const wchar_t* wcp)
{
	std::wstring ws(wcp);
	std::string str(ws.begin(), ws.end());
	return str;
}

void GetTokens(const std::string& str, const char& delimiter, std::vector<std::string>& tokens)
{
	std::istringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delimiter))
		tokens.push_back(token);
}

void GetTokens(std::ifstream& ifs, const char& delimiter, std::vector<std::string>& tokens)
{
	std::string token = "";
	while (std::getline(ifs, token, delimiter))
		tokens.push_back(token);
}