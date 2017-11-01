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
	btRigidBody* GetRigidBody() { return m_rigidBody.get(); }
	btDefaultMotionState* GetMotionState() { return m_motionState.get(); }

	void GetWorldTransform(btTransform&);

protected:
	void Init(float);

protected:
	std::unique_ptr<btCollisionShape> m_collisionShape;
	std::unique_ptr<btRigidBody> m_rigidBody;
	std::unique_ptr<btDefaultMotionState> m_motionState;
};

