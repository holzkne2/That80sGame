#pragma once

#include <vector>
#include "btBulletDynamicsCommon.h"
#include "Collider.h"
#include "DebugDraw.h"

class PhysicsManager
{
public:
	PhysicsManager();
	virtual ~PhysicsManager();

	void Initialize();
	void Clear();

	void Tick(float);

	void AddCollider(Collider*);

	DebugDraw* GetDebugDraw() { return m_debugDraw.get(); }

private:
	std::unique_ptr<btBroadphaseInterface> m_broadphase;
	std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
	std::unique_ptr<btCollisionDispatcher> m_dispatcher;
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
	std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;

	std::vector<Collider*> m_colliders;

	std::unique_ptr<DebugDraw> m_debugDraw;
};

