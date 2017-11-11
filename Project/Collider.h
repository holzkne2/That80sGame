#pragma once
#include "Component.h"
#include "btBulletDynamicsCommon.h"
#include "MyMotionState.h"

class Collider :
	public Component
{
public:
	Collider(GameObject*);
	virtual ~Collider();

	btCollisionShape* GetCollisionShape() { return m_collisionShape.get(); }
	btRigidBody* GetRigidBody() { return m_rigidBody.get(); }
	btMotionState* GetMotionState() { return m_motionState.get(); }

	void SetWorldTransform(DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Quaternion);
	void GetWorldTransform(btTransform&);

	void SetVelocity(DirectX::SimpleMath::Vector3);

protected:
	void Init(float, bool);

protected:
	std::unique_ptr<btCollisionShape> m_collisionShape;
	std::unique_ptr<btRigidBody> m_rigidBody;
	std::unique_ptr<MyMotionState> m_motionState;
};

