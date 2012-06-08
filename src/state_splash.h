// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__STATE_SPLASH
#define GUARD__STATE_SPLASH


#include "application.h"


/*******************************************************************************
*******************************************************************************/
class StateSplash :
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
	StateSplash m_state;


};


/*******************************************************************************
*******************************************************************************/
inline
void
StateSplash::changeState()
{
	ApplicationState::changeState(m_state);
}


/*******************************************************************************
*******************************************************************************/
inline
bool
StateSplash::isState()
{
	return ApplicationState::isState(m_state);
}


#endif // GUARD
