#pragma once

#include "pch.h"

class GameObject;
class Transform;
class Collider;

class Component
{
public:
	Component(GameObject*);
	virtual ~Component();

	GameObject* GetGameObject() const { return m_gameObject; }

	virtual void Update() {}
	virtual void LateUpdate() {}

	bool IsActive();

	bool IsSelfActive() { return m_active; }
	virtual void SetActive(bool active) { m_active = active; }

	virtual void CollisionStay(const Collider*) const {}

protected:
	GameObject* m_gameObject;
	bool m_active;
};

