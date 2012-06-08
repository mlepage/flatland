// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "brush.h"


/*******************************************************************************
*******************************************************************************/
void
Brush::addVertex(
	const Vec2& kvVertex)
{
	// Set bounds.
	if (m_cvVertex.empty())
	{
		m_rBounds = Rect(kvVertex, kvVertex);
	}
	else
	{
		Geometry::addPointToBounds(
			kvVertex,
			m_rBounds);
	}

	// Add vertex and edge line.
	m_cvVertex.push_back(kvVertex);
	m_cvLine.push_back(Vec3());

	// Set edge lines.
	int n1;
	int n2;

	for (n1 = m_cvVertex.size() - 1, n2 = 0;
		n2 != m_cvVertex.size();
		n1 = n2, ++n2)
	{
		const Vec2 kvEdgeDirection = getVertex(n2) - getVertex(n1);
		const Vec2 kvEdgeNormal = Vec2(kvEdgeDirection[1], -kvEdgeDirection[0]);
		const scalar kfEdgeMagnitude = sqrt(kvEdgeDirection % kvEdgeDirection);
		const Vec2 kvNormalizedEdgeNormal = kvEdgeNormal / kfEdgeMagnitude;
		const scalar kfNormalizedEdgeDistance =
			getVertex(n1) % -kvNormalizedEdgeNormal;
		m_cvLine[n1] =
			Vec3(
				kvNormalizedEdgeNormal[0],
				kvNormalizedEdgeNormal[1],
				kfNormalizedEdgeDistance);
	}
}
