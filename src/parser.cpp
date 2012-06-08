// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "parser.h"

#include "anim.h"
#include "brush.h"
#include "entity.h"
#include "entity_type.h"
#include "exception.h"
#include "matrix.h"
#include "model.h"
#include "resourcex.h"


/*******************************************************************************
*******************************************************************************/
Parser::Parser(
	FILE* const kpFile) :
	m_kpFile(kpFile),
	m_cnLookaheadBuffer(FLATLAND_PARSER_LINE_SIZE + 2),
	m_cnTestBuffer(128)
{
	// TODO Eliminate test buffer hardcoded size.

	getLine();
}


/*******************************************************************************
*******************************************************************************/
void
Parser::getLine()
{
	// Specialized comment handling here. If a line has leading non-whitespace
	// character '#' it is ignored.

	while (true)
	{
		if (_fgetts(
			&m_cnLookaheadBuffer[0],
			m_cnLookaheadBuffer.size(),
			m_kpFile) == NULL)
		{
			if (feof(m_kpFile))
			{
				break;
			}
			else
			{
				THROW(Exception::Parse);
			}
		}

		_TCHAR nChar;
		if (_stscanf(
			&m_cnLookaheadBuffer[0],
			_T(" %c"),
			&nChar) != 1)
		{
			THROW(Exception::Parse);
		}
		if (nChar != _T('#'))
		{
			break;
		}
	}
}


/*******************************************************************************
	Matches the line with the lookahead buffer, then gets a new line.

	Actually, only the first word in the line is matched in this implementation.
*******************************************************************************/
void
Parser::matchLine(
	const tstring& ksLine)
{
	if (queryLine(ksLine))
	{
		getLine();
	}
	else
	{
		THROW(Exception::Parse);
	}
}


/*******************************************************************************
*******************************************************************************/
void
Parser::parseAnim(
	Anim& anim)
{
	matchLine(_T("anim"));
	matchLine(_T("{"));

	tstring sAttributeName;

	while (true)
	{
		if (queryLine(_T("image_name")))
		{
			tstring s;
			parseAttribute(sAttributeName, s);
			Image& image = Resourcex::getImage(s);
			anim.setImage(image);
		}
		else if (queryLine(_T("size")))
		{
			Vec2 v;
			parseAttribute(sAttributeName, v);
			anim.setSize(v);
		}
		else
		{
			break;
		}
	}

	{
		Anim::Frame frame;
		parseAnimFrame(frame);
		anim.addFrame(frame);
	}

	while (queryLine(_T("frame")))
	{
		Anim::Frame frame;
		parseAnimFrame(frame);
		anim.addFrame(frame);
	}

	matchLine(_T("}"));
}


/*******************************************************************************
*******************************************************************************/
void
Parser::parseAnimFrame(
	Anim::Frame& frame)
{
	matchLine(_T("frame"));
	matchLine(_T("{"));

	tstring sAttributeName;

	while (true)
	{
		if (queryLine(_T("index")))
		{
			float f;
			parseAttribute(sAttributeName, f);
			frame.m_nIndex = f;
		}
		else if (queryLine(_T("count")))
		{
			float f;
			parseAttribute(sAttributeName, f);
			frame.m_nCount = f;
		}
		else
		{
			break;
		}
	}

	matchLine(_T("}"));
}


/*******************************************************************************
*******************************************************************************/
void
Parser::parseAttribute(
	tstring& sName,
	float& f)
{
	if (_stscanf(
		&m_cnLookaheadBuffer[0],
		_T(" %128s = %f "),
		&m_cnTestBuffer[0],
		&f) == 2)
	{
		sName = &m_cnTestBuffer[0];
		getLine();
	}
	else
	{
		THROW(Exception::Parse);
	}
}


/*******************************************************************************
*******************************************************************************/
void
Parser::parseAttribute(
	tstring& sName,
	tstring& s)
{
	if (_stscanf(
		&m_cnLookaheadBuffer[0],
		_T(" %128s = %*s "),
		&m_cnTestBuffer[0]) == 1)
	{
		sName = &m_cnTestBuffer[0];
	}
	else
	{
		THROW(Exception::Parse);
	}

	if (_stscanf(
		&m_cnLookaheadBuffer[0],
		_T(" %*s = %128s "),
		&m_cnTestBuffer[0]) == 1)
	{
		s = &m_cnTestBuffer[0];
	}
	else
	{
		THROW(Exception::Parse);
	}

	getLine();
}


/*******************************************************************************
*******************************************************************************/
void
Parser::parseAttribute(
	tstring& sName,
	Vec2& v)
{
	if (_stscanf(
		&m_cnLookaheadBuffer[0],
		_T(" %128s = < %f , %f > "),
		&m_cnTestBuffer[0],
		&v[0],
		&v[1]) == 3)
	{
		sName = &m_cnTestBuffer[0];
		getLine();
	}
	else
	{
		THROW(Exception::Parse);
	}
}


