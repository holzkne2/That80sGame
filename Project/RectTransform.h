#pragma once

#include "pch.h"
#include "Transform.h"

class RectTransform : public Transform
{
public:
	RectTransform(GameObject*);
	virtual ~RectTransform();

	RectTransform(Transform*);

	DirectX::SimpleMath::Vector2 GetScreenPosition(const RECT&);

	// TODO: parenting

	void SetAnchors(DirectX::SimpleMath::Vector2 anchors) { m_anchors = anchors; }
	DirectX::SimpleMath::Vector2 GetAnchors() { return m_anchors; }

	void SetPivot(DirectX::SimpleMath::Vector2 pivot) { m_pivot = pivot; }
	DirectX::SimpleMath::Vector2 GetPivot() { return m_pivot; }

private:
	DirectX::SimpleMath::Vector2 m_anchors;
	DirectX::SimpleMath::Vector2 m_pivot;
};

