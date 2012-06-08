// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget_line_edit.h"

#include "screen.h" // for text drawing font, for now


/*******************************************************************************
******************************************************************************/
void
WidgetLineEdit::eventKeyPress(
	const int knKeyCode)
{
	if (knKeyCode == VK_BACK)
	{
		if (m_sText.size())
		{
			m_sText.erase(m_sText.size() - 1);
		}
	}
	else if (knKeyCode == _T('-') ||
		_T('a') <= knKeyCode &&
		knKeyCode <= _T('z') ||
		_T('0') <= knKeyCode &&
		knKeyCode <= _T('9'))
	{
		m_sText += tstring(1, knKeyCode);
	}
}


/*******************************************************************************
******************************************************************************/
void
WidgetLineEdit::eventPaint()
{
	// TODO Fix this.
	RECT screenRect = reinterpret_cast<const RECT&>(getGlobalBounds());
	getBackBuffer().FillRect(
		&screenRect,
		RGB(0, 0, 64),
		0,
		NULL);
	getBackBuffer().DrawRect(
		&screenRect,
		RGB(255, 255, 255),
		0,
		NULL);

	// Use name for now.
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
