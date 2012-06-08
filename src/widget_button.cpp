// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget_button.h"


/*******************************************************************************
*******************************************************************************/
void
WidgetButton::drawButton()
{
	COLORREF colour;

	if (isDown())
	{
		colour = Colour::highlight();
	}
	else if (!isEnabled())
	{
		colour = RGB(48, 48, 255);
	}
	else
	{
		colour = RGB(0, 0, 255);
	}

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
