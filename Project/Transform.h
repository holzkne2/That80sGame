#pragma once

#include "pch.h"
#include <vector>
#include "Object.h"

class GameObject;

class Transform : Object
{
public:
	Transform(GameObject*);
	virtual ~Transform();

	void Translate(DirectX::SimpleMath::Vector3);

	void TranslateLocal(DirectX::SimpleMath::Vector3 translation) { m_position += translation; }

	void SetPosition(DirectX::SimpleMath::Vector3);
	DirectX::SimpleMath::Vector3 GetPosition();

	void SetLocalPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }
	DirectX::SimpleMath::Vector3 GetLocalPosition() { return m_position; }

	void SetRotation(DirectX::SimpleMath::Quaternion);
	DirectX::SimpleMath::Quaternion GetRotation();

	void SetLocalRotation(DirectX::SimpleMath::Quaternion rotation) { m_rotation = rotation; }
	DirectX::SimpleMath::Quaternion GetLocalRotation() { return m_rotation; }

	void SetScale(float scale) { m_scale = scale; }
	float GetScale() { return m_scale; }

	// TODO: lossy scale

	DirectX::SimpleMath::Vector3 GetForward();

	DirectX::SimpleMath::Matrix GetWorldMatrix();
	DirectX::SimpleMath::Matrix GetWorldToLocalMatrix();
	DirectX::SimpleMath::Matrix GetLocalToWorldMatrix();

	// TODO: const
	GameObject* GetGameObject() { return m_gameObject; }

	Transform* GetParent() { return m_parent; }
	void SetParent(Transform*);

	int ChildCount() { return m_childern.size(); }
	Transform* GetChild(int index) { return m_childern[index]; }

	virtual void Save(std::map<std::string, std::string>& data) override;

private:
	void RemoveChild(Transform*);
	void AddChild(Transform*);

	void UpdateCollider();

protected:
	GameObject* m_gameObject;

	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Quaternion m_rotation;
	float m_scale;

private:
	Transform* m_parent;
	std::vector<Transform*> m_childern;
};

