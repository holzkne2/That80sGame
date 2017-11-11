#include "pch.h"
#include "ShipController.h"
#include "GameObject.h"
#include "Game.h"
#include "BoxCollider.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace SimpleMath_LinearMath;

ShipController::ShipController(GameObject* gameObject) : Component(gameObject)
{
	m_slideSpeed = 500;
}


ShipController::~ShipController()
{
}

void ShipController::Update()
{
	//Stop Rotation
	m_gameObject->GetComponent<Collider>()->GetRigidBody()->setAngularFactor(btVector3(0, 0, 0));
	m_gameObject->GetComponent<Collider>()->GetRigidBody()->setLinearFactor(btVector3(1, 1, 1));

	Vector3 v = Vector3::Zero;
	float deltaTime = Game::Get()->GetTimer()->GetElapsedSeconds();
	auto state = GamePad::Get().GetState(0, GamePad::DEAD_ZONE_CIRCULAR);

	if (state.IsConnected())
	{
		Vector3 offset = Vector3(0 - state.thumbSticks.leftX, state.thumbSticks.leftY, 0);
		v = (offset * deltaTime * m_slideSpeed);
	}
	
	m_gameObject->GetComponent<Collider>()->SetVelocity(v);
}
void ShipController::CollisionStay(const Collider* other) const
{
	if (other->GetGameObject()->GetName() == "Tower Collider")
	{
		m_gameOverUI->SetActive(true);
	}
}