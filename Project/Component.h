#pragma once

#include "pch.h"

class GameObject;
class Transform;

class Component
{
public:
	Component(GameObject*);
	virtual ~Component();



protected:
	GameObject* m_gameObject;
};

