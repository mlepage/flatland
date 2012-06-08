// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__STYLUS
#define GUARD__STYLUS


#include "matrix.h"


/*******************************************************************************
******************************************************************************/
class Stylus
{


public:

	static
	const Vec2&
	getAnchorPoint();

	static
	const Vec2&
	getLeadPoint();

	static
	void
	setAnchorPoint(
		const Vec2& kvPoint);

	static
	void
	setLeadPoint(
		const Vec2& kvPoint);


private:

	static
	Vec2 m_vAnchorPoint;

	static
	Vec2 m_vLeadPoint;

};


#endif // GUARD
