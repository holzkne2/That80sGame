#pragma once
#include "Component.h"
#include "btBulletDynamicsCommon.h"

class Collider :
	public Component
{
public:
	Collider(GameObject*);
	virtual ~Collider();

	btCollisionShape* GetCollisionShape() { return m_collisionShape.get(); }

protected:
	std::unique_ptr<btCollisionShape> m_collisionShape;
};

