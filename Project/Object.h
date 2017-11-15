#pragma once
#include <map>

class Object
{
public:
	Object();
	virtual ~Object();

	virtual void Save(std::map<std::string, std::string>& data) {}
};

