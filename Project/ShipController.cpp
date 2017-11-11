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

	m_minPosition = Vector2(-2.5, 1.75);
	m_maxPosition = Vector2(2.5, 4.75);
}


ShipController::~ShipController()
{
}

void ShipController::Update()
{
	Vector3 v = Vector3::Zero;
	float deltaTime = Game::Get()->GetTimer()->GetElapsedSeconds();
	auto state = GamePad::Get().GetState(0, GamePad::DEAD_ZONE_CIRCULAR);

	if (state.IsConnected())
	{
		Vector3 offset = Vector3(0 - state.thumbSticks.leftX, state.thumbSticks.leftY, 0);
		v = (offset * deltaTime * m_slideSpeed);

		//position += (offset * deltaTime * m_slideSpeed);
	}
	
	//Vector3 original_position = m_gameObject->GetTransform()->GetPosition();
	//Vector3 limited_position = Vector3(
	//	XMMin<float>(m_maxPosition.x, XMMax<float>(m_minPosition.x, predicted_position.x)),
	//	XMMin<float>(m_maxPosition.y, XMMax<float>(m_minPosition.y, predicted_position.y)),
	//	0);

	m_gameObject->GetComponent<Collider>()->SetVelocity(v);

	//m_gameObject->GetTransform()->SetPosition(position);
}