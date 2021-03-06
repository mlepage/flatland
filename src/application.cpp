// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma comment(lib, "gd204.lib")
#pragma warning(disable: 4786)


#include "application.h"

// TODO Figure out what to do with Windows resources.
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
#include "state_nag.h"
#include "state_splash.h"
#include "stylus.h"
#include "test.h"
#include "variable.h"
#include "view.h"
#include "widget.h"
#include "widget_file_dialog.h"
#include "widget_main_window.h"
#include "widget_menu_bar.h"
#include "widget_message_dialog.h"
#include "widget_push_button.h"
#include "widget_tool_bar.h"
#include "widget_tool_button.h"
#include "world.h"
#include "editor/model_editor.h"


// TEST Testing a bunch of widget and signals/slots stuff.
struct SlotTester : public sigslot::has_slots<>
{
	void in() {Console::print(_T("in: %s\n"), m_sName.c_str());};
	void out() {Console::print(_T("out: %s\n"), m_sName.c_str());};
	tstring m_sName;
};
struct Base : public sigslot::has_slots<>
{
	virtual ~Base() {};
	virtual void slot1() {};
	void slot2() {};
	virtual void slot4() {};
};
struct DerivedA : public Base
{
	sigslot::signal0<> sig;
};
struct DerivedB : public Base
{
	virtual void slot1() {};
	void slot3() {};
};


Application*
Application::m_pApplication;

ApplicationState*
Application::m_pApplicationState;

int
Application::m_nFrameNumber = 0;

bool
Application::m_bScrollViewEnabled = false;

WidgetMainWindow*
Application::m_pModelEditorMainWindow;
WidgetFileDialog*
Application::m_pFileDialog;
WidgetMessageDialog*
Application::m_pSaveChangesDialog;


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
WidgetFileDialog&
Application::getFileDialog()
{
	if (!m_pFileDialog)
	{
		m_pFileDialog = new WidgetFileDialog(_T("file_dialog"));
	}

	return *m_pFileDialog;
}


