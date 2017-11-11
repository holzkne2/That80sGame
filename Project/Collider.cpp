#include "pch.h"
#include "Collider.h"
#include "GameObject.h"
#include "Game.h"
#include "MyMotionState.h"

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

void Collider::Init(float mass, bool kinematic)
{
	m_collisionShape->setLocalScaling(m_gameObject->GetTransform()->GetScale() * btVector3(1, 1, 1));

	m_motionState = std::make_unique<MyMotionState>(btTransform(
		smQ_btQ(m_gameObject->GetTransform()->GetRotation()),
		smV3_btV3(m_gameObject->GetTransform()->GetPosition())
		));

	btScalar btmass = mass;
	btVector3 inertia(0, 0, 0);
	m_collisionShape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo
		rigidBodyCI(btmass, m_motionState.get(), m_collisionShape.get(), inertia);
	m_rigidBody = std::make_unique<btRigidBody>(rigidBodyCI);

	if (kinematic || mass <= 0)
	{
		m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	}
	else
	{
		m_rigidBody->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	}
	m_rigidBody->setActivationState(DISABLE_DEACTIVATION);

	Game::Get()->GetPhysicsManager()->AddCollider(this);
}

void Collider::SetWorldTransform(Vector3 position, Quaternion rotation = Quaternion::Identity)
{
	m_motionState->SetKinematicPos(
		btTransform(
		smQ_btQ(rotation), smV3_btV3(position)
		));
}

void Collider::SetVelocity(Vector3 v)
{
	m_rigidBody->setLinearVelocity(smV3_btV3(v));
}