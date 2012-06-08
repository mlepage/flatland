// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__EDIT_MODEL
#define GUARD__EDIT_MODEL


#include "directory.h"
#include "edit_line.h"
#include "edit_point.h"
#include "edit_polygon.h"


/*******************************************************************************
*******************************************************************************/
class EditModel
{

public:

	void
	addLine(
		EditLine& line);

	void
	addPoint(
		EditPoint& point);

	void
	addPolygon(
		EditPolygon& polygon);

	EditLine*
	findLine(
		const tstring& ksName);

	EditPoint*
	findPoint(
		const tstring& ksName);

	EditPolygon*
	findPolygon(
		const tstring& ksName);

	const tstring&
	getName() const;

	int
	getNumberOfLines() const;

	int
	getNumberOfPoints() const;

	int
	getNumberOfPolygons() const;

	EditLine&
	getLine(
		const int knIndex);

	EditPoint&
	getPoint(
		const int knIndex);

	EditPolygon&
	getPolygon(
		const int knIndex);

	void
	setName(
		const tstring& ksName);


private:

	// The name.
	tstring m_sName;

	Directory<EditLine> m_lineDirectory;

	Directory<EditPoint> m_pointDirectory;

	Directory<EditPolygon> m_polygonDirectory;

	std::vector<EditLine*> m_cpLine;

	std::vector<EditPoint*> m_cpPoint;

	std::vector<EditPolygon*> m_cpPolygon;

};


/*******************************************************************************
*******************************************************************************/
inline
void
EditModel::addLine(
	EditLine& line)
{
	m_lineDirectory.insert(line.getName(), line);

	// TODO Still need to handle case where there already was one of the
	// same name.
	m_cpLine.push_back(&line);
}


/*******************************************************************************
*******************************************************************************/
inline
void
EditModel::addPoint(
	EditPoint& point)
{
	m_pointDirectory.insert(point.getName(), point);

	// TODO Still need to handle case where there already was one of the
	// same name.
	m_cpPoint.push_back(&point);
}


/*******************************************************************************
*******************************************************************************/
inline
void
EditModel::addPolygon(
	EditPolygon& polygon)
{
	m_polygonDirectory.insert(polygon.getName(), polygon);

	// TODO Still need to handle case where there already was one of the
	// same name.
	m_cpPolygon.push_back(&polygon);
}


/*******************************************************************************
	Returns null if none found.
*******************************************************************************/
inline
EditLine*
EditModel::findLine(
	const tstring& ksName)
{
	return m_lineDirectory.find(ksName);
}


/*******************************************************************************
	Returns null if none found.
*******************************************************************************/
inline
EditPoint*
EditModel::findPoint(
	const tstring& ksName)
{
	return m_pointDirectory.find(ksName);
}


/*******************************************************************************
	Returns null if none found.
*******************************************************************************/
inline
EditPolygon*
EditModel::findPolygon(
	const tstring& ksName)
{
	return m_polygonDirectory.find(ksName);
}


/*******************************************************************************
*******************************************************************************/
inline
const tstring&
EditModel::getName() const
{
	return m_sName;
}


/*******************************************************************************
*******************************************************************************/
inline
int
EditModel::getNumberOfLines() const
{
	return m_cpLine.size();
}


/*******************************************************************************
*******************************************************************************/
inline
int
EditModel::getNumberOfPoints() const
{
	return m_cpPoint.size();
}


/*******************************************************************************
*******************************************************************************/
inline
int
EditModel::getNumberOfPolygons() const
{
	return m_cpPolygon.size();
}


/*******************************************************************************
*******************************************************************************/
inline
EditLine&
EditModel::getLine(
	const int knIndex)
{
	return *m_cpLine[knIndex];
}


/*******************************************************************************
*******************************************************************************/
inline
EditPoint&
EditModel::getPoint(
	const int knIndex)
{
	return *m_cpPoint[knIndex];
}


/*******************************************************************************
*******************************************************************************/
inline
EditPolygon&
EditModel::getPolygon(
	const int knIndex)
{
	return *m_cpPolygon[knIndex];
}


/*******************************************************************************
*******************************************************************************/
inline
void
EditModel::setName(
	const tstring& ksName)
{
	m_sName = ksName;
}


#endif // GUARD
