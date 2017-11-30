#pragma once
#include "Collider.h"
class MeshCollider :
	public Collider
{
public:
	MeshCollider(GameObject*);
	virtual ~MeshCollider();

	void Init(const std::vector<DirectX::SimpleMath::Vector3>&, float, bool, int, int);

	virtual void Save(std::map<std::string, std::string>& data) override;
};

