#include "pch.h"
#include "Transform.h"
#include "GameObject.h"
#include "PhysicsComponent.h"

using namespace std;
using namespace DirectX::SimpleMath;

Transform::Transform(GameObject* gameObject)
{
	m_gameObject = gameObject;

	m_position = Vector3::Zero;
	m_rotation = Quaternion::Identity;
	m_scale = 1.0;

	m_parent = 0;
}


Transform::~Transform()
{
}

Matrix Transform::GetWorldMatrix()
{
	Matrix r = Matrix::Identity;
	
	if (m_parent != nullptr)
	{
		r *= m_parent->GetWorldMatrix();
	}

	Matrix local = Matrix::Identity;
	r *= Matrix::CreateScale(m_scale);
	r *= Matrix::CreateFromQuaternion(m_rotation);
	r *= Matrix::CreateTranslation(m_position);
	return r * local;
}

Matrix Transform::GetWorldToLocalMatrix()
{
	if (m_parent == nullptr)
		return Matrix::Identity;
	else
		return m_parent->GetWorldMatrix().Invert();
}

Matrix Transform::GetLocalToWorldMatrix()
{
	if (m_parent == nullptr)
		return Matrix::Identity;
	else
		return m_parent->GetWorldMatrix();
}

Vector3 Transform::GetForward()
{
	//return m_rotation * DirectX::SimpleMath::Vector3::UnitZ;
	Vector3 r;
	float x2 = m_rotation.x * 2.0f;
	float y2 = m_rotation.y * 2.0f;
	float z2 = m_rotation.z * 2.0f;
	float xx2 = m_rotation.x * x2;
	float yy2 = m_rotation.y * y2;
	float xz2 = m_rotation.x * z2;
	float yz2 = m_rotation.y * z2;
	float wx2 = m_rotation.w * x2;
	float wy2 = m_rotation.w * y2;
	r.x = (xz2 + wy2);
	r.y = (yz2 - wx2);
	r.z = (1.0f - (xx2 + yy2));
	return r;
}

void Transform::SetParent(Transform* parent)
{
	if (m_parent != nullptr)
	{
		m_parent->RemoveChild(this);
	}

	m_parent = parent;
	m_parent->AddChild(this);

	if (!parent->IsActive() && Object::IsSelfActive())
		m_gameObject->OnDisable();
}

void Transform::RemoveChild(Transform* child)
{
	for (auto itr = m_childern.begin(); itr != m_childern.end(); ++itr)
	{
		if ((*itr) == child)
		{
			m_childern.erase(itr);
			break;
		}
	}
}

void Transform::AddChild(Transform* child)
{
	m_childern.push_back(child);
}

void Transform::Translate(Vector3 translate)
{
	m_position += (Matrix::CreateTranslation(translate) * GetWorldToLocalMatrix()).Translation();
	UpdateCollider();
}

void Transform::SetPosition(Vector3 position)
{
	m_position = (Matrix::CreateTranslation(position) * GetWorldToLocalMatrix()).Translation();
	UpdateCollider();
}

Vector3 Transform::GetPosition()
{
	return (Matrix::CreateTranslation(m_position) * GetLocalToWorldMatrix()).Translation();
}

// Not Tested
void Transform::SetRotation(Quaternion rotation)
{
	Matrix tempMatrix;
	Vector3 tempPosition;
	Vector3 tempScale;
	Quaternion tempRotation;
	tempMatrix = (Matrix::CreateFromQuaternion(rotation) * GetWorldToLocalMatrix());
	tempMatrix.Decompose(tempScale, tempRotation, tempPosition);
	m_rotation = tempRotation;

	UpdateCollider();
}

// Not Tested
Quaternion Transform::GetRotation()
{
	Matrix tempMatrix;
	Vector3 tempPosition;
	Vector3 tempScale;
	Quaternion tempRotation;
	tempMatrix = (Matrix::CreateFromQuaternion(m_rotation) * GetLocalToWorldMatrix());
	tempMatrix.Decompose(tempScale, tempRotation, tempPosition);
	return tempRotation;
}

void Transform::UpdateCollider()
{
	for (unsigned int i = 0; i < m_childern.size(); i++)
	{
		m_childern[i]->UpdateCollider();
	}
	auto colliders = m_gameObject->GetComponents<PhysicsComponent>();
	for (unsigned int i = 0; i < colliders.size(); i++) {
		colliders[i]->SetWorldTransform(GetPosition(), GetRotation());
	}
}

void Transform::Save(std::map<std::string, std::string>& data)
{
	Object::Save(data);

	data.insert(std::pair<std::string, std::string>("Position", to_string(m_position)));
	data.insert(std::pair<std::string, std::string>("Rotation", to_string(m_rotation)));
	data.insert(std::pair<std::string, std::string>("Scale", std::to_string(m_scale)));

	data.insert(std::pair<std::string, std::string>("Parent", std::to_string((int)m_parent)));
}

void Transform::Load(std::map<std::string, std::string>& data)
{
	Object::Load(data);

	m_position = stov3(data["Position"]);
	m_rotation = stoq(data["Rotation"]);
	m_scale = stof(data["Scale"]);
}

void Transform::OnDisable()
{
	for (unsigned int i = 0; i < m_childern.size(); i++)
	{
		m_childern[i]->GetGameObject()->OnDisable();
	}
}

void Transform::OnEnable()
{
	for (unsigned int i = 0; i < m_childern.size(); i++)
	{
		m_childern[i]->GetGameObject()->OnEnable();
	}
}

bool Transform::IsActive()
{
	return m_gameObject->IsActive();
}

bool Transform::IsSelfActive()
{
	return m_gameObject->IsSelfActive();
}