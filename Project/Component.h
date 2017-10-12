#pragma once

#include "pch.h"

class GameObject;
class Transform;

class Component
{
public:
	Component(GameObject*);
	virtual ~Component();

	GameObject* GetGameObject() { return m_gameObject; }

protected:
	GameObject* m_gameObject;
};

