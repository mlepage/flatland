// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#include "geometry.h"

#include <cmath>
#include "brush.h"
#include "config.h"
#include "model.h"


/*******************************************************************************
*******************************************************************************/
bool
Geometry::isRectTouchingRect(
	const Rect& kr1,
	const Rect& kr2)
{
	// TODO is this correct? is it efficient?
	// Look up correct algorithm.

	return
		((kr1.getMax()[0] >= kr2.getMin()[0] &&
		kr1.getMin()[0] <= kr2.getMax()[0]) ||
		(kr2.getMax()[0] >= kr1.getMin()[0] &&
		kr2.getMin()[0] <= kr1.getMax()[0])) &&
		((kr1.getMax()[1] >= kr2.getMin()[1] &&
		kr1.getMin()[1] <= kr2.getMax()[1]) ||
		(kr2.getMax()[1] >= kr1.getMin()[1] &&
		kr2.getMin()[1] <= kr1.getMax()[1]));
}


/*******************************************************************************
	Basically, it projects the brush onto the normal, determines the minimum
	and maximum distance along that normal (i.e. extents), and records the
	vertices and edges that resulted in that extent.
*******************************************************************************/
Geometry::EffectiveRadius
Geometry::makeEffectiveRadius(
	const Brush& kBrush,
	const Vec2& kvNormal)
{
	EffectiveRadius effectiveRadius;

	scalar fDistance;

	// The index of the vertex with the minimum distance to the line.
	int nMinIndex = 0;
	effectiveRadius.m_bMinEdge = false;
	effectiveRadius.m_fMinDistance = kvNormal % kBrush.getVertex(0);

#ifdef FLATLAND_ENABLE_EFFECTIVE_RADIUS_MAX
	// The index of the vertex with the maximum distance to the line.
	int nMaxIndex = 0;
	effectiveRadius.m_bMaxEdge = false;
	effectiveRadius.m_fMaxDistance = effectiveRadius.m_fMinDistance;
#endif

	{
		// Process vertex 1 here.
		int nVertexIndex = 1;

		const Vec2& kvVertex = kBrush.getVertex(nVertexIndex);

		fDistance = kvNormal % kvVertex;

		if (fDistance <
			(effectiveRadius.m_fMinDistance - FLATLAND_TOL_EDGE))
		{
			// New minimum distance.
			nMinIndex = nVertexIndex;
			effectiveRadius.m_fMinDistance = fDistance;
		}
		else if (fDistance <
			(effectiveRadius.m_fMinDistance + FLATLAND_TOL_EDGE))
		{
			// Edge.
			effectiveRadius.m_bMinEdge = true;
		}

#ifdef FLATLAND_ENABLE_EFFECTIVE_RADIUS_MAX
		if ((effectiveRadius.m_fMaxDistance + FLATLAND_TOL_EDGE) <
			fDistance)
		{
			// New maximum distance.
			nMaxIndex = nVertexIndex;
			effectiveRadius.m_fMaxDistance = fDistance;
		}
		else if ((effectiveRadius.m_fMaxDistance - FLATLAND_TOL_EDGE) <
			fDistance)
		{
			// Edge.
			effectiveRadius.m_bMaxEdge = true;
		}
#endif
	}

	for (int nVertexIndex = 2;
		nVertexIndex != kBrush.getNumberOfVertices();
		++nVertexIndex)
	{
		const Vec2& kvVertex = kBrush.getVertex(nVertexIndex);

		fDistance = kvNormal % kvVertex;

		if (fDistance <
			(effectiveRadius.m_fMinDistance -FLATLAND_TOL_EDGE))
		{
			// New minimum distance.
			nMinIndex = nVertexIndex;
			effectiveRadius.m_fMinDistance = fDistance;
			effectiveRadius.m_bMinEdge = false;
		}
		else if (fDistance <
			(effectiveRadius.m_fMinDistance + FLATLAND_TOL_EDGE))
		{
			// Edge.
			effectiveRadius.m_bMinEdge = true;
			if (nMinIndex == 0)
			{
				nMinIndex = nVertexIndex;
				effectiveRadius.m_fMinDistance = fDistance;
			}
		}
#ifdef FLATLAND_ENABLE_EFFECTIVE_RADIUS_MAX
		else if ((effectiveRadius.m_fMaxDistance + FLATLAND_TOL_EDGE) <
			fDistance)
		{
			// New maximum distance.
			nMaxIndex = nVertexIndex;
			effectiveRadius.m_fMaxDistance = fDistance;
			effectiveRadius.m_bMaxEdge = false;
		}
		else if ((effectiveRadius.m_fMaxDistance - FLATLAND_TOL_EDGE) <
			fDistance)
		{
			// Edge.
			effectiveRadius.m_bMaxEdge = true;
			if (nMaxIndex == 0)
			{
				nMaxIndex = nVertexIndex;
				effectiveRadius.m_fMaxDistance = fDistance;
			}
		}
#endif
	}

	// PERF use constructor to facilitate RVO?

	effectiveRadius.m_vNormal = kvNormal;
	effectiveRadius.m_vMin1 =
		kBrush.getVertex(nMinIndex);
	effectiveRadius.m_vMin2 =
		kBrush.getVertex((nMinIndex + 1) % kBrush.getNumberOfVertices());
#ifdef FLATLAND_ENABLE_EFFECTIVE_RADIUS_MAX
	effectiveRadius.m_vMax1 =
		kBrush.getVertex(nMaxIndex);
	effectiveRadius.m_vMax2 =
		kBrush.getVertex((nMaxIndex + 1) % kBrush.getNumberOfVertices());
#endif

	return effectiveRadius;
}


