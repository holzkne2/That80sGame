#include "pch.h"
#include "Object.h"


Object::Object()
{
	m_active = true;
}


Object::~Object()
{
}

void Object::SetActive(bool active) 
{
	if (m_active == active)
		return;
	if (active)
	{
		m_active = true;
		OnEnable();
	}
	else
	{
		m_active = false;
		OnDisable();
	}
}