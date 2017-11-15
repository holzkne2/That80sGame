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

	void AddObject(GameObject*);

	virtual void Save(std::map<std::string, std::string>& data) override;

private:
	float m_speed;

	int m_liveThreshold;
	std::vector<GameObject*> m_deadObjects;
	std::vector<GameObject*> m_liveObjects;
};

