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