/*******************************************************************************
*******************************************************************************/
Geometry::EffectiveRadius
Geometry::makeEffectiveRadius(
	const Model& kModel,
	const Vec2& kvNormal)
{
	EffectiveRadius effectiveRadius =
		makeEffectiveRadius(kModel.getBrush(0), kvNormal);

	for (int nBrushIndex = 1;
		nBrushIndex != kModel.getNumberOfBrushes();
		++nBrushIndex)
	{
		const EffectiveRadius kEffectiveRadius =
			makeEffectiveRadius(kModel.getBrush(nBrushIndex), kvNormal);

		if (kEffectiveRadius.m_fMinDistance <
			effectiveRadius.m_fMinDistance - FLATLAND_TOL_EDGE)
		{
			// New minimum distance.
			effectiveRadius.m_fMinDistance = kEffectiveRadius.m_fMinDistance;
			effectiveRadius.m_bMinEdge = kEffectiveRadius.m_bMinEdge;
			effectiveRadius.m_vMin1 = kEffectiveRadius.m_vMin1;
			effectiveRadius.m_vMin2 = kEffectiveRadius.m_vMin2;
		}

#ifdef FLATLAND_ENABLE_EFFECTIVE_RADIUS_MAX
		if (effectiveRadius.m_fMaxDistance + FLATLAND_TOL_EDGE <
			kEffectiveRadius.m_fMaxDistance)
		{
			// New maximum distance.
			effectiveRadius.m_fMaxDistance = kEffectiveRadius.m_fMaxDistance;
			effectiveRadius.m_bMaxEdge = kEffectiveRadius.m_bMaxEdge;
			effectiveRadius.m_vMax1 = kEffectiveRadius.m_vMax1;
			effectiveRadius.m_vMax2 = kEffectiveRadius.m_vMax2;
		}
#endif
	}

	return effectiveRadius;
}


/*******************************************************************************
	Performs rotation then translation.
*******************************************************************************/
Mat3
Geometry::makeTransform(
	const scalar kfRotation,
	const Vec2& kvTranslation)
{
	// PERF Can optimize for no rotation/translation?
	// But right now it's mostly called by linkEntity, and most entities move.

	const scalar kfCos =
		static_cast<scalar>(cos(kfRotation));
	const scalar kfSin =
		static_cast<scalar>(sin(kfRotation));

	return
		Mat3(
			1, 0, kvTranslation[0],
			0, 1, kvTranslation[1],
			0, 0, 1) *
		Mat3(
			kfCos, -kfSin, 0,
			kfSin, kfCos, 0,
			0, 0, 1);
}


/*******************************************************************************
	Performs translation then rotation.
*******************************************************************************/
Mat3
Geometry::makeTransformInverse(
	const scalar kfRotation,
	const Vec2& kvTranslation)
{
	// PERF Can optimize for no rotation/translation?
	// But right now it's mostly called by linkEntity, and most entities move.

	const scalar kfCos =
		static_cast<scalar>(cos(kfRotation));
	const scalar kfSin =
		static_cast<scalar>(sin(kfRotation));

	return
		Mat3(
			kfCos, -kfSin, 0,
			kfSin, kfCos, 0,
			0, 0, 1) *
		Mat3(
			1, 0, kvTranslation[0],
			0, 1, kvTranslation[1],
			0, 0, 1);
}
