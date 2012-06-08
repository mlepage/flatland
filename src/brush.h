// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__BRUSH
#define GUARD__BRUSH


#include <cmath>
#include <vector>
#include "geometry.h"
#include "matrix.h"
#include "rect.h"


/*******************************************************************************
	The brush is a convex polygon. It is specified by its vertices, in
	counterclockwise order. Therefore, in a righthanded coordinate system, the
	edge normals point outwards.
  
	The edges are indexed with the tail vertex. That is, if the polygon has
	vertices A B C, then it has edges AB BC CA (and not CA AB BC). Put another
	way, edge 0 is from vertex 0 to vertex 1.

	The brush exists in the model's coordinate system.
*******************************************************************************/
class Brush
{


public:

	void
	addVertex(
		const Vec2& kvVertex);

	const Rect&
	getBounds() const;

	const Vec3&
	getLine(
		const int knIndex) const;

	int
	getNumberOfVertices() const;

	const Vec2&
	getVertex(
		const int knIndex) const;


private:

	// The bounding box.
	Rect m_rBounds;

	// The vertices.
	std::vector<Vec2> m_cvVertex;

	// The lines.
	std::vector<Vec3> m_cvLine;

};


/*******************************************************************************
*******************************************************************************/
inline
const Rect&
Brush::getBounds() const
{
	return m_rBounds;
}


/*******************************************************************************
*******************************************************************************/
inline
const Vec3&
Brush::getLine(
	const int knIndex) const
{
	return m_cvLine[knIndex];
}


/*******************************************************************************
*******************************************************************************/
inline
int
Brush::getNumberOfVertices() const
{
	return m_cvVertex.size();
}


/*******************************************************************************
*******************************************************************************/
inline
const Vec2&
Brush::getVertex(
	const int knIndex) const
{
	return m_cvVertex[knIndex];
}


#endif // GUARD
