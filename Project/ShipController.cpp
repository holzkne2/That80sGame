#include "pch.h"
#include "ShipController.h"
#include "GameObject.h"
#include "Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

ShipController::ShipController(GameObject* gameObject) : Component(gameObject)
{
	m_forwardSpeed = 10;
	m_slideSpeed = 10;
}


ShipController::~ShipController()
{
}

void ShipController::Update()
{
	Vector3 forwardDir = m_gameObject->GetTransform()->GetForward();
	forwardDir.Normalize();

	float deltaTime = Game::Get()->GetTimer()->GetElapsedSeconds();

	m_gameObject->GetTransform()->Translate(forwardDir * m_forwardSpeed * deltaTime);


	auto state = GamePad::Get().GetState(0, GamePad::DEAD_ZONE_CIRCULAR);

	if (state.IsConnected())
	{
		Vector3 offset = Vector3(0 - state.thumbSticks.leftX, 0, 0);
		m_gameObject->GetTransform()->Translate(offset * deltaTime * m_slideSpeed);
	}
}