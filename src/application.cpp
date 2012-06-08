// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma comment(lib, "gd204.lib")
#pragma warning(disable: 4786)


#include "application.h"

// TODO Figure out what to do with resources.
#include "resource.h"

#include "console.h"
#include "entity_type.h"
#include "exception.h"
#include "file.h"
#include "game.h"
#include "image.h"
#include "key.h"
#include "menu.h"
#include "render.h"
#include "resourcex.h"
#include "screen.h"
#include "shell.h"
#include "state_game.h"
#include "state_editor.h"
#include "state_splash.h"
#include "stylus.h"
#include "test.h"
#include "variable.h"
#include "view.h"
#include "world.h"


Application*
Application::m_pApplication;

ApplicationState*
Application::m_pApplicationState;

int
Application::m_nFrameNumber = 0;


// TODO This needs a proper home but refactor first.
void
Application::setActiveEntity(const Vec2& kvPoint)
{
	//Stylus::setLeadPoint(kvPoint);

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


/*******************************************************************************
	Entry point to the program. Initializes everything, then goes into a
	message processing loop. Exit the loop by calling Application::Shutdown().
*******************************************************************************/
int APIENTRY
WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInst,
	LPTSTR pCmdLine,
	int nCmdShow)
{
	return Application::initAndRun(hInstance);
}


/*******************************************************************************
*******************************************************************************/
HRESULT
Application::initAndRun(
	HINSTANCE hInstance)
{
	// Initialization.
	Variable::init();
	Resourcex::init();
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

	config.hInstance					= hInstance;
	config.pAppTitle					= _T("Flatland");
	config.dwWindowIcon					= IDI_APP;
	config.dwDisplayMode				= GDDISPMODE_NORMAL;
	config.dwTargetFPS					= 30;
	config.dwDisplayFlags				= GDDISPLAY_WINDOW;
	config.dwWindowBorder				= WS_DLGFRAME;
	config.dwDisplayWidth				= 240;
	config.dwDisplayHeight				= 320;

	// Create the application.
	Console::print(_T("Creating application.\n"));
	Application::m_pApplication = new Application(config);

	Console::print(_T("Flatland initialized.\n"));

	// TEMP
	StateSplash::changeState();

	// Start main loop.
	return m_pApplication->Run();
}


/*******************************************************************************
*******************************************************************************/
void
Application::shutdown()
{
	m_pApplication->Shutdown();
}


/*******************************************************************************
*******************************************************************************/
Application::Application(
	const GDAPPCONFIG& config) :
	CGapiApplication(config)
{
}


/*******************************************************************************
*******************************************************************************/
Application::~Application()
{
}


/*******************************************************************************
*******************************************************************************/
HRESULT
Application::InitInstance()
{
	return S_OK;
}


/*******************************************************************************
*******************************************************************************/
HRESULT
Application::ExitInstance()
{
	return S_OK;
}


/*******************************************************************************
*******************************************************************************/
HRESULT
Application::CreateSurfaces(
	CGapiDisplay& display,
	HINSTANCE hInstance)
{
	Screen::setSystemFont(display.GetSystemFont());

#ifndef FLATLAND_ENABLE_CUSTOM_LINE_CLIPPING
	m_backbuffer.SetClipper(NULL);
#endif
	Screen::setBackBuffer(&m_backbuffer);

	return S_OK;
}


/*******************************************************************************
*******************************************************************************/
HRESULT
Application::ProcessNextFrame(
	CGapiSurface& backBuffer,
	DWORD dwFlags)
{
	m_pApplicationState->processFrame();

	// Menu handling.
	if (Menu::hasCurrentMenu())
	{
		Screen::drawMenu();
	}

	++m_nFrameNumber;

	return S_OK;
}


