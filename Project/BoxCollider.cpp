#include "pch.h"
#include "BoxCollider.h"

using namespace DirectX::SimpleMath;
using namespace SimpleMath_LinearMath;

BoxCollider::BoxCollider(GameObject* gameObject) : Collider(gameObject)
{
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::Init(Vector3 halfExtern, float mass, bool kinematic, int group, int mask)
{
	m_collisionShape = std::make_unique<btBoxShape>(smV3_btV3(halfExtern));
	
	Collider::Init(mass, kinematic, group, mask);
}
