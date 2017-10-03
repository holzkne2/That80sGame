#pragma once

#include "pch.h"

class GameObject;
class RectTransform;

class Component
{
public:
	Component(GameObject*);
	virtual ~Component();



protected:
	GameObject* m_gameObject;
	RectTransform* m_transform;
};

