#pragma once

#include "btBulletDynamicsCommon.h"

class PhysicsManager
{
public:
	PhysicsManager();
	virtual ~PhysicsManager();

	void Initialize();

private:
	std::unique_ptr<btBroadphaseInterface> m_broadphase;
	std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
	std::unique_ptr<btCollisionDispatcher> m_dispatcher;
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
	std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
};

