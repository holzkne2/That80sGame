#include "pch.h"
#include "ShipController.h"
#include "GameObject.h"
#include "Game.h"
#include "PhysicsComponent.h"
#include "TrackManager.h"
#include "TimerUI.h"

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
	m_gameObject->GetComponent<PhysicsComponent>()->GetRigidBody()->setAngularFactor(btVector3(0, 0, 0));
	m_gameObject->GetComponent<PhysicsComponent>()->GetRigidBody()->setLinearFactor(btVector3(1, 1, 1));

	Vector3 v = Vector3::Zero;
	float deltaTime = Game::Get()->GetTimer()->GetElapsedSeconds();
	auto state = GamePad::Get().GetState(0, GamePad::DEAD_ZONE_CIRCULAR);

	if (state.IsConnected())
	{
		Vector3 offset = Vector3(0 - state.thumbSticks.leftX, state.thumbSticks.leftY, 0);
		v = (offset * deltaTime * m_slideSpeed);
	}
	
	m_gameObject->GetComponent<PhysicsComponent>()->SetVelocity(v);
}
void ShipController::CollisionStay(const PhysicsComponent* other)
{
	if (other->GetGameObject()->GetTag() == "Obsticle")
	{
		m_gameOverUI->SetActive(true);
		Game::Get()->GetScene()->GetGameObject("Track Manager")->GetComponent<TrackManager>()->SetSpeed(0);
		Game::Get()->GetScene()->GetGameObject("Timer")->GetComponent<TimerUI>()->SetActive(false);
		m_slideSpeed = 0;
	}
}

void ShipController::Save(std::map<std::string, std::string>& data)
{
	Component::Save(data);

	data.insert(std::pair<std::string, std::string>("Slide Speed", std::to_string(m_slideSpeed)));
	data.insert(std::pair<std::string, std::string>("Game Over", std::to_string((int)m_gameOverUI)));
}

void ShipController::Load(std::map<std::string, std::string>& data)
{
	Component::Load(data);

	m_slideSpeed = std::stof(data["Slide Speed"]);
}