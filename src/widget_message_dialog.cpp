// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget_message_dialog.h"

#include "screen.h"
#include "widget_push_button.h"


/*******************************************************************************
*******************************************************************************/
void
WidgetMessageDialog::constructCommon()
{
	const int knSpace = 70;
	setSize(WDim(240, 320 - knSpace - knSpace));
	setPosition(WPoint(0, knSpace));

	layoutButtons();
}


/*******************************************************************************
*******************************************************************************/
void
WidgetMessageDialog::eventPaint()
{
	// TODO Fix this.
	COLORREF colour = RGB(0, 0, 128);
	RECT screenRect = reinterpret_cast<const RECT&>(getGlobalBounds());

	const int knSpace = 70;

	getBackBuffer().FillRect(
		&screenRect,
		colour,
		0,
		NULL);

	// These two commands draw outside the widget. Bad.
	screenRect.top = 0;
	screenRect.bottom = knSpace;
	getBackBuffer().FillRect(
		&screenRect,
		RGB(0, 0, 0),
		0,
		NULL);
	screenRect.top = 320 - knSpace;
	screenRect.bottom = 320;
	getBackBuffer().FillRect(
		&screenRect,
		RGB(0, 0, 0),
		0,
		NULL);

	getBackBuffer().DrawText(
		64,
		knSpace + 24,
		getText().c_str(),
		Screen::getSystemFont(),
		0,
		NULL,
		0,
		NULL);
}
