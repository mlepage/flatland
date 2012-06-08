// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "edit_model.h"

#include <set>


/*******************************************************************************
*******************************************************************************/
EditModel::~EditModel()
{
	{for (int n = 0; n != m_cpPoint.size(); ++n)
	{
		delete m_cpPoint[n];
	}}
	{for (int n = 0; n != m_cpLine.size(); ++n)
	{
		delete m_cpLine[n];
	}}
	{for (int n = 0; n != m_cpPolygon.size(); ++n)
	{
		delete m_cpPolygon[n];
	}}
}


/*******************************************************************************
	Roughly, need to create sets of points, lines, and polygons to destroy.
	Then, need to delete them, removing them from internal lists and also
	internal directories. Since the points/lines/polygons don't currently
	delete each other, I don't have to worry about double deletion.

	It's OK to leave lone points, but not lone lines. This is tricky, since for
	a deleted polygon, some of its lines may have to be deleted (because they
	only belong to that polygon), and some may not have to be deleted (because
	they also belong to another polygon).

	Another problem is dangling pointers. Say a point is deleted, and also a
	line containing it. But the other point the line contains is not deleted.
	This is a problem because that point still thinks it is a part of the line
	which was just deleted. To fix this, I need to ensure that points, lines,
	and polygons actually remove themselves from each other when deleted.

	TODO I need a whole friggin document on the structure of these classes and
	ownership thereof.

	TODO Fix data corruption with dangling pointers.
*******************************************************************************/
void
EditModel::destroyPoints(
	const std::vector<EditPoint*> kcpPoint)
{
	// Sets of lines and polygons to destroy.
	std::set<EditLine*> lineSet;
	std::set<EditPolygon*> polygonSet;

	for (int nPointIndex = 0; nPointIndex != kcpPoint.size(); ++nPointIndex)
	{
		EditPoint& point = *kcpPoint[nPointIndex];
		for (int nLineIndex = 0;
			nLineIndex != point.getNumberOfLines(); ++nLineIndex)
		{
			EditLine& line = point.getLine(nLineIndex);
			lineSet.insert(&line);
			for (int nPolygonIndex = 0;
				nPolygonIndex != line.getNumberOfPolygons(); ++nPolygonIndex)
			{
				EditPolygon& polygon = line.getPolygon(nPolygonIndex);
				polygonSet.insert(&polygon);
			}
		}
	}

	{for (std::set<EditPolygon*>::iterator i = polygonSet.begin();
		i != polygonSet.end(); ++i)
	{
		for (int nLineIndex = 0;
			nLineIndex != (*i)->getNumberOfLines(); ++nLineIndex)
		{
			EditLine& line = (*i)->getLine(nLineIndex);
			if (std::find(lineSet.begin(), lineSet.end(),
				&line) == lineSet.end())
			{
				line.removePolygon(**i);
			}
		}
		m_polygonDirectory.erase((*i)->getName());
		m_cpPolygon.erase(
			std::find(m_cpPolygon.begin(), m_cpPolygon.end(), *i));
		delete *i;
	}}

	{for (std::set<EditLine*>::iterator i = lineSet.begin();
		i != lineSet.end(); ++i)
	{
		if (std::find(kcpPoint.begin(), kcpPoint.end(),
			&(*i)->getBeginPoint()) == kcpPoint.end())
		{
			(*i)->getBeginPoint().removeLine(**i);
		}
		if (std::find(kcpPoint.begin(), kcpPoint.end(),
			&(*i)->getEndPoint()) == kcpPoint.end())
		{
			(*i)->getEndPoint().removeLine(**i);
		}
		m_lineDirectory.erase((*i)->getName());
		m_cpLine.erase(
			std::find(m_cpLine.begin(), m_cpLine.end(), *i));
		delete *i;
	}}

	{for (std::vector<EditPoint*>::const_iterator i = kcpPoint.begin();
		i != kcpPoint.end(); ++i)
	{
		m_pointDirectory.erase((*i)->getName());
		m_cpPoint.erase(
			std::find(m_cpPoint.begin(), m_cpPoint.end(), *i));
		delete *i;
	}}
}
