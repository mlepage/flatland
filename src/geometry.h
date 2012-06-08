// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__GEOMETRY
#define GUARD__GEOMETRY


#include "config.h"
#include "matrix.h"
#include "rect.h"
#include "test.h"


class Brush;
class Model;


/*******************************************************************************
*******************************************************************************/
class Geometry
{


public:

	struct EffectiveRadius;

	static
	void
	addPointToBounds(
		const Vec2& kv,
		Rect& rBounds);

	static
	void
	addRectToBounds(
		const Rect& kr,
		Rect& rBounds);

	static
	scalar
	convertDegreesToRadians(
		const scalar kfAngle); // Angle in degrees.

	static
	bool
	isPointInRect(
		const Vec2& kv,
		const Rect& kr);

	static
	bool
	isRectTouchingRect(
		const Rect& kr1,
		const Rect& kr2);

	static
	EffectiveRadius
	makeEffectiveRadius(
		const Brush& kBrush,
		const Vec2& kvNormal);

	static
	EffectiveRadius
	makeEffectiveRadius(
		const Model& kModel,
		const Vec2& kvNormal);

	static
	Mat3
	makeTransform(
		const scalar kfRotation,    // Angle in radians.
		const Vec2& kvTranslation);

	static
	Mat3
	makeTransformInverse(
		const scalar kfRotation,    // Angle in radians.
		const Vec2& kvTranslation);

	static
	Vec2
	transformDirection(
		const Mat3& kmTransform,
		const Vec2& kvDirection);

	static
	Vec2
	transformPoint(
		const Mat3& kmTransform,
		const Vec2& kvPoint);


private:

#ifdef FLATLAND_ENABLE_TEST
	friend
	void
	Test::testGeometry();
#endif


};


/*******************************************************************************
*******************************************************************************/
inline
void
Geometry::addPointToBounds(
	const Vec2& kv,
	Rect& rBounds)
{
	// PERF there must be some way to eliminate some of these checks.

	if (kv[0] < rBounds.getMin()[0])
	{
		rBounds.getMin()[0] = kv[0];
	}
	if (rBounds.getMax()[0] < kv[0])
	{
		rBounds.getMax()[0] = kv[0];
	}

	if (kv[1] < rBounds.getMin()[1])
	{
		rBounds.getMin()[1] = kv[1];
	}
	if (rBounds.getMax()[1] < kv[1])
	{
		rBounds.getMax()[1] = kv[1];
	}
}


/*******************************************************************************
*******************************************************************************/
inline
void
Geometry::addRectToBounds(
	const Rect& kr,
	Rect& rBounds)
{
	addPointToBounds(kr.getMin(), rBounds);
	addPointToBounds(kr.getMax(), rBounds);
}


/*******************************************************************************
*******************************************************************************/
inline
scalar
Geometry::convertDegreesToRadians(
	const scalar kfAngle)
{
	return static_cast<scalar>(kfAngle * 3.14159265358979323846 * 2 / 360);
}


/*******************************************************************************
*******************************************************************************/
inline
bool
Geometry::isPointInRect(
	const Vec2& kv,
	const Rect& kr)
{
	return
		kr.getMin()[0] <= kv[0] &&
		kv[0] <= kr.getMax()[0] &&
		kr.getMin()[1] <= kv[1] &&
		kv[1] <= kr.getMax()[1];
}


/*******************************************************************************
*******************************************************************************/
inline
Vec2
Geometry::transformDirection(
	const Mat3& kmTransform,
	const Vec2& kvDirection)
{
#ifdef FLATLAND_OPTIMIZE_TRANSFORMS
	return
		Vec2(
			kmTransform[0][0] * kvDirection[0] +
				kmTransform[0][1] * kvDirection[1],
			kmTransform[1][0] * kvDirection[0] +
				kmTransform[1][1] * kvDirection[1]);
#else
	const Vec3 kv = kmTransform * Vec3(kvDirection[0], kvDirection[1], 0);
	return Vec2(kv[0], kv[1]);
#endif
}


/*******************************************************************************
*******************************************************************************/
inline
Vec2
Geometry::transformPoint(
	const Mat3& kmTransform,
	const Vec2& kvPoint)
{
#ifdef FLATLAND_OPTIMIZE_TRANSFORMS
	return
		Vec2(
			kmTransform[0][0] * kvPoint[0] +
				kmTransform[0][1] * kvPoint[1] +
				kmTransform[0][2],
			kmTransform[1][0] * kvPoint[0] +
				kmTransform[1][1] * kvPoint[1] +
				kmTransform[1][2]);
#else
	const Vec3 kv = kmTransform * Vec3(kvPoint[0], kvPoint[1], 1);
	return Vec2(kv[0], kv[1]);
#endif
}


/*******************************************************************************
	This data structure represents the effective radius of a convex polygon
	towards a normal (i.e. in the direction decreasing along the normal) and in
	the opposite direction (i.e. in the direction increasing along the normal).

	For example, consider a convex polygon's effective radius relative to the
	normal (-1, 0). The normal is pointing to the left, so the effective radius
	is the radius when travelling to the right of the convex polygon, and the
	opposite effective radius is the radius travelling to the left.

	The effective radius of a convex polygon is determined by either a vertex,
	or an edge. In the case of a vertex, the first vertex is that vertex. In
	the case of an edge, the first vertex is the head, and the second vertex is
	the tail.

	Note that the effective radius is not the distance from the origin to the
	vertex, but rather the distance from the origin projected onto the normal
	to the vertex projected onto the normal.
*******************************************************************************/
struct Geometry::EffectiveRadius
{
	Vec2 m_vNormal;
	scalar m_fMinDistance;
	bool m_bMinEdge;
	Vec2 m_vMin1;
	Vec2 m_vMin2;
#ifdef FLATLAND_ENABLE_EFFECTIVE_RADIUS_MAX
	scalar m_fMaxDistance;
	bool m_bMaxEdge;
	Vec2 m_vMax1;
	Vec2 m_vMax2;
#endif
};


#endif // GUARD
