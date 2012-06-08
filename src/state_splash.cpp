// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "state_splash.h"

#include "image.h"
#include "menu.h"
#include "resourcex.h"
#include "screen.h"
#include "state_title.h"


StateSplash
StateSplash::m_state;


/*******************************************************************************
*******************************************************************************/
void
StateSplash::processFrame()
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();

	// TODO move these into configuration somewhere
	const int knFadeTime = 30;
	const int knFullTime = 30;
	static const int knFadeInFrame = Application::getActualFrameNumber();
	static const int knFullFrame = knFadeInFrame + knFadeTime;
	static const int knFadeOutFrame = knFullFrame + knFullTime;
	static const int knFinishFrame = knFadeOutFrame + knFadeTime;
	const int knCurrentFrame = Application::getActualFrameNumber();

	if (knCurrentFrame < knFinishFrame)
	{
		backBuffer.FillRect(
			NULL,
			RGB(0, 0, 0),
			0,
			NULL);
		GDBLTFASTFX fx;
		if (knCurrentFrame < knFullFrame)
		{
			// Fade in.
			fx.dwOpacity =
				static_cast<double>(knCurrentFrame - knFadeInFrame) /
					knFadeTime * 255;
		}
		else if (knFadeOutFrame <= knCurrentFrame)
		{
			// Fade out.
			fx.dwOpacity =
				static_cast<double>(knFinishFrame - knCurrentFrame - 1) /
					knFadeTime * 255;
		}
		else
		{
			fx.dwOpacity = 255;
		}
		backBuffer.BltFast(
			0,
			0,
			&Resourcex::getImage(_T("screen_splash")).getSurface(),
			NULL,
			GDBLTFAST_OPACITY,
			&fx);
	}
	else
	{
		StateTitle::changeState();
		Menu::setCurrentMenu(Menu::title_main);
		Menu::setCurrentItem(0);
	}
}
