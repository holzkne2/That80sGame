#include "pch.h"
#include "GameObject.h"
#include "Game.h"

GameObject::GameObject()
{
	m_transform = std::make_unique<Transform>();
	m_active = true;
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
		m_components[i]->Update();
	}
}

void GameObject::LateUpdateComponents()
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->LateUpdate();
	}
}