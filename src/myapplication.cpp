#ifndef STRICT
#define STRICT
#endif


// If I don't include this here, it won't compile in VC++ 6.
// Must be something in MyApplication.h?
#include <vector>

#include "MyApplication.h"
#include "resource.h"

#include "console.h"
#include "game.h"
#include "key.h"
#include "screen.h"
#include "stylus.h"
#include "test.h"
#include "view.h"
#include "world.h"

// TEST
#include "matrix.h"


#pragma comment(lib, "gd201b.lib")


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and goes into a
//       message-processing loop. Exit the loop by calling Shutdown().
//-----------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR pCmdLine, int nCmdShow)
{
	GDAPPCONFIG config;
	::ZeroMemory(&config, sizeof(GDAPPCONFIG));

	config.hInstance           = hInst;
	config.pAppTitle           = TEXT("Flatland");
	config.dwWindowIcon        = IDI_APP;
	config.dwDisplayMode       = GDDISPMODE_NORMAL;
	config.dwTargetFPS         = 90;
	config.dwDisplayFlags      = GDDISPLAY_WINDOW;
	config.dwWindowBorder      = WS_DLGFRAME;
	config.dwDisplayWidth      = 240;
	config.dwDisplayHeight     = 320;

	// Create the application
	CMyApplication app(config);

	// Initialization.
	Console::init();

	// These are not unit tests, they just set up a temporary world.
	Game::makeTestWorld();
	View::makeTestView();

#ifdef FLATLAND_ENABLE_TEST
	Test::test();
#endif

	// Start main loop
	return app.Run();
}

CMyApplication::CMyApplication(const GDAPPCONFIG& config) : CGapiApplication(config)
{
}

CMyApplication::~CMyApplication()
{
}

HRESULT CMyApplication::InitInstance()
{
	return S_OK;
}

HRESULT CMyApplication::ExitInstance()
{
	return S_OK;
}

HRESULT CMyApplication::CreateSurfaces(CGapiDisplay& display, HINSTANCE hInstance)
{
	return S_OK;
}

HRESULT DrawFrameInfo(CGapiSurface& surface, DWORD dwY, const TCHAR* pText, COLORREF color)
{
	DWORD strX = 30;
	DWORD strWidth;
	RECT rect;

	GDFILLRECTFX fillrectfx;
	fillrectfx.dwOpacity = 128;

	surface.DrawText(0, 0, pText, GDDRAWTEXT_CALCWIDTH, &strWidth);
	::SetRect(&rect, 0, dwY-1, strX+strWidth+3, dwY+8);

	surface.DrawLine(0, dwY-2, strX+strWidth+3, dwY-2, RGB(0, 0, 0), 0, NULL);
	surface.DrawLine(strX+strWidth+3, dwY-2, strX+strWidth+3, dwY+8, RGB(0, 0, 0), 0, NULL);
	surface.DrawLine(0, dwY+8, strX+strWidth+3, dwY+8, RGB(0, 0, 0), 0, NULL);
	surface.FillRect(&rect, color, GDFILLRECT_OPACITY, &fillrectfx);
	surface.DrawText(strX, dwY, pText, GDDRAWTEXT_BORDER, NULL);

	return S_OK;
}


