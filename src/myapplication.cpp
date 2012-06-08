#ifndef STRICT
#define STRICT
#endif


#pragma comment(lib, "gd204.lib")
#pragma warning(disable: 4786)


// If I don't include this here, it won't compile in VC++ 6.
// Must be something in MyApplication.h?
#include <vector>

#include "MyApplication.h"
#include "resource.h"

#include "console.h"
#include "entity_type.h"
#include "exception.h"
#include "file.h"
#include "game.h"
#include "image.h"
#include "key.h"
#include "menu.h"
#include "program_state.h"
#include "render.h"
#include "resourcex.h"
#include "screen.h"
#include "shell.h"
#include "stylus.h"
#include "test.h"
#include "variable.h"
#include "view.h"
#include "world.h"


CMyApplication*
CMyApplication::m_pMyApplication;


// TODO this needs a home but refactor first
// (it is currently in both ProgramState and CMyApplication
static
void
setActiveEntity(const Vec2& kvPoint)
{
	Stylus::setLeadPoint(kvPoint);

	std::vector<Entity*> cpEntity =
		World::getEntitiesAtPoint(
			dehomo(View::getTransformS2W() * homo(kvPoint)));

	// Go through the entities. If we are in a control entity, and also a
	// player entity, then make that entity the active entity.

	Entity* pPlayerEntity = NULL;
	bool bControlEntity = false;

	for (int n = 0; n != cpEntity.size(); ++n)
	{
		// TODO use static variables to lookup entity type?
		if (cpEntity[n]->getType().getName() == tstring(_T("control")))
		{
			bControlEntity = true;
		}
		else if (cpEntity[n]->getType().getName() == tstring(_T("player")))
		{
			pPlayerEntity = cpEntity[n];
		}
	}

	if (bControlEntity && pPlayerEntity)
	{
		Game::setActiveEntity(pPlayerEntity);
		// Set autoscroll entity.
		Game::setAutoscrollEntity(pPlayerEntity);
	}
}


static
int
getMenuItemIndex(
	const Vec2& kvPoint)
{
	for (int n = 0; n != Menu::getCurrentMenu().getNumberOfItems(); ++n)
	{
		const Menu::Item& kItem = Menu::getCurrentMenu().getItem(n);
		const Rect krBounds = kItem.getBounds();
		if (Geometry::isPointInRect(kvPoint, krBounds))
		{
			return n;
		}
	}

	return -1;
}


// TODO this needs a home but refactor first
static
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
	ProgramState::init();
	Console::init();
	Render::init();
	View::init();

	EntityType::init();
	Menu::init();

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
	CMyApplication::m_pMyApplication = &app;

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

#ifndef FLATLAND_ENABLE_CUSTOM_LINE_CLIPPING
	backbuffer.SetClipper(NULL);
#endif
	Screen::setBackBuffer(&m_backbuffer);

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
	ProgramState::getCurrentState().processFrame();
	ProgramState::incrementCurrentFrame();

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
		if (Variable::render_entity_image.getFloatValue())
		{
			Variable::render_brush_bounds.setValue(_T("1"));
			Variable::render_brush_outline.setValue(_T("1"));
			Variable::render_entity_bounds.setValue(_T("1"));
			Variable::render_entity_identifier.setValue(_T("1"));
			Variable::render_entity_velocity.setValue(_T("1"));
			Variable::render_entity_image.setValue(_T("0"));
			Variable::render_model_bounds.setValue(_T("1"));
		}
		else
		{
			Variable::render_brush_bounds.setValue(_T("0"));
			Variable::render_brush_outline.setValue(_T("0"));
			Variable::render_entity_bounds.setValue(_T("0"));
			Variable::render_entity_identifier.setValue(_T("0"));
			Variable::render_entity_velocity.setValue(_T("0"));
			Variable::render_entity_image.setValue(_T("1"));
			Variable::render_model_bounds.setValue(_T("0"));
		}
	}
	else if (dwKey == keylist.vkB)
	{
		Game::runFrame();
	}
	else if (dwKey == keylist.vkC)
	{
		if (&ProgramState::getCurrentState() == &ProgramState::game_running)
		{
			Game::setPaused(!Game::isPaused());
			if (Game::isPaused())
			{
				// Menu.
				Menu::setCurrentMenu(Menu::game_main);
				Menu::setCurrentItem(0);
			}
			else
			{
				Menu::clearCurrentMenu();
			}
		}
	}
	else if (dwKey == keylist.vkStart)
	{
		if (Menu::hasCurrentMenu())
		{
			Menu::getCurrentMenu().getItem(Menu::getCurrentItem()).doCommand();
		}

		if (&ProgramState::getCurrentState() == &ProgramState::game_running)
		{
			// Toggle autoscroll.
			Game::setAutoscrollEnabled(!Game::isAutoscrollEnabled());
		}
	}
	else if (dwKey == keylist.vkUp)
	{
		Key::keyDown(Key::m_knKeyUp);
		if (Menu::hasCurrentMenu())
		{
			const int knCurrentItem = Menu::getCurrentItem();
			if (knCurrentItem < 1)
			{
				Menu::setCurrentItem(
					Menu::getCurrentMenu().getNumberOfItems() - 1);
			}
			else
			{
				Menu::setCurrentItem(
					knCurrentItem - 1);
			}
		}
	}
	else if (dwKey == keylist.vkDown)
	{
		Key::keyDown(Key::m_knKeyDown);
		if (Menu::hasCurrentMenu())
		{
			const int knCurrentItem = Menu::getCurrentItem();
			if (Menu::getCurrentMenu().getNumberOfItems() - 1 <= knCurrentItem)
			{
				Menu::setCurrentItem(0);
			}
			else
			{
				Menu::setCurrentItem(
					knCurrentItem + 1);
			}
		}
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
	Stylus::down();
	
	const Vec2 kvPoint(p.x, p.y);
	Stylus::setAnchorPoint(kvPoint);
	Stylus::setLeadPoint(kvPoint);

	setActiveEntity(kvPoint);

	if (Menu::hasCurrentMenu())
	{
		Menu::setCurrentItem(getMenuItemIndex(kvPoint));
	}

	return S_OK;
}

HRESULT CMyApplication::StylusUp(POINT p)
{
	Stylus::up();

	const Vec2 kvPoint(p.x, p.y);

	if (Game::getActiveEntity())
	{
		// This is so forces don't act on a thrown player entity for a few
		// frames, to allow them to escape.
		Game::getActiveEntity()->m_nForceFrame =
			ProgramState::getCurrentFrame();

		Game::setActiveEntity(0);
	}

	if (Menu::hasCurrentMenu())
	{
		const int knMenuItemIndex = getMenuItemIndex(kvPoint);
		if (knMenuItemIndex != -1)
		{
			Menu::getCurrentMenu().getItem(knMenuItemIndex).doCommand();
		}
	}

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

	if (Menu::hasCurrentMenu() && Stylus::isDown())
	{
		Menu::setCurrentItem(getMenuItemIndex(kvPoint));
	}

	return S_OK;
}