/*******************************************************************************
*******************************************************************************/
void
Parser::parseAttribute(
	tstring& sName,
	Vec3& v)
{
	if (_stscanf(
		&m_cnLookaheadBuffer[0],
		_T(" %128s = < %f , %f , %f > "),
		&m_cnTestBuffer[0],
		&v[0],
		&v[1],
		&v[2]) == 4)
	{
		sName = &m_cnTestBuffer[0];
		getLine();
	}
	else
	{
		THROW(Exception::Parse);
	}
}


/*******************************************************************************
*******************************************************************************/
void
Parser::parseBrush(
	Brush& brush)
{
	matchLine(_T("brush"));
	matchLine(_T("{"));

	{
		Vec2 v;
		parseVec2(v);
		brush.addVertex(v);
	}

	{
		Vec2 v;
		parseVec2(v);
		brush.addVertex(v);
	}

	{
		Vec2 v;
		parseVec2(v);
		brush.addVertex(v);
	}

	while (!queryLine(_T("}")))
	{
		Vec2 v;
		parseVec2(v);
		brush.addVertex(v);
	}

	matchLine(_T("}"));
}


/*******************************************************************************
*******************************************************************************/
void
Parser::parseEntity(
	Entity& entity)
{
	matchLine(_T("entity"));
	matchLine(_T("{"));

	tstring sAttributeName;

	while (true)
	{
		if (queryLine(_T("anim")))
		{
			Anim& anim = *new Anim();
			parseAnim(anim);
			entity.setAnim(anim);
		}
		else if (queryLine(_T("anim_name")))
		{
			tstring s;
			parseAttribute(sAttributeName, s);
			Anim& anim = Resourcex::getAnim(s);
			entity.setAnim(anim);
		}
		else if (queryLine(_T("colour")))
		{
			Vec3 v;
			parseAttribute(sAttributeName, v);
			entity.getColour() = v;
		}
		else if (queryLine(_T("path_point")))
		{
			Vec2 v;
			parseAttribute(sAttributeName, v);
			entity.m_cvPathPoint.push_back(v);
		}
		else if (queryLine(_T("path_speed")))
		{
			float f;
			parseAttribute(sAttributeName, f);
			entity.m_fPathSpeed = f;
			entity.m_nPathCurrent = 0;
			entity.m_nPathStuckCount = 0;
		}
		else if (queryLine(_T("position")))
		{
			Vec2 v;
			parseAttribute(sAttributeName, v);
			entity.getOrigin() = v;
		}
		else if (queryLine(_T("mobile")))
		{
			float f;
			parseAttribute(sAttributeName, f);
			entity.setMobile(f != 0);
		}
		else if (queryLine(_T("model")))
		{
			Model& model = *new Model();
			parseModel(model);
			entity.setModel(&model);
		}
		else if (queryLine(_T("model_name")))
		{
			tstring s;
			parseAttribute(sAttributeName, s);
			Model& model = Resourcex::getModel(s);
			entity.setModel(&model);
		}
		else if (queryLine(_T("type")))
		{
			tstring s;
			parseAttribute(sAttributeName, s);
			entity.setType(EntityType::find(s));
		}
		else if (queryLine(_T("velocity")))
		{
			Vec2 v;
			parseAttribute(sAttributeName, v);
			entity.getVelocity() = v;
		}
		else
		{
			break;
		}
	}

	matchLine(_T("}"));
}


/*******************************************************************************
*******************************************************************************/
void
Parser::parseModel(
	Model& model)
{
	matchLine(_T("model"));
	matchLine(_T("{"));

	{
		Brush brush;
		parseBrush(brush);
		model.addBrush(brush);
	}

	while (queryLine(_T("brush")))
	{
		Brush brush;
		parseBrush(brush);
		model.addBrush(brush);
	}

	matchLine(_T("}"));
}


/*******************************************************************************
*******************************************************************************/
void
Parser::parseVec2(
	Vec2& v)
{
	if (_stscanf(
		&m_cnLookaheadBuffer[0],
		_T(" < %f , %f > "),
		&v[0],
		&v[1]) == 2)
	{
		getLine();
	}
	else
	{
		THROW(Exception::Parse);
	}
}


/*******************************************************************************
	Queries the line against the lookahead buffer to see if it matches.

	Actually, only the first word in the line is matched in this implementation.
*******************************************************************************/
bool
Parser::queryLine(
	const tstring& ksLine)
{
	// TODO Eliminate test buffer hardcoded size. (See also above.)

	if (_stscanf(
		&m_cnLookaheadBuffer[0],
		_T("%128s"),
		&m_cnTestBuffer[0]) == 1)
	{
		if (_tcscmp(&m_cnTestBuffer[0], ksLine.c_str()) == 0)
		{
			return true;
		}
	}

	return false;
}