/*******************************************************************************
*******************************************************************************/
WidgetMainWindow&
Application::getModelEditorMainWindow()
{
	if (!m_pModelEditorMainWindow)
	{
		WDim buttonSize(120 - 32, 16);
		int nX = 16;
		int nYDelta = 24;
		int nY = 8 - nYDelta;

		m_pModelEditorMainWindow =
			new WidgetMainWindow(_T("model_editor_main_window"));

		// Contents.
		ModelEditor& modelEditor =
			*new ModelEditor(
				*m_pModelEditorMainWindow,
				_T("model_editor_contents"));
		m_pModelEditorMainWindow->setContents(modelEditor);
		modelEditor.setFocusEnabled(true);

		// Menu bar.
		WidgetMenuBar& menuBar =
			*new WidgetMenuBar(
				*m_pModelEditorMainWindow,
				_T("model_editor_menu_bar"));
		m_pModelEditorMainWindow->setMenuBar(menuBar);
		menuBar.setHeight(menuBar.getHeight() + 32);
		menuBar.setFocusEnabled(true);
		menuBar.hide();
		{
			WidgetPushButton& button =
				*new WidgetPushButton(menuBar, _T("new_model"));
			button.setText(_T("new model"));
			button.setBounds(WRect(WPoint(nX, nY += nYDelta), buttonSize));
			button.clicked.connect(
				&modelEditor, &ModelEditor::doNewModel);
		}
		{
			WidgetPushButton& button =
				*new WidgetPushButton(menuBar, _T("load_model"));
			button.setText(_T("load model"));
			button.setBounds(WRect(WPoint(nX, nY += nYDelta), buttonSize));
			button.clicked.connect(
				&modelEditor, &ModelEditor::doLoadModel);
		}
		{
			WidgetPushButton& button =
				*new WidgetPushButton(menuBar, _T("save_model"));
			button.setText(_T("save model"));
			button.setBounds(WRect(WPoint(nX, nY += nYDelta), buttonSize));
			button.clicked.connect(
				&modelEditor, &ModelEditor::doSaveModel);
		}
		{
			WidgetPushButton& button =
				*new WidgetPushButton(menuBar, _T("save_model_as"));
			button.setText(_T("save model as"));
			button.setBounds(WRect(WPoint(nX, nY += nYDelta), buttonSize));
			button.clicked.connect(
				&modelEditor, &ModelEditor::doSaveModelAs);
		}
		{
			WidgetPushButton& button =
				*new WidgetPushButton(menuBar, _T("compile_model"));
			button.setText(_T("compile model"));
			button.setBounds(WRect(WPoint(nX, nY += nYDelta), buttonSize));
			button.clicked.connect(
				&modelEditor, &ModelEditor::doCompileModel);
		}
		{
			WidgetPushButton& button =
				*new WidgetPushButton(menuBar, _T("load_image"));
			button.setText(_T("load image"));
			button.setBounds(WRect(WPoint(nX, nY += nYDelta), buttonSize));
			button.clicked.connect(
				&modelEditor, &ModelEditor::doLoadImage);
		}
		{
			WidgetPushButton& button =
				*new WidgetPushButton(menuBar, _T("set_image_origin"));
			button.setText(_T("set image origin"));
			button.setBounds(WRect(WPoint(nX, nY += nYDelta), buttonSize));
			button.clicked.connect(
				&modelEditor, &ModelEditor::doSetImageOrigin);
		}
		{
			WidgetPushButton& button =
				*new WidgetPushButton(menuBar, _T("close_editor"));
			button.setText(_T("close editor"));
			button.setBounds(WRect(WPoint(nX, nY += nYDelta), buttonSize));
			button.clicked.connect(
				&modelEditor, &ModelEditor::doClose);
		}
		{
			WidgetPushButton& button =
				*new WidgetPushButton(menuBar, _T("quit"));
			button.setText(_T("quit"));
			button.setBounds(WRect(WPoint(nX, nY += nYDelta), buttonSize));
			button.clicked.connect(
				&modelEditor, &ModelEditor::doQuit);
		}

		// Tool bar.
		WidgetToolBar& toolBar =
			*new WidgetToolBar(
				*m_pModelEditorMainWindow,
				_T("model editor tool bar"));
		m_pModelEditorMainWindow->setToolBar(toolBar);
		{
			WidgetToolButton& toolButton =
				*new WidgetToolButton(toolBar, _T("tool_move"));
			toolButton.setPosition(WPoint(24 * 0, 1));
			toolButton.setIconSet(
				Resourcex::getImage(_T("tool_move")).getSurface());
			toolButton.setToggleButton(true);
			toolButton.toggle();
			toolButton.toggled.connect(
				&modelEditor, &ModelEditor::toolToggle);
		}
		{
			WidgetToolButton& toolButton =
				*new WidgetToolButton(toolBar, _T("tool_create_point"));
			toolButton.setPosition(WPoint(24 * 1, 1));
			toolButton.setIconSet(
				Resourcex::getImage(_T("tool_create_point")).getSurface());
			toolButton.setToggleButton(true);
			toolButton.toggled.connect(
				&modelEditor, &ModelEditor::toolToggle);
		}
		{
			WidgetToolButton& toolButton =
				*new WidgetToolButton(toolBar, _T("tool_create_polygon"));
			toolButton.setPosition(WPoint(24 * 2, 1));
			toolButton.setIconSet(
				Resourcex::getImage(_T("tool_create_polygon")).getSurface());
			toolButton.clicked.connect(
				&modelEditor, &ModelEditor::doCreatePolygonFromSelectedPoints);
		}
		{
			WidgetToolButton& toolButton =
				*new WidgetToolButton(toolBar,
					_T("tool_create_inverse_polygon"));
			toolButton.setPosition(WPoint(24 * 3, 1));
			toolButton.setIconSet(
				Resourcex::getImage(
					_T("tool_create_inverse_polygon")).getSurface());
			toolButton.clicked.connect(
				&modelEditor,
				&ModelEditor::doCreateInversePolygonFromSelectedPoints);
		}
		{
			WidgetToolButton& toolButton =
				*new WidgetToolButton(toolBar, _T("tool_delete"));
			toolButton.setPosition(WPoint(24 * 4, 1));
			toolButton.setIconSet(
				Resourcex::getImage(_T("tool_delete")).getSurface());
			toolButton.clicked.connect(
				&modelEditor, &ModelEditor::doDeleteSelectedPoints);
		}
		{
			WidgetToolButton& toolButton =
				*new WidgetToolButton(toolBar, _T(""));
			toolButton.setPosition(WPoint(24 * 5, 1));
		}
		{
			WidgetToolButton& toolButton =
				*new WidgetToolButton(toolBar, _T(""));
			toolButton.setPosition(WPoint(24 * 6, 1));
		}
		{
			WidgetToolButton& toolButton =
				*new WidgetToolButton(toolBar, _T(""));
			toolButton.setPosition(WPoint(24 * 7, 1));
		}
		{
			WidgetToolButton& toolButton =
				*new WidgetToolButton(toolBar, _T(""));
			toolButton.setPosition(WPoint(24 * 8, 1));
		}
		{
			WidgetToolButton& toolButton =
				*new WidgetToolButton(toolBar, _T(""));
			toolButton.setPosition(WPoint(24 * 9, 1));
		}
	}

	return *m_pModelEditorMainWindow;
}


/*******************************************************************************
*******************************************************************************/
WidgetMessageDialog&
Application::getSaveChangesDialog()
{
	if (!m_pSaveChangesDialog)
	{
		m_pSaveChangesDialog =
			new WidgetMessageDialog(_T("save_changes_dialog"));
		m_pSaveChangesDialog->setText(_T("Save changes?"));
	}

	return *m_pSaveChangesDialog;
}


