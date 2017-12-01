#pragma once

#include <vector>
#include <type_traits>

#include "pch.h"
#include "RectTransform.h"
#include "Component.h"
#include "Object.h"
#include "PhysicsComponent.h"

class PrefabLoader;

class GameObject : public Object
{
public:
	friend PrefabLoader;
	friend Component;

	GameObject();
	GameObject(std::string);
	virtual ~GameObject();

	template<typename T> T* AddComponent();
	template<typename T> T* GetComponent();
	template<typename T> std::vector<T*> GetComponents();

	std::vector<std::unique_ptr<Component>>* GetComponents() { return &m_components; }

	Transform* GetTransform() const { return m_transform.get(); }
	void MakeRectTransform();

	void UpdateComponents();
	void LateUpdateComponents();

	bool IsActive();
	void SetActive(bool active) { m_active = active; }

	std::string GetName() const { return m_name; }

	void CollisionStay(const PhysicsComponent*) const;

	virtual void Save(std::map<std::string, std::string>& data) override;
	virtual void Load(std::map<std::string, std::string>& data) override;

private:
	void SetTransform(Transform* transform) { m_transform.reset(transform); transform->m_gameObject = this; }
	void AddPrebuiltComponent(Component* component) { m_components.push_back(std::unique_ptr<Component>(component)); }

private:
	std::unique_ptr<Transform> m_transform;
	std::vector<std::unique_ptr<Component>> m_components;

	std::string m_name;
	bool m_active;
};

template<typename T> T* GameObject::AddComponent()
{
	if (!std::is_base_of<Component, T>::value)
		return nullptr;

	m_components.push_back(std::make_unique<T>(this));
	return dynamic_cast<T*>(m_components[m_components.size() - 1].get());
}

template<typename T> T* GameObject::GetComponent()
{
	if (!std::is_base_of<Component, T>::value)
		return nullptr;

	T* result;
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		result = dynamic_cast<T*>(m_components[i].get());
		if (result != NULL)
			return result;
	}
	return nullptr;
}

template<typename T> std::vector<T*> GameObject::GetComponents()
{
	std::vector<T*> result;
	if (!std::is_base_of<Component, T>::value)
		return result;

	T* temp;
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		temp = dynamic_cast<T*>(m_components[i].get());
		if (temp != NULL)
			result.push_back(temp);
	}
	return result;
}