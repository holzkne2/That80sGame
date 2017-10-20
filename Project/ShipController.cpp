#include "pch.h"
#include "ShipController.h"
#include "GameObject.h"
#include "Game.h"

using namespace DirectX::SimpleMath;

ShipController::ShipController(GameObject* gameObject) : Component(gameObject)
{
	m_forwardSpeed = 1;
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
}