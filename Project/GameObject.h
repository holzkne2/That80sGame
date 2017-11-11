#pragma once

#include <vector>
#include <type_traits>

#include "pch.h"
#include "RectTransform.h"
#include "Component.h"

class GameObject
{
public:
	GameObject();
	GameObject(std::string);
	virtual ~GameObject();

	template<typename T> T* AddComponent();
	template<typename T> T* GetComponent();

	Transform* GetTransform() { return m_transform.get(); }
	void MakeRectTransform();

	void UpdateComponents();
	void LateUpdateComponents();

	bool IsActive();
	void SetActive(bool active) { m_active = active; }

	std::string GetName() const { return m_name; }

	void CollisionStay(const Collider*) const;

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

