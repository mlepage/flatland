// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__EDIT_POINT
#define GUARD__EDIT_POINT


#include <algorithm>
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
		EditLine& line)
	{
		m_cpLine.push_back(&line);
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

	const Vec2&
	getPoint() const
	{
		return m_v;
	}

	void
	removeLine(
		EditLine& line)
	{
		m_cpLine.erase(std::find(m_cpLine.begin(), m_cpLine.end(), &line));
	}

	void
	setName(
		const tstring& ksName)
	{
		m_sName = ksName;
	}

	void
	setPoint(
		const Vec2& kv)
	{
		m_v = kv;
	}


private:

	// The name.
	tstring m_sName;

	// The point.
	Vec2 m_v;

	std::vector<EditLine*> m_cpLine;


};


#endif // GUARD
