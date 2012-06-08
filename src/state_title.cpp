// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "state_title.h"

#include "image.h"
#include "resourcex.h"
#include "screen.h"


StateTitle
StateTitle::m_state;


/*******************************************************************************
*******************************************************************************/
void
StateTitle::processFrame()
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();

	CGapiSurface& image = Resourcex::getImage(_T("screen_title")).getSurface();
	backBuffer.BltFast(0, 0, &image, NULL, 0, NULL);

	//Screen::updateScreen();
}
