// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget_push_button.h"

#include "screen.h" // for text drawing font, for now


/*******************************************************************************
*******************************************************************************/
void
WidgetPushButton::drawButton()
{
	WidgetButton::drawButton();

	// Draw text.
	getBackBuffer().DrawText(
		getGlobalX() + getBounds().getWidth() / 2,
		getGlobalY() + getBounds().getHeight() / 2 - 2,
		getText().c_str(),
		Screen::getSystemFont(),
		GDDRAWTEXT_CENTER,
		NULL,
		0,
		NULL);
}
