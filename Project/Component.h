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

	virtual void Update() {}
	virtual void LateUpdate() {}

	bool IsActive();

	bool IsSelfActive() { return m_active; }
	void SetActive(bool active) { m_active = active; }

protected:
	GameObject* m_gameObject;
	bool m_active;
};

