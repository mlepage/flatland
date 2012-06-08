// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__STATE_GAME
#define GUARD__STATE_GAME


#include "application.h"


/*******************************************************************************
*******************************************************************************/
class StateGame :
	public ApplicationState
{

public:

	static
	void
	changeState();

	static
	bool
	isState();


public:

	virtual
	void
	enterState();

	virtual
	void
	processFrame();


private:

	static
	StateGame m_state;


};


/*******************************************************************************
*******************************************************************************/
inline
void
StateGame::changeState()
{
	ApplicationState::changeState(m_state);
}


/*******************************************************************************
*******************************************************************************/
inline
bool
StateGame::isState()
{
	return ApplicationState::isState(m_state);
}


#endif // GUARD
