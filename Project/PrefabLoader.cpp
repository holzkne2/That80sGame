#include "pch.h"
#include "PrefabLoader.h"
#include "AssetHelper.h"
#include <map>
#include "Object.h"
#include "GameObject.h"
#include <fstream>
#include <iostream>

PrefabLoader::PrefabLoader()
{
}


PrefabLoader::~PrefabLoader()
{
}

void PrefabLoader::SavePrefab(GameObject* gameObject)
{
	std::map<int, ObjectData> objects = std::map<int, ObjectData>();

	SavePrefab(gameObject, objects);

	std::filebuf file;
	file.open(".\\Assets\\Prefabs\\" + gameObject->GetName() + ".file", std::ios::out);
	std::ostream os(&file);

	for (auto itr = objects.begin(); itr != objects.end(); ++itr)
	{
		os << itr->second.type;
		os << ":";
		os << std::to_string(itr->first) + "\n";
		os << itr->second.to_string();
		os << "\n";
	}

	file.close();
}

void PrefabLoader::SavePrefab(GameObject* gameObject, std::map<int, ObjectData>& objects)
{
	ObjectData data = ObjectData();
	gameObject->Save(data.member_value);
	data.type = typeid(*gameObject).name();
	objects.insert(std::pair<int, ObjectData>((int)gameObject, data));

	data.Reset();
	gameObject->GetTransform()->Save(data.member_value);
	data.type = typeid(*gameObject->GetTransform()).name();
	objects.insert(std::pair<int, ObjectData>((int)(gameObject->GetTransform()), data));

	std::vector<std::unique_ptr<Component>>* components = gameObject->GetComponents();
	for (unsigned int i = 0; i < components->size(); i++)
	{
		data.Reset();
		components->at(i)->Save(data.member_value);
		data.type = typeid(*components->at(i).get()).name();
		objects.insert(std::pair<int, ObjectData>((int)(components->at(i).get()), data));
	}

	for (unsigned int i = 0; i < gameObject->GetTransform()->ChildCount(); i++)
	{
		SavePrefab(gameObject->GetTransform()->GetChild(i)->GetGameObject(), objects);
	}
}

GameObject* PrefabLoader::LoadPrefab(const std::string& str)
{
	if (!AssetHelper::FileExists(str))
		return nullptr;

	std::map<int, ObjectData> objects = std::map<int, ObjectData>();

	// Get ID and other data

	// Link IDs
}

void PrefabLoader::SaveToPrefab(Object*, std::map<int, ObjectData>&)
{

}