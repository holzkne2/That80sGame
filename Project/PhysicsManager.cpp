#include "pch.h"
#include "PhysicsManager.h"


PhysicsManager::PhysicsManager()
{
}


PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::Initialize()
{
	// Build the broadphase
	m_broadphase = std::make_unique<btDbvtBroadphase>();

	// Set up the collision configuration and dispatcher
	m_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
	m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get());

	// The actual physics solver
	m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();

	// The world.
	m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>
		(m_dispatcher.get(), m_broadphase.get(),
		m_solver.get(), m_collisionConfiguration.get());
}