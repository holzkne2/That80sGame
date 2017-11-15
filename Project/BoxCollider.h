#pragma once
#include "Collider.h"
class BoxCollider :
	public Collider
{
public:
	BoxCollider(GameObject*);
	virtual ~BoxCollider();

	void Init(DirectX::SimpleMath::Vector3, float, bool, int, int);

	virtual void Save(std::map<std::string, std::string>& data) override;

private:
	DirectX::SimpleMath::Vector3 m_halfExtern;
};

