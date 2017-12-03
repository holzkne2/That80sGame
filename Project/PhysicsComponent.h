#pragma once
#include "Component.h"
#include "MyMotionState.h"

#define BIT(x) (1<<(x))
enum collisiontypes {
	COL_NOTHING = 0, //<Collide with nothing
	COL_SHIP = BIT(0), //<Collide with ships
	COL_CONTRAINT = BIT(1), //<Collide with contraints
	COL_OBSTICLE = BIT(2), //<Collide with walls
	COL_POWERUP = BIT(3) //<Collide with powerups
};

class PhysicsComponent :
	public Component
{
public:
	PhysicsComponent(GameObject*);
	virtual ~PhysicsComponent();

	void SetMass(float mass) { m_mass = mass; }
	void SetKinematic(bool kinematic) { m_kinematic = kinematic; }
	void SetGroup(int group) { m_group = group; }
	void SetMask(int mask) { m_mask = mask; }

	void SetVelocity(DirectX::SimpleMath::Vector3);

	void SetWorldTransform(DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Quaternion);
	void GetWorldTransform(btTransform&);

	btRigidBody* GetRigidBody() { return m_rigidBody.get(); }
	btMotionState* GetMotionState() { return m_motionState.get(); }

	void AddBoxCollider(const DirectX::SimpleMath::Vector3&);
	void AddMeshCollider(const std::vector<DirectX::SimpleMath::Vector3>&);

	// Call after all colliders are in
	void init();

	virtual void Save(std::map<std::string, std::string>& data) override;

	void OnDisable() override;
	void OnEnable() override;

private:
	std::vector<std::unique_ptr<btBoxShape>> m_boxColliders;
	std::vector<std::unique_ptr<btConvexHullShape>> m_meshColliders;

	std::unique_ptr<btRigidBody> m_rigidBody;
	std::unique_ptr<MyMotionState> m_motionState;

	std::unique_ptr<btCompoundShape> m_compoundShape;

	float m_mass;
	bool m_kinematic;
	int m_group;
	int m_mask;
};

