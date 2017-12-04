#include "pch.h"
#include "GameObject.h"
#include "Game.h"

GameObject::GameObject()
{
	m_transform = std::make_unique<Transform>(this);
}

GameObject::GameObject(std::string name) : GameObject()
{
	m_name = name;
}

GameObject::~GameObject()
{
}

void GameObject::MakeRectTransform()
{
	RectTransform* result = dynamic_cast<RectTransform*>(m_transform.get());
	if (result == NULL)
	{
		m_transform = std::make_unique<RectTransform>(m_transform.get());
	}
}

void GameObject::UpdateComponents()
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		if (m_components[i]->IsActive())
			m_components[i]->Update();
	}
}

void GameObject::LateUpdateComponents()
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		if (m_components[i]->IsActive())
			m_components[i]->LateUpdate();
	}
}

bool GameObject::IsActive()
{
	if (m_transform->GetParent() == nullptr)
		return Object::IsSelfActive();
	else
		return Object::IsSelfActive() && m_transform->GetParent()->GetGameObject()->IsActive();
}

void GameObject::AddPrebuiltComponent(Component* component)
{
	m_components.push_back(std::unique_ptr<Component>(component));
	if (component->IsActive())
		component->OnEnable();
	else
		component->OnDisable();
}

void GameObject::OnDisable()
{
	m_transform->OnDisable();
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->OnDisable();
	}
}

void GameObject::OnEnable()
{
	m_transform->OnEnable();
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->OnEnable();
	}
}

void GameObject::CollisionStay(const PhysicsComponent* other)
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->CollisionStay(other);
	}
}

void GameObject::Save(std::map<std::string, std::string>& data)
{
	Object::Save(data);

	data.insert(std::pair<std::string, std::string>("Name", m_name));
	data.insert(std::pair<std::string, std::string>("Active", Object::IsSelfActive() ? "True" : "False"));
	data.insert(std::pair<std::string, std::string>("Transform", std::to_string((int)m_transform.get())));
}

void GameObject::Load(std::map<std::string, std::string>& data)
{
	Object::Load(data);

	m_active = (data["Active"] == "True");
}