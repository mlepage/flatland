// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "screen.h"

#include <cstdio>
#include "console.h"
#include "menu.h"
#include "render.h"
#include "state_game.h"
#include "variable.h"
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
void
Screen::drawMenu()
{
	CGapiSurface& backBuffer = *getBackBuffer();

	const Rect& krScreenView = View::getScreenView();

	// Menu.
	if (Menu::hasCurrentMenu())
	{
		for (int n = 0; n != Menu::getCurrentMenu().getNumberOfItems(); ++n)
		{
			const Menu::Item& kItem = Menu::getCurrentMenu().getItem(n);
			const Rect krBounds = kItem.getBounds();
			COLORREF dwColor =
				n == Menu::getCurrentItem() ?
					RGB(255, 0, 0) :
					RGB(0, 0, 255);
			Render::drawLine(
				krBounds.getMin()[0],
				krBounds.getMin()[1],
				krBounds.getMax()[0],
				krBounds.getMin()[1],
				krScreenView.getMin()[0],
				krScreenView.getMax()[0],
				krScreenView.getMin()[1],
				krScreenView.getMax()[1],
				dwColor);
			Render::drawLine(
				krBounds.getMin()[0],
				krBounds.getMax()[1],
				krBounds.getMax()[0],
				krBounds.getMax()[1],
				krScreenView.getMin()[0],
				krScreenView.getMax()[0],
				krScreenView.getMin()[1],
				krScreenView.getMax()[1],
				dwColor);
			Render::drawLine(
				krBounds.getMin()[0],
				krBounds.getMin()[1],
				krBounds.getMin()[0],
				krBounds.getMax()[1],
				krScreenView.getMin()[0],
				krScreenView.getMax()[0],
				krScreenView.getMin()[1],
				krScreenView.getMax()[1],
				dwColor);
			Render::drawLine(
				krBounds.getMax()[0],
				krBounds.getMin()[1],
				krBounds.getMax()[0],
				krBounds.getMax()[1],
				krScreenView.getMin()[0],
				krScreenView.getMax()[0],
				krScreenView.getMin()[1],
				krScreenView.getMax()[1],
				dwColor);
			Screen::getBackBuffer()->DrawText(
				(krBounds.getMax()[0] + krBounds.getMin()[0]) / 2,
				(krBounds.getMax()[1] + krBounds.getMin()[1]) / 2 - 2,
				kItem.getName().c_str(),
				Screen::getSystemFont(),
				GDDRAWTEXT_CENTER,
				NULL,
				0,
				NULL);
		}
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
	// TODO maybe all this should just be in game state process frame?
	if (StateGame::isState())
	{
		// TODO fix this clear so only what isn't drawn is cleared.
		clear();
		View::renderView();
		if (Variable::render_console.getFloatValue())
		{
			drawConsole();
		}
		if (Variable::render_frame_info.getFloatValue())
		{
			drawCenterText();
		}
	}
}
