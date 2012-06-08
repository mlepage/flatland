// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__EDIT_POLYGON
#define GUARD__EDIT_POLYGON


#include <vector>

#include "edit_line.h"
#include "tstring.h"


/*******************************************************************************
*******************************************************************************/
class EditPolygon
{

public:

	void
	addLine(
		EditLine& line);

	EditLine&
	getLine(
		const int knIndex);

	const tstring&
	getName() const;

	int
	getNumberOfLines() const;

	void
	setName(
		const tstring& ksName);


private:

	// The name.
	tstring m_sName;

	std::vector<EditLine*> m_cpLine;


};


/*******************************************************************************
*******************************************************************************/
inline
void
EditPolygon::addLine(
	EditLine& line)
{
	m_cpLine.push_back(&line);

	line.addPolygon(*this);
}


/*******************************************************************************
*******************************************************************************/
inline
EditLine&
EditPolygon::getLine(
	const int knIndex)
{
	return *m_cpLine[knIndex];
}


/*******************************************************************************
*******************************************************************************/
inline
const tstring&
EditPolygon::getName() const
{
	return m_sName;
}


/*******************************************************************************
*******************************************************************************/
inline
int
EditPolygon::getNumberOfLines() const
{
	return m_cpLine.size();
}


/*******************************************************************************
*******************************************************************************/
inline
void
EditPolygon::setName(
	const tstring& ksName)
{
	m_sName = ksName;
}


#endif // GUARD
