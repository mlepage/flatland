// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget_menu_bar.h"

#include "key.h"


/*******************************************************************************
*******************************************************************************/
void
WidgetMenuBar::eventHide()
{
	releaseKeyboard();
}


/*******************************************************************************
*******************************************************************************/
void
WidgetMenuBar::eventKeyPress(
	const int knKeyCode)
{
	if (knKeyCode == KeyCode::menu())
	{
		hide();
	}
}


/*******************************************************************************
*******************************************************************************/
void
WidgetMenuBar::eventShow()
{
	grabKeyboard();
}


/*******************************************************************************
*******************************************************************************/
void
WidgetMenuBar::eventPaint()
{
	COLORREF colour = RGB(0, 0, 128);
	// TODO Fix this.
	RECT screenRect = reinterpret_cast<const RECT&>(getGlobalBounds());
	++screenRect.top;
	--screenRect.bottom;
	getBackBuffer().DrawLine(
		screenRect.left + 1,
		screenRect.top - 1,
		screenRect.right - 2,
		screenRect.top - 1,
		colour,
		0,
		NULL);
	getBackBuffer().FillRect(
		&screenRect,
		colour,
		0,
		NULL);
	getBackBuffer().DrawLine(
		screenRect.left + 1,
		screenRect.bottom,
		screenRect.right - 2,
		screenRect.bottom,
		colour,
		0,
		NULL);
}
