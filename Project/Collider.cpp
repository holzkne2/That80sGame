#include "pch.h"
#include "Collider.h"
#include "GameObject.h"
#include "Game.h"

using namespace DirectX::SimpleMath;
using namespace SimpleMath_LinearMath;

Collider::Collider(GameObject* gameObject) : Component(gameObject)
{
}


Collider::~Collider()
{
}

void Collider::GetWorldTransform(btTransform& transform)
{
	return m_rigidBody->getMotionState()->getWorldTransform(transform);
}

void Collider::Init(float mass)
{
	m_collisionShape->setLocalScaling(m_gameObject->GetTransform()->GetScale() * btVector3(1, 1, 1));

	m_motionState = std::make_unique<btDefaultMotionState>(btTransform(
		smQ_btQ(m_gameObject->GetTransform()->GetRotation()),
		smV3_btV3(m_gameObject->GetTransform()->GetPosition())
		));

	btScalar btmass = mass;
	btVector3 inertia(0, 0, 0);
	m_collisionShape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo
		rigidBodyCI(btmass, m_motionState.get(), m_collisionShape.get(), inertia);
	m_rigidBody = std::make_unique<btRigidBody>(rigidBodyCI);

	Game::Get()->GetPhysicsManager()->AddCollider(this);
}