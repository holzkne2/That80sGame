#include "pch.h"
#include "RectTransform.h"

using namespace DirectX::SimpleMath;

RectTransform::RectTransform(GameObject* gameObject) : Transform(gameObject)
{
	m_anchors = Vector2(0.5, 0.5);
	m_pivot = Vector2::Zero;
}


RectTransform::~RectTransform()
{
}

RectTransform::RectTransform(Transform* base) : RectTransform(base->GetGameObject())
{
	m_position = base->GetPosition();
	m_rotation = base->GetRotation();
	m_scale = base->GetScale();
}

Vector2 RectTransform::GetScreenPosition(const RECT& screen)
{
	return Vector2(m_position.x + screen.right * m_anchors.x, m_position.y + screen.bottom * m_anchors.y);
}