// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget_push_button.h"

#include "screen.h" // for text drawing font, for now


/*******************************************************************************
******************************************************************************/
void
WidgetPushButton::drawButton()
{
	COLORREF colour =
		isDown() ?
			RGB(128 + 64, 128 + 64, 0) :
			RGB(0, 0, 255);

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

	// Use name for now.
	getBackBuffer().DrawText(
		getGlobalX() + getBounds().getWidth() / 2,
		getGlobalY() + getBounds().getHeight() / 2 - 2,
		getName().c_str(),
		Screen::getSystemFont(),
		GDDRAWTEXT_CENTER,
		NULL,
		0,
		NULL);
}
