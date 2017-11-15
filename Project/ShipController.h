#pragma once
#include "Component.h"
class ShipController :
	public Component
{
public:
	ShipController(GameObject*);
	virtual ~ShipController();

	void Update() override;

	void CollisionStay(const Collider*) const override;

	void SetGameOverUI(GameObject* gameoverUI) { m_gameOverUI = gameoverUI; }

	virtual void Save(std::map<std::string, std::string>& data) override;
	virtual void Load(std::map<std::string, std::string>& data) override;

private:	
	float m_slideSpeed;
	GameObject* m_gameOverUI;
};

