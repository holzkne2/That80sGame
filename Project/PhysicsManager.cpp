#include "pch.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include "BoxCollider.h"

using namespace DirectX::SimpleMath;

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
	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

	// Ex
	GameObject* groundObj = new GameObject("Ground Object");
	groundObj->AddComponent<BoxCollider>()->Init(Vector3(5, 0.5, 5));
	groundObj->GetTransform()->SetPosition(Vector3(0, 0, 0));
	
	GameObject* fallObj = new GameObject("Fall Object");
	fallObj->AddComponent<BoxCollider>()->Init(Vector3(0.5, 0.5, 0.5));
	fallObj->GetTransform()->SetPosition(Vector3(0, 50, 0));
	
	std::unique_ptr<btDefaultMotionState> groundMotionState =
		std::make_unique<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState.get(), groundObj->GetComponent<BoxCollider>()->GetCollisionShape(), btVector3(0, 0, 0));
	std::unique_ptr<btRigidBody> groundRigidBody = std::make_unique<btRigidBody>(groundRigidBodyCI);
	m_dynamicsWorld->addRigidBody(groundRigidBody.get());

	
	std::unique_ptr<btDefaultMotionState> fallMotionState =
		std::make_unique<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	fallObj->GetComponent<BoxCollider>()->GetCollisionShape()->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo
		fallRigidBodyCI(mass, fallMotionState.get(), fallObj->GetComponent<BoxCollider>()->GetCollisionShape(), fallInertia);
	std::unique_ptr<btRigidBody> fallRigidBody = std::make_unique<btRigidBody>(fallRigidBodyCI);
	m_dynamicsWorld->addRigidBody(fallRigidBody.get());

	for (int i = 0; i < 300; i++)
	{
		m_dynamicsWorld->stepSimulation(1 / 60.0, 10);

		btTransform trans;
		fallRigidBody->getMotionState()->getWorldTransform(trans);

		fallObj->GetTransform()->SetPosition(Vector3(
			trans.getOrigin().getX(),
			trans.getOrigin().getY(),
			trans.getOrigin().getZ()
			));

		fallObj->GetTransform()->SetRotation(Quaternion(
			trans.getRotation().getX(),
			trans.getRotation().getY(),
			trans.getRotation().getZ(),
			trans.getRotation().getW()
			));
	}

	m_dynamicsWorld->removeRigidBody(fallRigidBody.get());
	m_dynamicsWorld->removeRigidBody(groundRigidBody.get());

	delete fallObj;
	delete groundObj;
}