/*******************************************************************************
*******************************************************************************/
HRESULT
Application::KeyDown(
	DWORD dwKey,
	GDKEYLIST& keylist)
{
	// TODO this code is OK but key repeat is not supported.
	// I suppose instead I should maintain key state and check also for
	// key up events? YES: see collision mask demo
	// Update: key state event is in Key module, but where do I process keys?
	// Update: some keys (e.g. menu) are immediate, some add cmds for later

#ifdef _WIN32_WCE
	// Pocket PC key bindings.
	const int knControlKey = keylist.vkA;
	const int knMenuKey = keylist.vkC;
#else
	// PC key bindings.
	const int knControlKey = VK_CONTROL;
	const int knMenuKey = VK_ESCAPE;
#endif

	if (dwKey == knControlKey)
	{
		Key::keyDown(Key::m_knKeyA);
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
		Key::keyDown(Key::m_knKeyB);
		//Game::runFrame();
	}
	else if (dwKey == knMenuKey)
	{
		Key::keyDown(Key::m_knKeyC);
		// Menu key.
		if (StateGame::isState())
		{
			if (Menu::hasCurrentMenu())
			{
				Menu::clearCurrentMenu();
			}
			else
			{
				Menu::setCurrentMenu(Menu::game_main);
				Menu::setCurrentItem(0);
			}
			Game::setPaused(!Game::isPaused());
		}
		else if (StateEditor::isState())
		{
			if (Menu::hasCurrentMenu())
			{
				Menu::clearCurrentMenu();
			}
			else
			{
				Menu::setCurrentMenu(Menu::editor);
				Menu::setCurrentItem(0);
			}
		}
	}
	else if (dwKey == keylist.vkStart)
	{
		Key::keyDown(Key::m_knKeyStart);
		if (Menu::hasCurrentMenu())
		{
			Menu::getCurrentMenu().getItem(Menu::getCurrentItem()).doCommand();
		}

		if (StateGame::isState())
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
	else
	{
		Console::print(_T("Unknown key code: %d\n"), dwKey);
	}

	return S_OK;
}


/*******************************************************************************
*******************************************************************************/
HRESULT
Application::KeyUp(
	DWORD dwKey,
	GDKEYLIST& keylist)
{
#ifdef _WIN32_WCE
	// Pocket PC key bindings.
	const int knControlKey = keylist.vkA;
	const int knMenuKey = keylist.vkC;
#else
	// PC key bindings.
	const int knControlKey = VK_CONTROL;
	const int knMenuKey = VK_ESCAPE;
#endif

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
	else if (dwKey == knControlKey)
	{
		Key::keyUp(Key::m_knKeyA);
	}
	else if (dwKey == keylist.vkB)
	{
		Key::keyUp(Key::m_knKeyB);
	}
	else if (dwKey == knMenuKey)
	{
		Key::keyUp(Key::m_knKeyC);
	}
	else if (dwKey == keylist.vkStart)
	{
		Key::keyUp(Key::m_knKeyStart);
	}

	return S_OK;
}


/*******************************************************************************
*******************************************************************************/
HRESULT
Application::StylusDblClk(
	POINT p)
{
	const Vec2 kvScreenPoint(p.x, p.y);
	Stylus::doubleClick(kvScreenPoint);

	m_pApplicationState->stylusDoubleClick(kvScreenPoint);

	return S_OK;
}


/*******************************************************************************
*******************************************************************************/
HRESULT
Application::StylusDown(
	POINT p)
{
	const Vec2 kvScreenPoint(p.x, p.y);
	Stylus::down(kvScreenPoint);

	// TODO Do this only during game play
	setActiveEntity(kvScreenPoint);

	// Menu handling.
	if (Menu::hasCurrentMenu())
	{
		Menu::setCurrentItem(getMenuItemIndex(kvScreenPoint));
	}
	else
	{
		m_pApplicationState->stylusDown(kvScreenPoint);
	}

	return S_OK;
}


/*******************************************************************************
*******************************************************************************/
HRESULT
Application::StylusMove(
	POINT p)
{
	const Vec2 kvScreenPoint(p.x, p.y);
	Stylus::move(kvScreenPoint);

	// Menu handling.
	if (Menu::hasCurrentMenu() && Stylus::isDown())
	{
		Menu::setCurrentItem(getMenuItemIndex(kvScreenPoint));
	}
	else
	{
		m_pApplicationState->stylusMove(kvScreenPoint);
	}

	return S_OK;
}


/*******************************************************************************
*******************************************************************************/
HRESULT
Application::StylusUp(
	POINT p)
{
	const Vec2 kvScreenPoint(p.x, p.y);
	Stylus::up(kvScreenPoint);

	// TODO move this into game state
	if (Game::getActiveEntity())
	{
		// This is so forces don't act on a thrown player entity for a few
		// frames, to allow them to escape.
		Game::getActiveEntity()->m_nForceFrame =
			Application::getActualFrameNumber();

		Game::setActiveEntity(0);
	}

	// Menu handling.
	if (Menu::hasCurrentMenu())
	{
		const int knMenuItemIndex = getMenuItemIndex(kvScreenPoint);
		if (knMenuItemIndex != -1)
		{
			Menu::getCurrentMenu().getItem(knMenuItemIndex).doCommand();
		}
	}
	else
	{
		m_pApplicationState->stylusUp(kvScreenPoint);
	}

	return S_OK;
}
