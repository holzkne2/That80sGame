#include "pch.h"
#include "GameObject.h"
#include "Game.h"

GameObject::GameObject()
{
	m_transform = std::make_unique<Transform>();
	Game::Get();
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
