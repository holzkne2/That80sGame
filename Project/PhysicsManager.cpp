#include "pch.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include "BoxCollider.h"

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
	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

	// Ex
	//GameObject* groundObj = new GameObject("Ground Object");
	//groundObj->GetTransform()->SetPosition(Vector3(0, 0, 0));
	//groundObj->AddComponent<BoxCollider>()->Init(Vector3(5, 0.5, 5), 0);
	//
	//GameObject* fallObj = new GameObject("Fall Object");
	//fallObj->GetTransform()->SetPosition(Vector3(0, 50, 0));
	//fallObj->AddComponent<BoxCollider>()->Init(Vector3(0.5, 0.5, 0.5), 1);
	//
	//AddCollider(groundObj->GetComponent<BoxCollider>());
	//AddCollider(fallObj->GetComponent<BoxCollider>());

	//for (int i = 0; i < 300; i++)
	//{
	//	m_dynamicsWorld->stepSimulation(1 / 60.0, 5);

	//	btTransform trans;
	//	fallObj->GetComponent<Collider>()->GetWorldTransform(trans);

	//	fallObj->GetTransform()->SetPosition(Vector3(
	//		trans.getOrigin().getX(),
	//		trans.getOrigin().getY(),
	//		trans.getOrigin().getZ()
	//		));

	//	fallObj->GetTransform()->SetRotation(Quaternion(
	//		trans.getRotation().getX(),
	//		trans.getRotation().getY(),
	//		trans.getRotation().getZ(),
	//		trans.getRotation().getW()
	//		));

	//}
	//
	//m_dynamicsWorld->getCollisionObjectArray().clear();
	////m_dynamicsWorld->removeRigidBody(fallRigidBody.get());
	////m_dynamicsWorld->removeRigidBody(groundRigidBody.get());

	//delete fallObj;
	//delete groundObj;
}

void PhysicsManager::Tick(float deltaTime)
{
	m_dynamicsWorld->stepSimulation(deltaTime, 5);

	btTransform transform;
	Collider* collider;
	for (unsigned int i = 0; i < m_colliders.size(); i++)
	{
		collider = m_colliders[i];
		collider->GetWorldTransform(transform);
		collider->GetGameObject()->GetTransform()->SetPosition(btV3_smV3(transform.getOrigin()));
		collider->GetGameObject()->GetTransform()->SetRotation(btQ_smQ(transform.getRotation()));
	}
}

void PhysicsManager::AddCollider(Collider* collider)
{
	m_dynamicsWorld->addRigidBody(collider->GetRigidBody());
	m_colliders.push_back(collider);
}