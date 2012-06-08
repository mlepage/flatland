// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__PROGRAM_STATE
#define GUARD__PROGRAM_STATE


/*******************************************************************************
	Program state.

	Key question: handle menu as separate states, or overlay it onto existing
	states?
*******************************************************************************/
class ProgramState
{

public:

	class Editor;
	class GameRunning;
	class GameStart;
	class Splash;
	class Title;
	class Quit;


public:

	static
	int
	getCurrentFrame();

	static
	ProgramState&
	getCurrentState();

	static
	void
	incrementCurrentFrame();

	static
	void
	init();

	static
	void
	setCurrentState(
		ProgramState& programState);


public:

	static
	Editor editor;
	static
	GameRunning game_running;
	static
	GameStart game_start;
	static
	Splash splash;
	static
	Title title;
	static
	Quit quit;


public:

	// TODO need virtual destructor?

	virtual
	void
	enterState();

	virtual
	void
	exitState();

	virtual
	void
	processFrame() = 0;


private:

	static
	ProgramState* m_pCurrentState;

	static
	int m_nCurrentFrame;


};


/*******************************************************************************
*******************************************************************************/
inline
void
ProgramState::enterState()
{
}


/*******************************************************************************
*******************************************************************************/
inline
void
ProgramState::exitState()
{
}


/*******************************************************************************
*******************************************************************************/
inline
int
ProgramState::getCurrentFrame()
{
	return m_nCurrentFrame;
}


/*******************************************************************************
*******************************************************************************/
inline
ProgramState&
ProgramState::getCurrentState()
{
	return *m_pCurrentState;
}


/*******************************************************************************
	This only needs to be public so I can call it from elsewhere. It saves me
	from having to handle all the default behaviour in this class, when I
	already have default functions elsewhere.
*******************************************************************************/
inline
void
ProgramState::incrementCurrentFrame()
{
	++m_nCurrentFrame;
}


/*******************************************************************************
*******************************************************************************/
inline
void
ProgramState::setCurrentState(
	ProgramState& programState)
{
	if (m_pCurrentState)
	{
		m_pCurrentState->exitState();
	}

	m_pCurrentState = &programState;

	m_pCurrentState->enterState();
}


/*******************************************************************************
*******************************************************************************/
class ProgramState::Editor : public ProgramState
{

public:

	virtual void processFrame();


};


/*******************************************************************************
*******************************************************************************/
class ProgramState::GameRunning : public ProgramState
{

public:

	virtual void processFrame();


};


/*******************************************************************************
*******************************************************************************/
class ProgramState::GameStart : public ProgramState
{

public:

	virtual void processFrame();


};


/*******************************************************************************
*******************************************************************************/
class ProgramState::Splash : public ProgramState
{

public:

	virtual void processFrame();


};


/*******************************************************************************
*******************************************************************************/
class ProgramState::Title : public ProgramState
{

public:

	virtual void processFrame();


};


/*******************************************************************************
*******************************************************************************/
class ProgramState::Quit : public ProgramState
{

public:

	virtual void processFrame();


};


/*******************************************************************************
	Moved this here for inlining.
*******************************************************************************/
inline
void
ProgramState::init()
{
	setCurrentState(ProgramState::splash);
}


#endif // GUARD
