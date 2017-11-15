#pragma once
#include "GameObject.h"
#include <map>
#include "Object.h"
#include <vector>

static class PrefabLoader
{
private:
	class ObjectData
	{
	public:
		ObjectData() { object = nullptr; }
		~ObjectData() {}

		void Reset() { type = ""; member_value.clear(); }

		std::string type;
		std::map<std::string, std::string> member_value;
		Object* object;

		std::string to_string()
		{
			std::string buffer = "";
			for (auto itr = member_value.begin(); itr != member_value.end(); ++itr)
			{
				buffer += itr->first + ":" + itr->second + "\n";
			}
			return buffer;
		}
	};

public:
	PrefabLoader();
	virtual ~PrefabLoader();

	static void SavePrefab(GameObject*);
	static GameObject* LoadPrefab(const std::string&);

private:
	static void SavePrefab(GameObject*, std::map<int, ObjectData>&);

	static void SaveToPrefab(Object*, std::map<int, ObjectData>&);
};

