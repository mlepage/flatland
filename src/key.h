// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__KEY
#define GUARD__KEY


#include <vector>


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
		const KeyCode knKeyCode);

	static
	void
	keyDown(
		const KeyCode knKeyCode);

	static
	void
	keyUp(
		const KeyCode knKeyCode);

	static
	void
	processCommands();


private:

	static
	std::vector<bool> m_cbKeyState;

};


#endif // GUARD
