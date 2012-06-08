// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "key.h"

#include "game.h"
#include "view.h"


std::vector<bool>
Key::m_cbKeyState(8);


/*******************************************************************************
*******************************************************************************/
bool
Key::isKeyDown(
	const KeyCode knKeyCode)
{
	return m_cbKeyState[knKeyCode];
}


/*******************************************************************************
*******************************************************************************/
void
Key::keyDown(
	const KeyCode knKeyCode)
{
	m_cbKeyState[knKeyCode] = true;
}


/*******************************************************************************
*******************************************************************************/
void
Key::keyUp(
	const KeyCode knKeyCode)
{
	m_cbKeyState[knKeyCode] = false;
}


/*******************************************************************************
*******************************************************************************/
void
Key::processCommands()
{
	if (Game::isPaused())
	{
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
