#include "pch.h"
#include "MeshCollider.h"

using namespace DirectX::SimpleMath;
using namespace SimpleMath_LinearMath;

MeshCollider::MeshCollider(GameObject* gameObject) : Collider(gameObject)
{
}


MeshCollider::~MeshCollider()
{
}

void MeshCollider::Init(const std::vector<Vector3>& points,
	float mass, bool kinematic, int group, int mask)
{
	m_collisionShape = std::make_unique<btConvexHullShape>();
	btConvexHullShape* hull = static_cast<btConvexHullShape*>(m_collisionShape.get());


	for (unsigned int i = 0; i < points.size(); ++i) {
		hull->addPoint(smV3_btV3(points[i]), false);
	}

	hull->initializePolyhedralFeatures(0.01);
	hull->optimizeConvexHull();

	Collider::Init(mass, kinematic, group, mask);
}

void MeshCollider::Save(std::map<std::string, std::string>& data)
{
	Collider::Save(data);

	btConvexHullShape* hull = static_cast<btConvexHullShape*>(m_collisionShape.get());
	std::string buffer = "";
	for (unsigned int i = 0; i < hull->getNumPoints(); ++i) {
		buffer += to_string(SimpleMath_LinearMath::btV3_smV3(hull->getPoints()[i]));
		if (i != hull->getNumPoints() - 1)
			buffer += ";";
	}

	data.insert(std::pair<std::string, std::string>("Points", buffer));
}