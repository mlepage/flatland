// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__EDIT_POLYGON
#define GUARD__EDIT_POLYGON


#include <algorithm>
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
		EditLine& line)
	{
		m_cpLine.push_back(&line);
		line.addPolygon(*this);
	}

	EditLine&
	getLine(
		const int knIndex)
	{
		return *m_cpLine[knIndex];
	}

	const tstring&
	getName() const
	{
		return m_sName;
	}

	int
	getNumberOfLines() const
	{
		return m_cpLine.size();
	}

	void
	removeLine(
		EditLine& line)
	{
		m_cpLine.erase(std::find(m_cpLine.begin(), m_cpLine.end(), &line));
		line.removePolygon(*this);
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

	std::vector<EditLine*> m_cpLine;


};


#endif // GUARD
