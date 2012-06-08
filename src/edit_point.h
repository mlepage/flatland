// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__EDIT_POINT
#define GUARD__EDIT_POINT


#include <vector>
#include "matrix.h"
#include "tstring.h"


class EditLine;


/*******************************************************************************
*******************************************************************************/
class EditPoint
{

public:

	void
	addLine(
		EditLine& line);

	const tstring&
	getName() const;

	const Vec2&
	getPoint() const;

	void
	setName(
		const tstring& ksName);

	void
	setPoint(
		const Vec2& kv);


private:

	// The name.
	tstring m_sName;

	// The point.
	Vec2 m_v;

	std::vector<EditLine*> m_cpLine;


};


/*******************************************************************************
*******************************************************************************/
inline
void
EditPoint::addLine(
	EditLine& line)
{
	m_cpLine.push_back(&line);
}


/*******************************************************************************
*******************************************************************************/
inline
const tstring&
EditPoint::getName() const
{
	return m_sName;
}


/*******************************************************************************
*******************************************************************************/
inline
const Vec2&
EditPoint::getPoint() const
{
	return m_v;
}


/*******************************************************************************
*******************************************************************************/
inline
void
EditPoint::setName(
	const tstring& ksName)
{
	m_sName = ksName;
}


/*******************************************************************************
*******************************************************************************/
inline
void
EditPoint::setPoint(
	const Vec2& kv)
{
	m_v = kv;
}


#endif // GUARD
