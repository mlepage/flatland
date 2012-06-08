// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget_tool_bar.h"


/*******************************************************************************
*******************************************************************************/
void
WidgetToolBar::eventPaint()
{
	// TODO Fix this.
	COLORREF colour = RGB(0, 0, 128);
	RECT screenRect = reinterpret_cast<const RECT&>(getGlobalBounds());
	getBackBuffer().FillRect(
		&screenRect,
		colour,
		0,
		NULL);
}
