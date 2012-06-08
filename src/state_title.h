// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__STATE_TITLE
#define GUARD__STATE_TITLE


#include "application.h"


/*******************************************************************************
*******************************************************************************/
class StateTitle :
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
	processFrame();


private:

	static
	StateTitle m_state;


};


/*******************************************************************************
*******************************************************************************/
inline
void
StateTitle::changeState()
{
	ApplicationState::changeState(m_state);
}


/*******************************************************************************
*******************************************************************************/
inline
bool
StateTitle::isState()
{
	return ApplicationState::isState(m_state);
}


#endif // GUARD
