// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__KEY
#define GUARD__KEY


#include <vector>
#include "GapiDraw.h"


/*******************************************************************************
*******************************************************************************/
class Key
{


public:

	enum KeyCode
	{
		m_knKeyUp = 0,
		m_knKeyDown = 1,
		m_knKeyLeft = 2,
		m_knKeyRight = 3,
		m_knKeyStart = 4,
		m_knKeyA = 5,
		m_knKeyB = 6,
		m_knKeyC = 7
	};

	static
	bool
	isKeyDown(
		const int knKeyCode)
	{
		return m_cbKeyDown[knKeyCode];
	}

	static
	void
	keyDown(
		const int knKeyCode)
	{
		m_cbKeyDown[knKeyCode] = true;
	}

	static
	void
	keyUp(
		const int knKeyCode)
	{
		m_cbKeyDown[knKeyCode] = false;
	}

	static
	void
	processCommands();


private:

	static
	std::vector<bool> m_cbKeyDown;


};


/*******************************************************************************
*******************************************************************************/
class KeyCode
{

public:

	static
	int
	control()
	{
		return m_knControl;
	}

	static
	int
	down()
	{
		return m_knDown;
	}

	static
	int
	enter()
	{
		return m_knEnter;
	}

	static
	int
	left()
	{
		return m_knLeft;
	}

	static
	int
	menu()
	{
		return m_knMenu;
	}

	static
	void
	restoreDefaults(
		GDKEYLIST& keyList);

	static
	int
	right()
	{
		return m_knRight;
	}

	static
	int
	screenShot()
	{
		return m_knScreenShot;
	}

	static
	int
	shift()
	{
		return m_knShift;
	}

	static
	int
	up()
	{
		return m_knUp;
	}


private:

	static int m_knEnter;
	static int m_knUp;
	static int m_knDown;
	static int m_knLeft;
	static int m_knRight;
	static int m_knMenu;
	static int m_knScreenShot;
	static int m_knControl;
	static int m_knShift;


};


#endif // GUARD
