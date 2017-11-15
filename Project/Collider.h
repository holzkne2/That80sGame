#pragma once
#include "Component.h"
#include "btBulletDynamicsCommon.h"
#include "MyMotionState.h"

#define BIT(x) (1<<(x))
enum collisiontypes {
	COL_NOTHING = 0, //<Collide with nothing
	COL_SHIP = BIT(0), //<Collide with ships
	COL_CONTRAINT = BIT(1), //<Collide with contraints
	COL_WALL = BIT(2), //<Collide with walls
	COL_POWERUP = BIT(3) //<Collide with powerups
};

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

	virtual void Save(std::map<std::string, std::string>& data) override;

protected:
	void Init(float, bool, int, int);

protected:
	std::unique_ptr<btCollisionShape> m_collisionShape;
	std::unique_ptr<btRigidBody> m_rigidBody;
	std::unique_ptr<MyMotionState> m_motionState;

private:
	float m_mass;
	bool m_kinematic;
	int m_group;
	int m_mask;
};

