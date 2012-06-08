// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__STATE_EDITOR
#define GUARD__STATE_EDITOR


#include "application.h"


/*******************************************************************************
*******************************************************************************/
class StateEditor :
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

	virtual
	void
	stylusDown(
		const Vec2& kvScreenPoint);

	virtual
	void
	stylusMove(
		const Vec2& kvScreenPoint);

	virtual
	void
	stylusUp(
		const Vec2& kvScreenPoint);


private:

	static
	StateEditor m_state;


};


/*******************************************************************************
*******************************************************************************/
inline
void
StateEditor::changeState()
{
	ApplicationState::changeState(m_state);
}


/*******************************************************************************
*******************************************************************************/
inline
bool
StateEditor::isState()
{
	return ApplicationState::isState(m_state);
}


#endif // GUARD
