#pragma once
#include <map>

class PhysicsComponent;

class Object
{
public:
	Object();
	virtual ~Object();

	virtual void Save(std::map<std::string, std::string>& data) {}
	virtual void Load(std::map<std::string, std::string>& data) {}

	virtual bool IsActive() { return m_active; }

	virtual bool IsSelfActive() { return m_active; }
	void SetActive(bool active);

	virtual void CollisionStay(const PhysicsComponent*) {}

	virtual void OnDisable() {};
	virtual void OnEnable() {};

protected:
	bool m_active;
};

