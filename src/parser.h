// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__PARSER
#define GUARD__PARSER


#include <cstdio>
#include <vector>
#include <tchar.h>
#include "tstring.h"


class Brush;
class Entity;
class Model;
class Vec2;
class Vec3;


/*******************************************************************************
*******************************************************************************/
class Parser
{

public:

	Parser(
		FILE* const kpFile);


public:

	void
	parseEntity(
		Entity& entity);

	void
	parseModel(
		Model& model);

	bool
	testLine(
		const tstring& ksLine);


private:

	void
	getLine();

	void
	matchLine(
		const tstring& ksLine);

	void
	parseAttribute(
		tstring& sName,
		float& f);

	void
	parseAttribute(
		tstring& sName,
		tstring& s);

	void
	parseAttribute(
		tstring& sName,
		Vec2& v);

	void
	parseAttribute(
		tstring& sName,
		Vec3& v);

	void
	parseBrush(
		Brush& brush);

	void
	parseVec2(
		Vec2& v);


private:

	FILE* m_kpFile;

	// Size is FLATLAND_PARSER_LINE_SIZE + 2 to hold newline and terminator.
	std::vector<_TCHAR> m_cnLookaheadBuffer;

	// TODO Eliminate test buffer hardcoded size.
	std::vector<_TCHAR> m_cnTestBuffer;

};


#endif // GUARD
