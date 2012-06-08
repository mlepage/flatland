// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__EDIT_LINE
#define GUARD__EDIT_LINE


#include <vector>

#include "edit_point.h"
#include "tstring.h"


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

	EditPoint* m_pBeginPoint;

	EditPoint* m_pEndPoint;

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
	return *m_pBeginPoint;
}


/*******************************************************************************
*******************************************************************************/
inline
EditPoint&
EditLine::getEndPoint()
{
	return *m_pEndPoint;
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
	// TODO remove old point

	m_pBeginPoint = &beginPoint;

	beginPoint.addLine(*this);
}


/*******************************************************************************
*******************************************************************************/
inline
void
EditLine::setEndPoint(
	EditPoint& endPoint)
{
	// TODO remove old point

	m_pEndPoint = &endPoint;

	endPoint.addLine(*this);
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
