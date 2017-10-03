#include "pch.h"
#include "RectTransform.h"

using namespace DirectX::SimpleMath;

RectTransform::RectTransform()
{
	m_position = Vector3::Zero;
	m_rotation = Quaternion::Identity;
	m_scale = 1.0;

	m_anchors = Vector2(0.5, 0.5);
	m_pivot = Vector2::Zero;
}


RectTransform::~RectTransform()
{
}

Vector2 RectTransform::GetScreenPosition(const RECT& screen)
{
	return Vector2(m_position.x + screen.right * m_anchors.x, m_position.y + screen.bottom * m_anchors.y);
}