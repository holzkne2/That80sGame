#pragma once
#include "Component.h"
class ShipController :
	public Component
{
public:
	ShipController(GameObject*);
	virtual ~ShipController();

	void Update() override;

private:
	float m_forwardSpeed;
};

