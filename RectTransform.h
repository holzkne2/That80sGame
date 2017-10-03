#pragma once

#include "pch.h"

class RectTransform
{
public:
	RectTransform();
	virtual ~RectTransform();

	DirectX::SimpleMath::Vector2 GetScreenPosition(const RECT&);

	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	void SetRotation(DirectX::SimpleMath::Quaternion rotation) { m_rotation = rotation; }
	DirectX::SimpleMath::Quaternion GetRotation() { return m_rotation; }

	void SetScale(float scale) { m_scale = scale; }
	float GetScale() { return m_scale; }

	void SetAnchors(DirectX::SimpleMath::Vector2 anchors) { m_anchors = anchors; }
	DirectX::SimpleMath::Vector2 GetAnchors() { return m_anchors; }

	void SetPivot(DirectX::SimpleMath::Vector2 pivot) { m_pivot = pivot; }
	DirectX::SimpleMath::Vector2 GetPivot() { return m_pivot; }

private:
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Quaternion m_rotation;
	float m_scale;

	DirectX::SimpleMath::Vector2 m_anchors;
	DirectX::SimpleMath::Vector2 m_pivot;
};

