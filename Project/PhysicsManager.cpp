#include "pch.h"
#include "PhysicsManager.h"
#include "GameObject.h"

#include <iostream>

using namespace DirectX::SimpleMath;
using namespace SimpleMath_LinearMath;

PhysicsManager::PhysicsManager()
{
}


PhysicsManager::~PhysicsManager()
{
	Clear();
}

void PhysicsManager::Clear()
{
	if (m_dynamicsWorld)
		for (unsigned int i = 0; i < m_colliders.size(); i++)
			m_dynamicsWorld->removeRigidBody(m_colliders[i]->GetRigidBody());
	m_colliders.clear();
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
	m_dynamicsWorld->setGravity(btVector3(0, 0, 0));

	m_debugDraw = std::make_unique<DebugDraw>();
	m_dynamicsWorld->setDebugDrawer(m_debugDraw.get());
}

void PhysicsManager::Tick(float deltaTime)
{
	btTransform transform;
	PhysicsComponent* collider;

	m_dynamicsWorld->stepSimulation(deltaTime, 5);

	for (unsigned int i = 0; i < m_colliders.size(); i++)
	{
		collider = m_colliders[i];
		collider->GetWorldTransform(transform);
		collider->GetGameObject()->GetTransform()->SetPosition(btV3_smV3(transform.getOrigin()));
		collider->GetGameObject()->GetTransform()->SetRotation(btQ_smQ(transform.getRotation()));
	}

	int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
	for (unsigned int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		if (contactManifold->getNumContacts() > 0) {
			const btCollisionShape* csA = contactManifold->getBody0()->getCollisionShape();
			const btCollisionShape* csB = contactManifold->getBody1()->getCollisionShape();
			const PhysicsComponent* cA = static_cast<PhysicsComponent*>(csA->getUserPointer());
			const PhysicsComponent* cB = static_cast<PhysicsComponent*>(csB->getUserPointer());
			cA->GetGameObject()->CollisionStay(cB);
			cB->GetGameObject()->CollisionStay(cA);
		}
	}

	m_dynamicsWorld->debugDrawWorld();
}

void PhysicsManager::AddCollider(PhysicsComponent* collider, int group, int mask)
{
	m_dynamicsWorld->addRigidBody(collider->GetRigidBody(), group, mask);
	m_colliders.push_back(collider);
}