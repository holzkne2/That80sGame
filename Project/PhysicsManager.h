#pragma once

#include <vector>
#include "btBulletDynamicsCommon.h"
#include "DebugDraw.h"
#include "PhysicsComponent.h"

class PhysicsManager
{
public:
	PhysicsManager();
	virtual ~PhysicsManager();

	void Initialize();
	void Clear();

	void Tick(float);

	void AddCollider(PhysicsComponent*, int, int);
	void RemoveCollider(PhysicsComponent*);

	DebugDraw* GetDebugDraw() { return m_debugDraw.get(); }

private:
	std::unique_ptr<btBroadphaseInterface> m_broadphase;
	std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
	std::unique_ptr<btCollisionDispatcher> m_dispatcher;
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
	std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;

	std::vector<PhysicsComponent*> m_colliders;

	std::unique_ptr<DebugDraw> m_debugDraw;
};

