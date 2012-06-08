// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__STATE_NAG
#define GUARD__STATE_NAG


#include "application.h"


/*******************************************************************************
*******************************************************************************/
class StateNag :
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
	StateNag m_state;


};


/*******************************************************************************
*******************************************************************************/
inline
void
StateNag::changeState()
{
	ApplicationState::changeState(m_state);
}


/*******************************************************************************
*******************************************************************************/
inline
bool
StateNag::isState()
{
	return ApplicationState::isState(m_state);
}


#endif // GUARD
