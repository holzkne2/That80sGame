#include "pch.h"
#include "Camera.h"
#include "Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera(GameObject* gameObject) : Component(gameObject)
{
	m_zFar = 1000;
	m_zNear = 0.1;
	m_fov = XMConvertToRadians(60);

	Game::Get()->GetScene()->AddCamera(this);
}


Camera::~Camera()
{
}

Matrix Camera::GetProjectionMatrix(const RECT& screen)
{
	return Matrix::CreatePerspectiveFieldOfView(m_fov,
		float(screen.right) / float(screen.bottom),
		m_zNear, m_zFar);
}

Matrix Camera::GetViewMatrix()
{
	//return  Matrix::CreateLookAt(Vector3(-3.3, 0, 0), Vector3(-2.31, 0, 4.9), Vector3::UnitY);
	return m_gameObject->GetTransform()->GetWorldMatrix().Invert();
}
