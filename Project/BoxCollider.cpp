#include "pch.h"
#include "BoxCollider.h"
#include "btBulletDynamicsCommon.h"

using namespace DirectX::SimpleMath;

BoxCollider::BoxCollider(GameObject* gameObject) : Collider(gameObject)
{
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::Init(Vector3 halfExtern)
{
	m_collisionShape = std::make_unique<btBoxShape>
		(btVector3(halfExtern.x, halfExtern.y, halfExtern.z));
}
