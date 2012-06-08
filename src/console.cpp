// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "console.h"

#include <cstdarg>
#include <cstdio>
#include "config.h"
#include "resourcex.h"


namespace
{
	_TCHAR anPrintBuffer[FLATLAND_CONSOLE_PRINT_BUFFER_SIZE + 1];

	_TCHAR aanTextBuffer
		[FLATLAND_CONSOLE_TEXT_BUFFER_SIZE]
		[FLATLAND_CONSOLE_TEXT_LINE_SIZE + 1];

	int nTextCurrentLine;

	FILE* pLogFile = NULL;
}


/*******************************************************************************
*******************************************************************************/
const _TCHAR*
Console::getLine(
	const int nLineIndex)
{
	return aanTextBuffer[nLineIndex % FLATLAND_CONSOLE_TEXT_BUFFER_SIZE];
}


/*******************************************************************************
*******************************************************************************/
int
Console::getDisplayLineIndex()
{
	return nTextCurrentLine;
}


/*******************************************************************************
*******************************************************************************/
void
Console::init()
{
	// Terminate print buffer.
	anPrintBuffer[FLATLAND_CONSOLE_PRINT_BUFFER_SIZE] = _T('\0');

	// Terminate text buffer lines.
	for (int nLine = 0;
		nLine != FLATLAND_CONSOLE_TEXT_BUFFER_SIZE;
		++nLine)
	{
		aanTextBuffer[nLine][FLATLAND_CONSOLE_TEXT_LINE_SIZE] = _T('\0');
	}

	nTextCurrentLine = -1;

	pLogFile = Resourcex::openFile(_T("flatland-log.txt"), _T("w"));

	Console::print(_T("Console initialized.\n"));
}


/*******************************************************************************
	Buffer size is limited, but will not overflow.
*******************************************************************************/
int
Console::print(
	const _TCHAR* kpFormat,
	...)
{
	va_list ap;

	va_start(ap, kpFormat);

	const int knResult =
		_vsntprintf(
			anPrintBuffer,
			FLATLAND_CONSOLE_PRINT_BUFFER_SIZE,
			kpFormat,
			ap);

	va_end(ap);

	// TODO break these lines properly.
	int nTotalCount = 0;
	while (nTotalCount != knResult)
	{
		++nTextCurrentLine;

		const int knCurrentCount =
			_sntprintf(
				aanTextBuffer[
					nTextCurrentLine % FLATLAND_CONSOLE_TEXT_BUFFER_SIZE],
				FLATLAND_CONSOLE_TEXT_LINE_SIZE,
				_T("%s"),
				anPrintBuffer + nTotalCount);
		if (knCurrentCount < 0)
		{
			nTotalCount += FLATLAND_CONSOLE_TEXT_LINE_SIZE;
		}
		else
		{
			nTotalCount += knCurrentCount;
		}
	}

	// Copy to the log.
	if (pLogFile)
	{
		_ftprintf(pLogFile, _T("%s"), anPrintBuffer);
	}

	return knResult;
}
