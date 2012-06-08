// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "key.h"

#include "application.h"
#include "game.h"
#include "menu.h"
#include "view.h"


// Key codes.
int KeyCode::m_knEnter;
int KeyCode::m_knUp;
int KeyCode::m_knDown;
int KeyCode::m_knLeft;
int KeyCode::m_knRight;
int KeyCode::m_knMenu;
int KeyCode::m_knScreenShot;
int KeyCode::m_knControl;
int KeyCode::m_knShift;

std::vector<bool>
Key::m_cbKeyDown(256);


/*******************************************************************************
*******************************************************************************/
void
Key::processCommands()
{
	// TODO Move all this shit out of here.

	if (Application::isScrollViewEnabled())
	{
		// Right now this function only handles view scrolling, so exit if
		// otherwise. This all needs to be refactored.
		return;
	}

	const int knViewMove = 10;

	if (isKeyDown(m_knKeyUp))
	{
		View::setWorldViewOrigin(
			View::getWorldView().getMin() + Vec2(0, knViewMove));
		Game::setAutoscrollEnabled(false);
	}
	if (isKeyDown(m_knKeyDown))
	{
		View::setWorldViewOrigin(
			View::getWorldView().getMin() + Vec2(0, -knViewMove));
		Game::setAutoscrollEnabled(false);
	}
	if (isKeyDown(m_knKeyLeft))
	{
		View::setWorldViewOrigin(
			View::getWorldView().getMin() + Vec2(-knViewMove, 0));
		Game::setAutoscrollEnabled(false);
	}
	if (isKeyDown(m_knKeyRight))
	{
		View::setWorldViewOrigin(
			View::getWorldView().getMin() + Vec2(knViewMove, 0));
		Game::setAutoscrollEnabled(false);
	}
}


/*******************************************************************************
*******************************************************************************/
void
KeyCode::restoreDefaults(
	GDKEYLIST& keyList)
{
	KeyCode::m_knEnter = keyList.vkStart;
	KeyCode::m_knUp = keyList.vkUp;
	KeyCode::m_knDown = keyList.vkDown;
	KeyCode::m_knLeft = keyList.vkLeft;
	KeyCode::m_knRight = keyList.vkRight;

#ifdef _WIN32_WCE
	KeyCode::m_knMenu = keyList.vkC;
	KeyCode::m_knScreenShot = 0;
	KeyCode::m_knControl = keyList.vkA;
	KeyCode::m_knShift = keyList.vkB;
#else
	KeyCode::m_knMenu = VK_ESCAPE;
	KeyCode::m_knScreenShot = 'S';
	KeyCode::m_knControl = VK_CONTROL;
	KeyCode::m_knShift = VK_SHIFT;
#endif
}
