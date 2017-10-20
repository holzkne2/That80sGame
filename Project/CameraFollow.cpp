#include "pch.h"
#include "CameraFollow.h"
#include "GameObject.h"

CameraFollow::CameraFollow(GameObject* gameObject) : Component(gameObject)
{
}


CameraFollow::~CameraFollow()
{
}

void CameraFollow::LateUpdate()
{
	if (m_targetTransform == nullptr)
		return;

	m_gameObject->GetTransform()->SetPosition(m_targetTransform->GetPosition() + m_offset);
}

void CameraFollow::SetTarget(Transform* target)
{
	m_targetTransform = target;
	m_offset = m_gameObject->GetTransform()->GetPosition() - m_targetTransform->GetPosition();
}