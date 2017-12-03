#include "pch.h"
#include "PhysicsComponent.h"
#include "GameObject.h"
#include "Game.h"

using namespace DirectX::SimpleMath;
using namespace SimpleMath_LinearMath;

PhysicsComponent::PhysicsComponent(GameObject* gameObject) : Component(gameObject)
{
	float m_mass = 1.0;
	bool m_kinematic = false;
	int m_group = 0;
	int m_mask = 0;

	if (m_gameObject != nullptr && !m_gameObject->IsActive())
		OnDisable();
	else
		OnEnable();
}


PhysicsComponent::~PhysicsComponent()
{
	m_meshColliders.clear();
	m_boxColliders.clear();
	
	if (m_compoundShape.get() != nullptr) {
		for (int i = m_compoundShape->getNumChildShapes() - 1; i >= 0; --i)
			m_compoundShape->removeChildShapeByIndex(i);
		m_compoundShape.release();
	}
	m_rigidBody.release();
	m_motionState.release();
}

void PhysicsComponent::SetVelocity(Vector3 velocity)
{
	m_rigidBody->setLinearVelocity(smV3_btV3(velocity));
}

void PhysicsComponent::SetWorldTransform(Vector3 position, Quaternion rotation = Quaternion::Identity)
{
	m_motionState->SetKinematicPos(
		btTransform(
		smQ_btQ(rotation), smV3_btV3(position)
		));
}

void PhysicsComponent::GetWorldTransform(btTransform& transform)
{
	return m_rigidBody->getMotionState()->getWorldTransform(transform);
}

void PhysicsComponent::AddBoxCollider(const Vector3& halfExtern)
{
	m_boxColliders.push_back(std::make_unique<btBoxShape>(smV3_btV3(halfExtern)));
	btBoxShape* box = m_boxColliders[m_boxColliders.size() - 1].get();
	box->setUserPointer(this);
}

void PhysicsComponent::AddMeshCollider(const std::vector<Vector3>& points)
{
	m_meshColliders.push_back(std::make_unique<btConvexHullShape>());

	btConvexHullShape* hull =  m_meshColliders[m_meshColliders.size() - 1].get();

	for (unsigned int i = 0; i < points.size(); ++i) {
		hull->addPoint(smV3_btV3(points[i]), false);
	}

	hull->initializePolyhedralFeatures(0.01);
	hull->optimizeConvexHull();
	hull->setUserPointer(this);
}

void PhysicsComponent::init()
{
	const int totalColliders = m_boxColliders.size() + m_meshColliders.size();
	if (totalColliders == 0)
		return;

	m_motionState = std::make_unique<MyMotionState>(btTransform(
		smQ_btQ(m_gameObject->GetTransform()->GetRotation()),
		smV3_btV3(m_gameObject->GetTransform()->GetPosition())
		));

	btScalar btmass = m_mass;
	btVector3 inertia(0, 0, 0);

	btCollisionShape* shape;
	if (totalColliders == 1) {
		if (m_boxColliders.size() == 1)
			shape = m_boxColliders[0].get();
		else
			shape = m_meshColliders[0].get();
	}
	else {
		m_compoundShape = std::make_unique<btCompoundShape>(true, totalColliders);
		shape = m_compoundShape.get();
		const btTransform t = btTransform(btQuaternion::getIdentity());
		for (unsigned int i = 0; i < m_boxColliders.size(); ++i)
			m_compoundShape->addChildShape(t, m_boxColliders[i].get());
		for (unsigned int i = 0; i < m_meshColliders.size(); ++i)
			m_compoundShape->addChildShape(t, m_meshColliders[i].get());
		m_compoundShape->createAabbTreeFromChildren();

	}

	shape->setLocalScaling(m_gameObject->GetTransform()->GetScale() * btVector3(1, 1, 1));
	shape->setMargin(0.01);
	shape->calculateLocalInertia(m_mass, inertia);
	shape->setUserPointer(this);

	btRigidBody::btRigidBodyConstructionInfo
		rigidBodyCI(btmass, m_motionState.get(), shape, inertia);
	m_rigidBody = std::make_unique<btRigidBody>(rigidBodyCI);

	if (m_kinematic || m_mass <= 0)
	{
		m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() |
			btCollisionObject::CF_KINEMATIC_OBJECT);
	}
	else
	{
		m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() |
			btCollisionObject::CF_CHARACTER_OBJECT);
	}
	m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
	m_rigidBody->setUserPointer(this);

	if (IsActive())
		Game::Get()->GetPhysicsManager()->AddCollider(this, m_group, m_mask);
}

void PhysicsComponent::Save(std::map<std::string, std::string>& data)
{
	Component::Save(data);

	data.insert(std::pair<std::string, std::string>("Mass", std::to_string(m_mass)));
	data.insert(std::pair<std::string, std::string>("Kinematic", m_kinematic ? "True" : "False"));
	data.insert(std::pair<std::string, std::string>("Group", std::to_string(m_group)));
	data.insert(std::pair<std::string, std::string>("Mask", std::to_string(m_mask)));

	std::string buffer = "";
	if (m_boxColliders.size() > 0)
	{
		for (unsigned int i = 0; i < m_boxColliders.size(); ++i) {
			buffer += to_string(btV3_smV3(m_boxColliders[i]->getHalfExtentsWithoutMargin()));
			if (i != m_boxColliders.size() - 1)
				buffer += '|';
		}
		data.insert(std::pair<std::string, std::string>("Boxes", buffer));
	}

	buffer = "";
	if (m_meshColliders.size() > 0)
	{
		for (unsigned int i = 0; i < m_meshColliders.size(); ++i) {
			for (unsigned int j = 0; j < m_meshColliders[i]->getNumPoints(); ++j) {
				buffer += to_string(btV3_smV3(m_meshColliders[i]->getPoints()[j]));
				if (j != m_meshColliders[i]->getNumPoints() - 1)
					buffer += ';';

			}
			if (i != m_meshColliders.size() - 1)
				buffer += '|';
		}
		data.insert(std::pair<std::string, std::string>("Meshes", buffer));
	}
}

void PhysicsComponent::OnDisable()
{
	if (m_rigidBody != nullptr)
		Game::Get()->GetPhysicsManager()->RemoveCollider(this);
}

void PhysicsComponent::OnEnable()
{
	if (m_rigidBody != nullptr)
		Game::Get()->GetPhysicsManager()->AddCollider(this, m_group, m_mask);
}