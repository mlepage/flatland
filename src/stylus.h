// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__STYLUS
#define GUARD__STYLUS


#include "matrix.h"


/*******************************************************************************
	TODO It would be nice to get rid of setAnchorPoint and setLeadPoint but
	I'm currently using them for moving the active entity. If I can change the
	active entity to more of a selection based method, and keep track of this
	some other way, or otherwise not require it, then I can remove those
	methods and just have this class itself maintain its state. That is, I'd
	like the anchor point to always be the point where the stylus went down,
	and not be changed.
******************************************************************************/
class Stylus
{


public:

	static
	void
	doubleClick(
		const Vec2& kvScreenPoint);

	static
	void
	down(
		const Vec2& kvScreenPoint);

	static
	const Vec2&
	getAnchorPoint();

	static
	const Vec2&
	getLeadPoint();

	static
	bool
	isDown();

	static
	void
	move(
		const Vec2& kvScreenPoint);

	static
	void
	setAnchorPoint(
		const Vec2& kvScreenPoint);

	static
	void
	setLeadPoint(
		const Vec2& kvScreenPoint);

	static
	void
	up(
		const Vec2& kvScreenPoint);


private:

	static
	bool m_bDown;

	static
	Vec2 m_vAnchorPoint;

	static
	Vec2 m_vLeadPoint;

};


#endif // GUARD
