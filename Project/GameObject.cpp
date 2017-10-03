#include "pch.h"
#include "GameObject.h"


GameObject::GameObject()
{
	m_transform = std::make_unique<RectTransform>();
}


GameObject::~GameObject()
{
}
