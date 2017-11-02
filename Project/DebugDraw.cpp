#include "pch.h"
#include "DebugDraw.h"

using namespace DirectX::SimpleMath;
using namespace SimpleMath_LinearMath;

DebugDraw::DebugDraw()
{
}


DebugDraw::~DebugDraw()
{
}

void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	m_lines.push_back(Line(btV3_smV3(from), btV3_smV3(to), btV3_smC(color)));
}
