// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__EDIT_LINE
#define GUARD__EDIT_LINE


#include <vector>
#include "tstring.h"


class EditPoint;
class EditPolygon;


/*******************************************************************************
*******************************************************************************/
class EditLine
{

public:

	void
	addPolygon(
		EditPolygon& polygon);

	EditPoint&
	getBeginPoint();

	EditPoint&
	getEndPoint();

	const tstring&
	getName() const;

	void
	setBeginPoint(
		EditPoint& beginPoint);

	void
	setEndPoint(
		EditPoint& endPoint);

	void
	setName(
		const tstring& ksName);


private:

	// The name.
	tstring m_sName;

	EditPoint* m_beginPoint;

	EditPoint* m_endPoint;

	std::vector<EditPolygon*> m_cpPolygon;


};


/*******************************************************************************
*******************************************************************************/
inline
void
EditLine::addPolygon(
	EditPolygon& polygon)
{
	// TODO Maybe should ensure that only two polygons are added.
	m_cpPolygon.push_back(&polygon);
}


/*******************************************************************************
*******************************************************************************/
inline
EditPoint&
EditLine::getBeginPoint()
{
	return *m_beginPoint;
}


/*******************************************************************************
*******************************************************************************/
inline
EditPoint&
EditLine::getEndPoint()
{
	return *m_endPoint;
}


/*******************************************************************************
*******************************************************************************/
inline
const tstring&
EditLine::getName() const
{
	return m_sName;
}


/*******************************************************************************
*******************************************************************************/
inline
void
EditLine::setBeginPoint(
	EditPoint& beginPoint)
{
	m_beginPoint = &beginPoint;
}


/*******************************************************************************
*******************************************************************************/
inline
void
EditLine::setEndPoint(
	EditPoint& endPoint)
{
	m_endPoint = &endPoint;
}


/*******************************************************************************
*******************************************************************************/
inline
void
EditLine::setName(
	const tstring& ksName)
{
	m_sName = ksName;
}


#endif // GUARD
