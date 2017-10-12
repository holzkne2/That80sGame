#pragma once
#include "pch.h"
#include "Component.h"

class Camera :
	public Component
{
public:
	Camera(GameObject*);
	virtual ~Camera();

	DirectX::SimpleMath::Matrix GetProjectionMatrix(const RECT&);
	DirectX::SimpleMath::Matrix GetViewMatrix();

private:
	float m_zFar;
	float m_zNear;
	float m_fov;
};

