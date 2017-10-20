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

Vector3 Transform::GetForward()
{
	//return m_rotation * DirectX::SimpleMath::Vector3::UnitZ;
	Vector3 r;
	float x2 = m_rotation.x * 2.0;
	float y2 = m_rotation.y * 2.0;
	float z2 = m_rotation.z * 2.0;
	float xx2 = m_rotation.x * x2;
	float yy2 = m_rotation.y * y2;
	float xz2 = m_rotation.x * z2;
	float yz2 = m_rotation.y * z2;
	float wx2 = m_rotation.w * x2;
	float wy2 = m_rotation.w * y2;
	r.x = (xz2 + wy2);
	r.y = (yz2 - wx2);
	r.z = (1.0 - (xx2 + yy2));
	return r;
}