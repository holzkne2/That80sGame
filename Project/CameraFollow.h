#pragma once
#include "Component.h"
#include "Transform.h"

class CameraFollow :
	public Component
{
public:
	CameraFollow(GameObject*);
	virtual ~CameraFollow();

	void LateUpdate() override;

	void SetTarget(Transform*);

	virtual void Save(std::map<std::string, std::string>& data) override;

private:
	Transform* m_targetTransform;
	DirectX::SimpleMath::Vector3 m_offset;
};

