#include "pch.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;

Transform::Transform()
{
	m_position = Vector3::Zero;
	m_rotation = Quaternion::Identity;
	m_scale = 1.0;
}


Transform::~Transform()
{
}

Matrix Transform::GetWorldMatrix()
{
	return Matrix::CreateScale(m_scale) * Matrix::CreateFromQuaternion(m_rotation) * Matrix::CreateTranslation(m_position);
}