#pragma once

#include "pch.h"

class Transform
{
public:
	Transform();
	virtual ~Transform();

	void Translate(DirectX::SimpleMath::Vector3 translation) { m_position += translation; }

	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	void SetRotation(DirectX::SimpleMath::Quaternion rotation) { m_rotation = rotation; }
	DirectX::SimpleMath::Quaternion GetRotation() { return m_rotation; }

	void SetScale(float scale) { m_scale = scale; }
	float GetScale() { return m_scale; }

	DirectX::SimpleMath::Vector3 GetForward();

	DirectX::SimpleMath::Matrix GetWorldMatrix();

protected:
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Quaternion m_rotation;
	float m_scale;
};

