// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__SCREEN
#define GUARD__SCREEN


#include <tchar.h>
#include "GapiDraw.h"


/*******************************************************************************
	Need: Cohen-Sutherland Line Clipping

	void
	clipDrawLine(
		const Vec2& kv1,
		const Vec2& kv2,
		const Vec2& kvMin,
		const Vec2& kvMax,
		extra stuff)
*******************************************************************************/
class Screen
{


public:

	static
	void
	clear();

	static
	void
	drawCenterText();

	static
	void
	drawConsole();

	static
	CGapiSurface*
	getBackBuffer();

	static
	CGapiBitmapFont*
	getSystemFont();

	// Call like this: print(_T("name=%s \n"), _T("value"));
	static
	int
	print(
		const _TCHAR* kpFormat,
		...);

	static
	void
	setBackBuffer(
		CGapiSurface* const kpBackBuffer);

	static
	void
	setSystemFont(
		CGapiBitmapFont* const kpSystemFont);

	static
	void
	updateScreen();


private:

	enum
	{
		m_nCenterBufferSize = 128
	};

	static
	_TCHAR m_anCenterBuffer[m_nCenterBufferSize];

	static
	CGapiSurface* m_pBackBuffer;

	static
	CGapiBitmapFont* Screen::m_pSystemFont;
};


#endif // GUARD
