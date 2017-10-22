#include "pch.h"
#include "TrackManager.h"
#include "GameObject.h"
#include "Game.h"


TrackManager::TrackManager(GameObject* gameObject) : Component(gameObject)
{
	m_speed = 10;
	m_liveThreshold = 1;
}


TrackManager::~TrackManager()
{
}

void TrackManager::Update()
{
	// Spawn Objects
	GameObject* gameObject;
	if (m_deadObjects.size() > 0 && m_liveObjects.size() < m_liveThreshold)
	{
		// TODO: Randomize
		gameObject = m_deadObjects[m_deadObjects.size() - 1];
		m_deadObjects.pop_back();
		m_liveObjects.push_back(gameObject);
		gameObject->SetActive(true);
	}

	float deltaTime = Game::Get()->GetTimer()->GetElapsedSeconds();

	// Move Objects
	Transform* transform;
	for (unsigned int i = 0; i < m_liveObjects.size(); i++)
	{
		transform = m_liveObjects[i]->GetTransform();
		transform->Translate(-transform->GetForward() * m_speed * deltaTime);
	}

	// Remove Objects
}

void TrackManager::AddObject(GameObject* gameObject)
{
	gameObject->SetActive(false);
	m_deadObjects.push_back(gameObject);
}