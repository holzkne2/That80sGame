#include "pch.h"
#include "ShipController.h"
#include "GameObject.h"
#include "Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

ShipController::ShipController(GameObject* gameObject) : Component(gameObject)
{
	m_slideSpeed = 10;

	m_minPosition = Vector2(-2.5, 1.75);
	m_maxPosition = Vector2(2.5, 4.75);
}


ShipController::~ShipController()
{
}

void ShipController::Update()
{
	Vector3 forwardDir = m_gameObject->GetTransform()->GetForward();
	forwardDir.Normalize();
	Vector3 position = m_gameObject->GetTransform()->GetPosition();

	float deltaTime = Game::Get()->GetTimer()->GetElapsedSeconds();



	auto state = GamePad::Get().GetState(0, GamePad::DEAD_ZONE_CIRCULAR);

	if (state.IsConnected())
	{
		Vector3 offset = Vector3(0 - state.thumbSticks.leftX, state.thumbSticks.leftY, 0);
		position += (offset * deltaTime * m_slideSpeed);
	}


	position.x = XMMin<float>(m_maxPosition.x, XMMax<float>(m_minPosition.x, position.x));
	position.y = XMMin<float>(m_maxPosition.y, XMMax<float>(m_minPosition.y, position.y));

	m_gameObject->GetTransform()->SetPosition(position);
}