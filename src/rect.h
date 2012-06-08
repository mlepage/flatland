// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__RECT
#define GUARD__RECT


#include "matrix.h"


/*******************************************************************************
	A rectangle defined by a minimum coordinate and a maximum coordinate.
*******************************************************************************/
class Rect
{


public:

	Rect();

	Rect(
		const Vec2& kvMin,
		const Vec2& kvMax);

	Vec2&
	getMax();

	const Vec2&
	getMax() const;

	Vec2&
	getMin();

	const Vec2&
	getMin() const;


private:

	// The minimum.
	Vec2 m_vMin;

	// The maximum.
	Vec2 m_vMax;


};


/*******************************************************************************
*******************************************************************************/
inline
Rect::Rect()
{
}


/*******************************************************************************
*******************************************************************************/
inline
Rect::Rect(
	const Vec2& kvMin,
	const Vec2& kvMax) :
	m_vMin(kvMin),
	m_vMax(kvMax)
{
}


/*******************************************************************************
*******************************************************************************/
inline
Vec2&
Rect::getMax()
{
	return m_vMax;
}


/*******************************************************************************
*******************************************************************************/
inline
const Vec2&
Rect::getMax() const
{
	return m_vMax;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec2&
Rect::getMin()
{
	return m_vMin;
}


/*******************************************************************************
*******************************************************************************/
inline
const Vec2&
Rect::getMin() const
{
	return m_vMin;
}


#endif // GUARD