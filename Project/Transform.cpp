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
	Matrix r = Matrix::Identity;
	r *= Matrix::CreateScale(m_scale);
	r *= Matrix::CreateFromQuaternion(m_rotation);
	r *= Matrix::CreateTranslation(m_position);
	return r;
}