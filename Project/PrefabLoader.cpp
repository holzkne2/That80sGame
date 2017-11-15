#include "pch.h"
#include "PrefabLoader.h"
#include "AssetHelper.h"
#include <map>
#include "Object.h"
#include "GameObject.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include "Game.h"

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
	// TODO: Create Folder, ..\ option 
	file.open(".\\Assets\\Prefabs\\" + gameObject->GetName() + ".prefab", std::ios::out);
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

GameObject* PrefabLoader::LoadPrefab(const std::string& name)
{
	std::string path = ".\\Assets\\Prefabs\\" + name + ".prefab";
	if (!AssetHelper::FileExists(path))
		return nullptr;

	GameObject* returnObject = 0;
	std::map<int, ObjectData> objects = std::map<int, ObjectData>();
	

	std::ifstream file(path);
	std::vector<std::string> lines;
	GetTokens(file, '\n', lines);
	
	// Get ID and other data
	for (unsigned int l = 0; l < lines.size(); l++)
	{
		std::vector<std::string> tokens;
		GetTokens(lines[l], ':', tokens);

		if (tokens.size() != 2)
			continue;

		if (tokens[0].substr(0, 5) == "class")
		{
			ObjectData data = ObjectData();
			data.type = tokens[0];

			// Load data
			std::vector<std::string> stringData;
			do
			{
				stringData.clear();
				GetTokens(lines[++l], ':', stringData);

				if (stringData.size() == 0)
					break;

				data.member_value.insert(std::pair<std::string, std::string>(
					stringData[0], stringData[1]));
			} while (l < lines.size() - 1);

			if (tokens[0] == "class GameObject")
			{
				GameObject* gobj = new GameObject(data.member_value["Name"]);
				data.object = gobj;
				if (gobj->GetName() == name)
					returnObject = gobj;
				Game::Get()->GetScene()->AddGameObject(gobj);
			}
			else if (tokens[0] == "class Transform")
			{
				Transform* transform = new Transform(nullptr);
				data.object = transform;
			}
			else if (tokens[0] == "class ModelRenderer")
			{
				ModelRenderer* modelRenderer = new ModelRenderer(nullptr);
				data.object = modelRenderer;
			}

			if (data.object != nullptr)
				data.object->Load(data.member_value);

			objects.insert(std::pair<int, ObjectData>(std::stoi(tokens[1]), data));
		}
	}

	// Link IDs
	for (auto itr = objects.begin(); itr != objects.end(); ++itr)
	{
		if (itr->second.type == "class GameObject")
		{
			dynamic_cast<GameObject*>(itr->second.object)->SetTransform(
				dynamic_cast<Transform*>(objects[
					stoi(itr->second.member_value["Transform"])].object));
		}
		else if (itr->second.type == "class Transform")
		{
			int parentID = stoi(itr->second.member_value["Parent"]);
			if (objects.find(parentID) == objects.end())
				continue;
			Transform* transform = dynamic_cast<Transform*>(itr->second.object);
			transform->SetParent(dynamic_cast<Transform*>(objects[parentID].object));
		}
		else // Componets
		{
			Component* component = dynamic_cast<Component*>(itr->second.object);
			if (component == nullptr)
				continue;

			component->SetGameObject(
				dynamic_cast<GameObject*>(objects[
					stoi(itr->second.member_value["GameObject"])].object));

			if (itr->second.type == "class ModelRenderer")
			{
				ModelRenderer* modelRenderer = dynamic_cast<ModelRenderer*>(component);
				std::string fileName = itr->second.member_value["file Name"];
				std::wstring wfileName(fileName.length(), L' ');
				std::copy(fileName.begin(), fileName.end(), wfileName.begin());
				modelRenderer->SetModel(
					Game::Get()->GetDeviceResources()->GetD3DDevice(),
					wfileName.c_str(),
					modelRenderer->GetAlpha());
			}
		}
	}

	return returnObject;
}

void PrefabLoader::SaveToPrefab(Object*, std::map<int, ObjectData>&)
{

}