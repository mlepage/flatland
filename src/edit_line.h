// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__EDIT_LINE
#define GUARD__EDIT_LINE


#include <algorithm>
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
		EditPolygon& polygon)
	{
		// TODO Maybe should ensure that only two polygons are added.
		m_cpPolygon.push_back(&polygon);
	}

	EditPoint&
	getBeginPoint()
	{
		return *m_pBeginPoint;
	}

	EditPoint&
	getEndPoint()
	{
		return *m_pEndPoint;
	}

	const tstring&
	getName() const
	{
		return m_sName;
	}

	int
	getNumberOfPolygons() const
	{
		return m_cpPolygon.size();
	}

	EditPolygon&
	getPolygon(
		const int knIndex)
	{
		return *m_cpPolygon[knIndex];
	}

	void
	removePolygon(
		EditPolygon& polygon)
	{
		m_cpPolygon.erase(
			std::find(m_cpPolygon.begin(), m_cpPolygon.end(), &polygon));
	}

	void
	setBeginPoint(
		EditPoint& beginPoint)
	{
		// TODO Handle existing old point.
		m_pBeginPoint = &beginPoint;
		beginPoint.addLine(*this);
	}

	void
	setEndPoint(
		EditPoint& endPoint)
	{
		// TODO Handle existing old point.
		m_pEndPoint = &endPoint;
		endPoint.addLine(*this);
	}

	void
	setName(
		const tstring& ksName)
	{
		m_sName = ksName;
	}


private:

	// The name.
	tstring m_sName;

	EditPoint* m_pBeginPoint;

	EditPoint* m_pEndPoint;

	std::vector<EditPolygon*> m_cpPolygon;


};


#endif // GUARD
