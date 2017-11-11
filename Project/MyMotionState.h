#pragma once
#include "btBulletDynamicsCommon.h"
#include "pch.h"

class MyMotionState :
	public btMotionState
{
public:
	MyMotionState(const btTransform &initialpos) { m_position = initialpos; }
	virtual ~MyMotionState() { }

	virtual void getWorldTransform(btTransform &worldTrans) const { worldTrans = m_position; }
	void SetKinematicPos(btTransform &currentPos) { m_position = currentPos; }
	virtual void setWorldTransform(const btTransform &worldTrans) {	m_position = worldTrans; }

protected:
	btTransform m_position;
};

