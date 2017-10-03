#include "pch.h"
#include "Component.h"
#include "GameObject.h"


Component::Component(GameObject* gameObject)
{
	m_gameObject = gameObject;
	m_transform = gameObject->GetTransform();
}


Component::~Component()
{
}
