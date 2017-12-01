#pragma once

#include "pch.h"
#include "Object.h"

class GameObject;
class Transform;
class PhysicsComponent;
class PrefabLoader;

class Component : public Object
{
public:
	friend PrefabLoader;

	Component(GameObject*);
	virtual ~Component();

	GameObject* GetGameObject() const { return m_gameObject; }

	virtual void Update() {}
	virtual void LateUpdate() {}

	bool IsActive();

	bool IsSelfActive() { return m_active; }
	virtual void SetActive(bool active) { m_active = active; }

	virtual void CollisionStay(const PhysicsComponent*) const {}

	virtual void Save(std::map<std::string, std::string>& data) override;
	virtual void Load(std::map<std::string, std::string>& data) override;

protected:
	GameObject* m_gameObject;
	bool m_active;

private:
	void SetGameObject(GameObject*);
};

