#ifndef STRICT
#define STRICT
#endif


// If I don't include this here, it won't compile in VC++ 6.
// Must be something in MyApplication.h?
#include <vector>

#include "MyApplication.h"
#include "resource.h"

#include "console.h"
#include "exception.h"
#include "file.h"
#include "game.h"
#include "key.h"
#include "render.h"
#include "resourcex.h"
#include "screen.h"
#include "shell.h"
#include "stylus.h"
#include "test.h"
#include "variable.h"
#include "view.h"
#include "world.h"


#pragma comment(lib, "gd204.lib")


void
doConfigFile()
{
	// Feed the config file to the shell interpreter.
	FILE* pFile = File::openFile(_T("flatland-cfg.txt"), _T("r"));
	if (!pFile)
	{
		return;
	}

	_TCHAR line[256];

	while (_fgetts(
		line,
		256,
		pFile))
	{
		Shell::doCommand(line);
	}

	File::closeFile(pFile);
}


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and goes into a
//       message-processing loop. Exit the loop by calling Shutdown().
//-----------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR pCmdLine, int nCmdShow)
{
	// Initialization.
	Variable::init();
	Resourcex::init();
	Console::init();
	Render::init();
	View::init();

#ifdef FLATLAND_ENABLE_TEST
	Console::print(_T("Performing automated tests.\n"));
	Test::test();
#endif

	// Load and execute the config file here, for now.
	doConfigFile();

	GDAPPCONFIG config;
	::ZeroMemory(&config, sizeof(GDAPPCONFIG));

	config.hInstance           = hInst;
	config.pAppTitle           = _T("Flatland");
	config.dwWindowIcon        = IDI_APP;
	config.dwDisplayMode       = GDDISPMODE_NORMAL;
	config.dwTargetFPS         = 30;
	config.dwDisplayFlags      = GDDISPLAY_WINDOW;
	config.dwWindowBorder      = WS_DLGFRAME;
	config.dwDisplayWidth      = 240;
	config.dwDisplayHeight     = 320;

	// Create the application
	Console::print(_T("Creating application.\n"));
	CMyApplication app(config);

	Console::print(_T("Flatland initialized.\n"));

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
	Screen::setSystemFont(display.GetSystemFont());

#if 0
	// Load images here for now, because we need to have a primary.
	Game::getEntity(0).getModel().setImage(
		Resourcex::loadImage(Game::getEntity(0).getModel().getName().c_str()));
	Game::getEntity(1).getModel().setImage(
		Resourcex::loadImage(Game::getEntity(1).getModel().getName().c_str()));
#endif

	return S_OK;
}

HRESULT DrawFrameInfo(CGapiSurface& surface, DWORD dwY, const TCHAR* pText, COLORREF color)
{
	DWORD strX = 30;
	DWORD strWidth;
	RECT rect;

	GDFILLRECTFX fillrectfx;
	fillrectfx.dwOpacity = 128;

	Screen::getSystemFont()->GetStringWidth(pText, &strWidth);
	::SetRect(&rect, 0, dwY-1, strX+strWidth+3, dwY+8);

	surface.DrawLine(0, dwY-2, strX+strWidth+3, dwY-2, RGB(0, 0, 0), 0, NULL);
	surface.DrawLine(strX+strWidth+3, dwY-2, strX+strWidth+3, dwY+8, RGB(0, 0, 0), 0, NULL);
	surface.DrawLine(0, dwY+8, strX+strWidth+3, dwY+8, RGB(0, 0, 0), 0, NULL);
	surface.FillRect(&rect, color, GDFILLRECT_OPACITY, &fillrectfx);
	surface.DrawText(strX, dwY, pText, Screen::getSystemFont(),
		0, NULL, 0, NULL);

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
	static bool bFirstTime = true;
	if (bFirstTime)
	{
		bFirstTime = false;
		Console::print(_T("Loading level\n"));
		Resourcex::loadLevel(Variable::first_level.getValue());
	}

#ifndef FLATLAND_ENABLE_CUSTOM_LINE_CLIPPING
	backbuffer.SetClipper(NULL);
#endif
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
	// Blast world to screen.
	// Screen origin and size.
	Vec2 vScreenOrigin = View::getScreenView().getMin();
	Vec2 vScreenViewSize =
		View::getScreenView().getMax() - View::getScreenView().getMin();
	// Set the source rect to the view in image coordinates.
	RECT srcRect;
	srcRect.top =
		Game::getEntity(0).getBounds().getMax()[1] -
		View::getWorldView().getMax()[1];
	srcRect.left =
		View::getWorldView().getMin()[0] -
		Game::getEntity(0).getBounds().getMin()[0];
	srcRect.bottom = srcRect.top + vScreenViewSize[1];
	srcRect.right = srcRect.left + vScreenViewSize[0];
	// Clip the source rect to the image.
	if (srcRect.top < 0)
	{
		vScreenOrigin[1] -= srcRect.top;
		srcRect.top = 0;
	}
	if (srcRect.left < 0)
	{
		vScreenOrigin[0] -= srcRect.left;
		srcRect.left = 0;
	}
	if (m_world.GetHeight() < srcRect.bottom)
	{
		srcRect.bottom = m_world.GetHeight();
	}
	if (m_world.GetWidth() < srcRect.right)
	{
		srcRect.right = m_world.GetWidth();
	}
	backbuffer.BltFast(
		vScreenOrigin[0],
		vScreenOrigin[1],
		&m_world,
		&srcRect,
		0,
		NULL);
#endif
#if 0
	// Blast circle to screen.
//	BltFast( 
//LONG destX, LONG destY, 
//CGapiSurface* pSrcSurface, RECT* pSrcRect, 
//DWORD dwFlags, GDBLTFASTFX* pGDBltFastFx); 
	backbuffer.BltFast(200, 200, &m_circle, NULL, 0, NULL);
#endif

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
		Game::setPaused(!Game::isPaused());
	}
	else if (dwKey == keylist.vkB)
	{
		Game::runFrame();
	}
	else if (dwKey == keylist.vkC)
	{
		Shutdown();
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
	if (dwKey == keylist.vkUp)
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
