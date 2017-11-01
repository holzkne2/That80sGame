#pragma once
#include "Collider.h"
class BoxCollider :
	public Collider
{
public:
	BoxCollider(GameObject*);
	virtual ~BoxCollider();

	void Init(DirectX::SimpleMath::Vector3);
};

