// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget_tool_button.h"

#include "screen.h" // for text drawing font, for now


/*******************************************************************************
*******************************************************************************/
void
WidgetToolButton::drawButton()
{
	WidgetButton::drawButton();

	if (!hasIconSet())
	{
		return;
	}

	const int knImageIndex =
		isDown() ? 2 : 0;

	const int knWidth = getIconSet().GetWidth() / 3;
	const int knHeight = getIconSet().GetHeight();

	RECT sourceRect;
	sourceRect.top = 0;
	sourceRect.bottom = knHeight;
	sourceRect.left = knImageIndex * knWidth;
	sourceRect.right = (knImageIndex + 1) * knWidth;

	if (!isEnabled())
	{
		GDBLTFASTFX fx;
		fx.dwFillColor = RGB(0, 0, 96);
		getBackBuffer().BltFast(
			getGlobalX(),
			getGlobalY(),
			&getIconSet(),
			&sourceRect,
			GDBLTFAST_COLORFILL | GDBLTFAST_KEYSRC,
			&fx);
	}
	else if (isToggleButton() && isOn() && !isDown())
	{
		GDBLTFASTFX fx;
		fx.dwFillColor = Colour::highlight();
		getBackBuffer().BltFast(
			getGlobalX(),
			getGlobalY(),
			&getIconSet(),
			&sourceRect,
			GDBLTFAST_COLORFILL | GDBLTFAST_KEYSRC,
			&fx);
	}
	else
	{
		getBackBuffer().BltFast(
			getGlobalX(),
			getGlobalY(),
			&getIconSet(),
			&sourceRect,
			GDBLTFAST_KEYSRC,
			NULL);
	}
}
