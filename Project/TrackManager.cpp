#include "pch.h"
#include "TrackManager.h"


TrackManager::TrackManager(GameObject* gameObject) : Component(gameObject)
{
	m_speed = 10;
}


TrackManager::~TrackManager()
{
}

void TrackManager::Update()
{
	//position += (forwardDir * m_forwardSpeed * deltaTime);
}