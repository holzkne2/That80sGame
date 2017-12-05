#include "pch.h"
#include "TrackManager.h"
#include "GameObject.h"
#include "Game.h"

using namespace DirectX::SimpleMath;

TrackManager::TrackManager(GameObject* gameObject) : Component(gameObject)
{
	m_speed = 10;
	m_liveThreshold = 3;
}


TrackManager::~TrackManager()
{
}

void TrackManager::Update()
{
	const float GridLength = 250;
	const float HalfGridLength = GridLength / 2.0f;

	// Spawn Objects
	GameObject* gameObject;
	if (m_deadObjects.size() > 0 && m_liveObjects.size() < m_liveThreshold)
	{
		Vector3 place;
		if (m_liveObjects.size() == 0)
			place = Vector3(0, 0, HalfGridLength);
		else
		{
			place = m_liveObjects[m_liveObjects.size() - 1]->GetTransform()->GetPosition()
				+ Vector3(0, 0, GridLength);
		}

		// TODO: Randomize
		gameObject = m_deadObjects[m_deadObjects.size() - 1];
		m_deadObjects.pop_back();
		m_liveObjects.push_back(gameObject);
		gameObject->GetTransform()->SetPosition(place);
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
	if (m_liveObjects[0]->GetTransform()->GetPosition().z <= -HalfGridLength)
	{
		gameObject = m_liveObjects[0];
		m_liveObjects.erase(m_liveObjects.begin());
		m_deadObjects.push_back(gameObject);
		gameObject->SetActive(false);
	}
}

void TrackManager::AddObject(GameObject* gameObject)
{
	gameObject->SetActive(false);
	m_deadObjects.push_back(gameObject);
}

void TrackManager::Save(std::map<std::string, std::string>& data)
{
	Component::Save(data);

	data.insert(std::pair<std::string, std::string>("Speed", std::to_string(m_speed)));
	data.insert(std::pair<std::string, std::string>("Live Threshold", std::to_string(m_liveThreshold)));
	
	std::string buffer = "";
	for (unsigned int i = 0; i < m_deadObjects.size(); i++)
	{
		buffer += std::to_string((int)m_deadObjects[i]);
		if (i != m_deadObjects.size() - 1)
			buffer += ",";
	}
	data.insert(std::pair<std::string, std::string>("Dead Objects", buffer));

	buffer = "";
	for (unsigned int i = 0; i < m_liveObjects.size(); i++)
	{
		buffer += std::to_string((int)m_liveObjects[i]);
		if (i != m_liveObjects.size() - 1)
			buffer += ",";
	}
	data.insert(std::pair<std::string, std::string>("Live Objects", buffer));
}