/*******************************************************************************
	Analogous to Quake 2's Qcommon_Frame.
*******************************************************************************/
HRESULT
CMyApplication::ProcessNextFrame(
	CGapiSurface& backbuffer,
	DWORD dwFlags)
{
	Screen::setBackBuffer(&backbuffer);

	// Move the active entity via the stylus. This needs to be before runFrame
	// so the active entity doesn't lag behind the stylus.
	if (Game::getActiveEntity())
	{
		// The active entity's velocity is set to place it exactly where the
		// stylus is.
		Game::getActiveEntity()->getVelocity() =
			Vec2(
				Stylus::getLeadPoint()[0] - Stylus::getAnchorPoint()[0],
				-(Stylus::getLeadPoint()[1] - Stylus::getAnchorPoint()[1]));
	}
	// Reset the anchor point to the lead point for the next frame.
	Stylus::setAnchorPoint(Stylus::getLeadPoint());

	// The following are analogous to Quake 2's SV_Frame.

	if (!Game::isPaused())
	{
		Game::runFrame();
	}

	// The following are analogous to Quake 2's CL_Frame.

	// I need this to process keys that repeat, like +forward and -forward,
	// until I find out where Quake 2 does it.
	Key::processCommands();

	// For debugging
	if (Game::isFast())
	{
		m_config.dwTargetFPS = 90;
		Screen::clear();
		Screen::drawCenterText();
	}
	else
	{
		m_config.dwTargetFPS = 30;
		Screen::updateScreen();
	}

#if 0
	// Preliminary performance testing code
	LARGE_INTEGER ticksPerSecond;
	LARGE_INTEGER tick1;
	LARGE_INTEGER tick2;
	LARGE_INTEGER time;

	QueryPerformanceFrequency(&ticksPerSecond);
	QueryPerformanceCounter(&tick1);

	// Insert code to time here.

	QueryPerformanceCounter(&tick2);

	time.QuadPart = tick2.QuadPart - tick1.QuadPart;

	TCHAR str[128];

	_stprintf(str, TEXT("%d ticks"), time.QuadPart);
	DrawFrameInfo(backbuffer, 320 - 40, str, RGB(170, 90, 60));

	_stprintf(str, TEXT("%.3f seconds"), (double)time.QuadPart / (double)ticksPerSecond.QuadPart);
	DrawFrameInfo(backbuffer, 320 - 20, str, RGB(90, 60, 170));
#endif

#if 1
	// Frame rate etc.
	TCHAR str[128];
	FLOAT nActualFrameTime = 0;
	m_timer.GetActualFrameTime(&nActualFrameTime);
	_stprintf(str, TEXT("Current frame: [%.3f ms]"), nActualFrameTime);
	DrawFrameInfo(backbuffer, 320 - 40, str, RGB(170, 90, 60));
	FLOAT nFrameRate = 0;
	m_timer.GetActualFrameRate(&nFrameRate);
	_stprintf(str, TEXT("Current framerate: [%.3f fps]"), nFrameRate);
	DrawFrameInfo(backbuffer, 320 - 20, str, RGB(90, 60, 170));
#endif

	return S_OK;
}

HRESULT CMyApplication::KeyDown(DWORD dwKey, GDKEYLIST& keylist)
{
	// TODO this code is OK but key repeat is not supported.
	// I suppose instead I should maintain key state and check also for
	// key up events? YES: see collision mask demo
	// Update: key state event is in Key module, but where do I process keys?
	// Update: some keys (e.g. menu) are immediate, some add cmds for later

	if (dwKey == keylist.vkA)
	{
		Shutdown();
	}
	else if (dwKey == keylist.vkB)
	{
		Game::setPaused(!Game::isPaused());
	}
	else if (dwKey == keylist.vkC)
	{
		Game::runFrame();
	}
	else if (dwKey == keylist.vkUp)
	{
		Key::keyDown(Key::m_knKeyUp);
	}
	else if (dwKey == keylist.vkDown)
	{
		Key::keyDown(Key::m_knKeyDown);
	}
	else if (dwKey == keylist.vkLeft)
	{
		Key::keyDown(Key::m_knKeyLeft);
	}
	else if (dwKey == keylist.vkRight)
	{
		Key::keyDown(Key::m_knKeyRight);
	}

	return S_OK;
}

HRESULT CMyApplication::KeyUp(DWORD dwKey, GDKEYLIST& keylist)
{
	if (dwKey == keylist.vkA)
	{
		Shutdown();
	}
	else if (dwKey == keylist.vkUp)
	{
		Key::keyUp(Key::m_knKeyUp);
	}
	else if (dwKey == keylist.vkDown)
	{
		Key::keyUp(Key::m_knKeyDown);
	}
	else if (dwKey == keylist.vkLeft)
	{
		Key::keyUp(Key::m_knKeyLeft);
	}
	else if (dwKey == keylist.vkRight)
	{
		Key::keyUp(Key::m_knKeyRight);
	}

	return S_OK;
}


HRESULT CMyApplication::StylusDown(POINT p)
{
	const Vec2 kvPoint(p.x, p.y);
	Stylus::setAnchorPoint(kvPoint);
	Stylus::setLeadPoint(kvPoint);

	std::vector<Entity*> cpEntity =
		World::getEntitiesAtPoint(
			dehomo(View::getTransformS2W() * homo(kvPoint)));

	if (!cpEntity.empty())
	{
		// TODO Check if it's the world? If it's immobile?
		Game::setActiveEntity(cpEntity.back());
	}

	return S_OK;
}

HRESULT CMyApplication::StylusUp(POINT p)
{
	const Vec2 kvPoint(p.x, p.y);

	Game::setActiveEntity(0);

	return S_OK;
}

HRESULT CMyApplication::StylusDblClk(POINT p)
{
	return S_OK;
}

HRESULT CMyApplication::StylusMove(POINT p)
{
	const Vec2 kvPoint(p.x, p.y);
	Stylus::setLeadPoint(kvPoint);

	return S_OK;
}
