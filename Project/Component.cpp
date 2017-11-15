#include "pch.h"
#include "Component.h"
#include "GameObject.h"


Component::Component(GameObject* gameObject)
{
	m_gameObject = gameObject;
	m_active = true;
}


Component::~Component()
{
}

bool Component::IsActive()
{
	return m_active && m_gameObject->IsActive();
}

void Component::Save(std::map<std::string, std::string>& data)
{
	Object::Save(data);

	data.insert(std::pair<std::string, std::string>("GameObject", std::to_string((int)m_gameObject)));
	data.insert(std::pair<std::string, std::string>("Active", m_active ? "True" : "False"));
}

void Component::Load(std::map<std::string, std::string>& data)
{
	Object::Load(data);

	m_active = (data["Active"] == "True");
}

void Component::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
	m_gameObject->AddPrebuiltComponent(this);
}