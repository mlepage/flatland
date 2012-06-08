// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget_tool_button.h"

#include "screen.h" // for text drawing font, for now


/*******************************************************************************
******************************************************************************/
void
WidgetToolButton::drawButton()
{
	// TODO Fix this.
	RECT screenRect = reinterpret_cast<const RECT&>(getGlobalBounds());
	getBackBuffer().FillRect(
		&screenRect,
		RGB(0, 0, 255),
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
