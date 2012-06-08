// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "screen.h"

#include <cstdio>
#include "console.h"
#include "view.h"


_TCHAR
Screen::m_anCenterBuffer[m_nCenterBufferSize];

CGapiSurface*
Screen::m_pBackBuffer;

CGapiBitmapFont*
Screen::m_pSystemFont;


/*******************************************************************************
*******************************************************************************/
void
Screen::clear()
{
	CGapiSurface& backBuffer = *getBackBuffer();

	backBuffer.FillRect(
		NULL,
		RGB(0, 0, 0),
		0,
		NULL);
}


/*******************************************************************************
*******************************************************************************/
void
Screen::drawCenterText()
{
	CGapiSurface& backBuffer = *getBackBuffer();

	backBuffer.DrawText(
		2,
		312,
		m_anCenterBuffer,
		Screen::getSystemFont(),
		0,
		NULL,
		0,
		NULL);
}


/*******************************************************************************
*******************************************************************************/
void
Screen::drawConsole()
{
	CGapiSurface& backBuffer = *getBackBuffer();

	for (int n = 0; n != 8; ++n)
	{
		backBuffer.DrawText(
			1,
			2 + n * 6,
			Console::getLine(Console::getDisplayLineIndex() - 7 + n),
			Screen::getSystemFont(),
			0,
			NULL,
			0,
			NULL);
	}
}


/*******************************************************************************
*******************************************************************************/
CGapiSurface*
Screen::getBackBuffer()
{
	return m_pBackBuffer;
}


/*******************************************************************************
*******************************************************************************/
CGapiBitmapFont*
Screen::getSystemFont()
{
	return m_pSystemFont;
}


/*******************************************************************************
*******************************************************************************/
int
Screen::print(
	const _TCHAR* kpFormat,
	...)
{
	va_list ap;

	va_start(ap, kpFormat);

	const int knResult =
		_vsntprintf(
			m_anCenterBuffer,
			m_nCenterBufferSize,
			kpFormat,
			ap);

	va_end(ap);

	// TODO Ensure buffer is null terminated under all circumstances.
	// This is a bit tricky because we may be using UNICODE. The best way is
	// probably to have an extra character at the end of the buffer, that is
	// null, and never write to it.

	return knResult;
}


/*******************************************************************************
*******************************************************************************/
void
Screen::setBackBuffer(
	CGapiSurface* const kpBackBuffer)
{
	m_pBackBuffer = kpBackBuffer;
}


/*******************************************************************************
*******************************************************************************/
void
Screen::setSystemFont(
	CGapiBitmapFont* const kpSystemFont)
{
	m_pSystemFont = kpSystemFont;
}


/*******************************************************************************
*******************************************************************************/
void
Screen::updateScreen()
{
	clear();

	// Also draw menu, console, status, etc.

	View::renderView();

	drawConsole();
	drawCenterText();
}
