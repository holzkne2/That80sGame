#pragma once
#include "Component.h"
class ShipController :
	public Component
{
public:
	ShipController(GameObject*);
	virtual ~ShipController();

	void Update() override;

	void CollisionStay(const Collider*) const override;

private:	
	float m_slideSpeed;

	DirectX::SimpleMath::Vector2 m_minPosition;
	DirectX::SimpleMath::Vector2 m_maxPosition;
};