/*******************************************************************************
*******************************************************************************/
void
Application::makeWidgets()
{
	// TODO This function is no longer necessary. Remove it once I am satisfied
	// that the signal/slot testing is OK.

	// TEST Compilation of sigslot connections.
	DerivedA derivedA;
	DerivedB derivedB;
	derivedA.sig.connect(&derivedB, &DerivedB::slot1);
	//derivedA.sig.connect(&derivedB, &DerivedB::slot2);
	derivedA.sig.connect(&derivedB, &DerivedB::slot3);
	//derivedA.sig.connect(&derivedB, &DerivedB::slot4);
	derivedB.slot2();
}


/*******************************************************************************
*******************************************************************************/
void
Application::quit()
{
	// Hide whatever got us here.
	Menu::clearCurrentMenu();
	getModelEditorMainWindow().hide();

	// Change to nag state.
	StateNag::changeState();
}


/*******************************************************************************
*******************************************************************************/
void
Application::screenShot()
{
	// TODO What if there are over 100 saved screenshots?
	// TODO What if a file error occurs (e.g. directory doesn't exist?

	// First, find a free file name.
	tstring sFileName;
	for (int n = 1; n != 100; ++n)
	{
		sFileName =
			tstring(_T("data\\screenshots\\screenshot ")) +
				toString(n) + tstring(_T(".png"));
		FILE* pFile = File::openFileNoException(sFileName, _T("r"));
		if (!pFile)
		{
			// Second, save the back buffer to that file name.
			m_pApplication->m_backbuffer.SaveSurface(
				File::makeFileName(sFileName).c_str(),
				GDSAVESURFACE_PNG);
			break;
		}
		else
		{
			File::closeFile(pFile);
		}
	}
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

	makeWidgets();

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

	{
		GDKEYLIST keyList;
		m_pApplication->m_input.GetKeyList(&keyList);
		KeyCode::restoreDefaults(keyList);
	}

	Console::print(_T("Flatland initialized.\n"));

	// TEMP Initial state change.
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

	Widget::setBackBuffer(m_backbuffer);

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

	// Widget event dispatching.
	Widget::dispatchEventProcess();
	Widget::dispatchEventPaint();

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

	Key::keyDown(dwKey);

	// No matter what, handle screenshot globally here.
	if (dwKey == KeyCode::screenShot())
	{
		Application::screenShot();
	}

	// Widget event dispatching.
	Widget::dispatchEventKeyPress(dwKey);
	if (!Widget::isOtherEventDispatchingEnabled())
	{
		return S_OK;
	}

#if 1
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
#endif
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

	m_pApplicationState->keyDown(dwKey);
	
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

	Key::keyUp(dwKey);

	// Widget event dispatching.
	Widget::dispatchEventKeyRelease(dwKey);
	if (!Widget::isOtherEventDispatchingEnabled())
	{
		return S_OK;
	}

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

	m_pApplicationState->keyUp(dwKey);

	return S_OK;
}


/*******************************************************************************
*******************************************************************************/
HRESULT
Application::StylusDblClk(
	POINT p)
{
	// Set stylus state first.
	const Vec2 kvScreenPoint(p.x, p.y);
	Stylus::doubleClick(kvScreenPoint);

	// Widget event dispatching.
	Widget::dispatchEventStylusDoubleClick(WPoint(p.x, p.y));
	if (!Widget::isOtherEventDispatchingEnabled())
	{
		return S_OK;
	}

	m_pApplicationState->stylusDoubleClick(kvScreenPoint);

	return S_OK;
}


/*******************************************************************************
*******************************************************************************/
HRESULT
Application::StylusDown(
	POINT p)
{
	// Set stylus state first.
	const Vec2 kvScreenPoint(p.x, p.y);
	Stylus::down(kvScreenPoint);

	// Widget event dispatching.
	Widget::dispatchEventStylusDown(WPoint(p.x, p.y));
	if (!Widget::isOtherEventDispatchingEnabled())
	{
		return S_OK;
	}

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
	// Only do anything if stylus is down.
	if (!Stylus::isDown())
	{
		return S_OK;
	}

	// Set stylus state first.
	const Vec2 kvScreenPoint(p.x, p.y);
	Stylus::move(kvScreenPoint);

	// Widget event dispatching.
	Widget::dispatchEventStylusMove(WPoint(p.x, p.y));
	if (!Widget::isOtherEventDispatchingEnabled())
	{
		return S_OK;
	}

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
	// Set stylus state first.
	const Vec2 kvScreenPoint(p.x, p.y);
	Stylus::up(kvScreenPoint);

	// Widget event dispatching.
	Widget::dispatchEventStylusUp(WPoint(p.x, p.y));
	if (!Widget::isOtherEventDispatchingEnabled())
	{
		return S_OK;
	}

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
