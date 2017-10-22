#pragma once
#include "Component.h"
#include <vector>

class TrackManager :
	public Component
{
public:
	TrackManager(GameObject*);
	virtual ~TrackManager();

	void Update() override;

private:
	float m_speed;

	std::vector<GameObject*> m_deadObjects;
	std::vector<GameObject*> m_liveObjects;
};

