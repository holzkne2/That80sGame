#pragma once
#include <vector>
#include "btBulletDynamicsCommon.h"

class DebugDraw :
	public btIDebugDraw
{
public:
	class Line
	{
	public:
		Line(DirectX::SimpleMath::Vector3 from,
			DirectX::SimpleMath::Vector3 to,
			DirectX::SimpleMath::Color color)
		{
			m_from = from; m_to = to; m_color = color;
		}
		~Line() {}

		void GetAsVertexPositionColors(DirectX::VertexPositionColor& from, DirectX::VertexPositionColor& to)
		{
			from = DirectX::VertexPositionColor(m_from, m_color);
			to = DirectX::VertexPositionColor(m_to, m_color);
		}

	private:
		DirectX::SimpleMath::Vector3 m_from;
		DirectX::SimpleMath::Vector3 m_to;
		DirectX::SimpleMath::Color m_color;
	};

public:
	DebugDraw();
	virtual ~DebugDraw();

	virtual void drawLine(const btVector3&, const btVector3&, const btVector3&);
	virtual void clearLines() override { m_lines.clear(); }

	virtual void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) {}
	virtual void setDebugMode(int) {}
	virtual int getDebugMode() const { return DebugDrawModes::DBG_DrawWireframe; }
	virtual void reportErrorWarning(const char *) {}
	virtual void draw3dText(const btVector3 &, const char *) {}

	std::vector<Line>* GetLines() { return &m_lines; }

private:
	std::vector<Line> m_lines;
};

