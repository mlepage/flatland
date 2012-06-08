#ifndef GUARD__APPLICATION
#define GUARD__APPLICATION


#include "GapiApplication.h"
#include "matrix.h"


class ApplicationState;


/*******************************************************************************
	The main class to run this application. Most functionality is inherited
	from the base class.
*******************************************************************************/
class Application :
	public CGapiApplication
{

public:

	static
	int
	getActualFrameNumber();

	static
	float
	getActualFrameRate();

	static
	float
	getActualFrameTime();

	static
	CGapiDisplay&
	getDisplay();

	static
	HRESULT
	initAndRun(
		HINSTANCE hInstance);

	// TODO This needs a proper home. Refactor first.
	static
	void
	setActiveEntity(
		const Vec2& kvPoint);

	// TODO Perhaps this should be obfuscated? Maybe it doesn't matter since
	// I don't believe it can be obfuscated in the base class.
	static
	void
	shutdown();


public:

	Application(const GDAPPCONFIG& config);

	virtual
	~Application();

	virtual HRESULT InitInstance();
	virtual HRESULT ExitInstance();

	virtual HRESULT CreateSurfaces(CGapiDisplay& display, HINSTANCE hInstance);

	virtual HRESULT ProcessNextFrame(CGapiSurface& backBuffer, DWORD dwFlags);

	virtual HRESULT KeyDown(DWORD dwKey, GDKEYLIST& keylist);
	virtual HRESULT KeyUp(DWORD dwKey, GDKEYLIST& keylist);

	virtual HRESULT StylusDblClk(POINT p);
	virtual HRESULT StylusDown(POINT p);
	virtual HRESULT StylusMove(POINT p);
	virtual HRESULT StylusUp(POINT p);


private:

	friend class ApplicationState;


private:

	static
	void
	changeState(
		ApplicationState& state);

	static
	bool
	isState(
		ApplicationState& state);


private:

	static Application* m_pApplication;
	static ApplicationState* m_pApplicationState;

	static int m_nFrameNumber;


};


/*******************************************************************************
	State design pattern.
*******************************************************************************/
class ApplicationState
{

public:

	virtual
	~ApplicationState() = 0;

	virtual
	void
	enterState() {};

	virtual
	void
	exitState() {};

	virtual
	void
	keyDown(
		const int knKeyCode) {};

	virtual
	void
	keyUp(
		const int knKeyCode) {};

	virtual
	void
	processFrame() {};

	virtual
	void
	stylusDoubleClick(
		const Vec2& kvScreenPoint) {};

	virtual
	void
	stylusDown(
		const Vec2& kvScreenPoint) {};

	virtual
	void
	stylusMove(
		const Vec2& kvScreenPoint) {};

	virtual
	void
	stylusUp(
		const Vec2& kvScreenPoint) {};


protected:

	static
	void
	changeState(
		ApplicationState& state);

	static
	bool
	isState(
		ApplicationState& state);


};


/*******************************************************************************
*******************************************************************************/
inline
void
Application::changeState(
	ApplicationState& state)
{
	if (m_pApplicationState)
	{
		m_pApplicationState->exitState();
	}

	m_pApplicationState = &state;

	m_pApplicationState->enterState();
}


/*******************************************************************************
*******************************************************************************/
inline
int
Application::getActualFrameNumber()
{
	return m_nFrameNumber;
}


/*******************************************************************************
*******************************************************************************/
inline
float
Application::getActualFrameRate()
{
	float f;
	m_pApplication->m_timer.GetActualFrameRate(&f);
	return f;
}


/*******************************************************************************
*******************************************************************************/
inline
float
Application::getActualFrameTime()
{
	float f;
	m_pApplication->m_timer.GetActualFrameTime(&f);
	return f;
}


/*******************************************************************************
*******************************************************************************/
inline
CGapiDisplay&
Application::getDisplay()
{
	return m_pApplication->m_display;
}


/*******************************************************************************
*******************************************************************************/
inline
bool
Application::isState(
	ApplicationState& state)
{
	return m_pApplicationState == &state;
}


/*******************************************************************************
*******************************************************************************/
inline
ApplicationState::~ApplicationState()
{
}


/*******************************************************************************
*******************************************************************************/
inline
void
ApplicationState::changeState(
	ApplicationState& state)
{
	Application::changeState(state);
}


/*******************************************************************************
*******************************************************************************/
inline
bool
ApplicationState::isState(
	ApplicationState& state)
{
	return Application::isState(state);
}


#endif // GUARD
