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

	virtual void Save(std::map<std::string, std::string>& data) override;

private:
	float m_zFar;
	float m_zNear;
	float m_fov;
};

