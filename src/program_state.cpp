// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "program_state.h"

#include "GapiDraw.h"
#include "console.h"
#include "entity_type.h"
#include "game.h"
#include "image.h"
#include "key.h"
#include "menu.h"
#include "myapplication.h"
#include "resourcex.h"
#include "screen.h"
#include "stylus.h"
#include "variable.h"
#include "view.h"


ProgramState::Editor ProgramState::editor;
ProgramState::GameRunning ProgramState::game_running;
ProgramState::GameStart ProgramState::game_start;
ProgramState::Splash ProgramState::splash;
ProgramState::Title ProgramState::title;
ProgramState::Quit ProgramState::quit;

ProgramState* ProgramState::m_pCurrentState = 0;

int ProgramState::m_nCurrentFrame = 0;


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
		Game::setAutoscrollEntity(pPlayerEntity);
	}
}


/*******************************************************************************
*******************************************************************************/
void
ProgramState::Editor::processFrame()
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();
}


/*******************************************************************************
*******************************************************************************/
void
ProgramState::GameRunning::processFrame()
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();

	// Try to set the active entity if in stylus easy mode.
	if (Variable::stylus_easy.getFloatValue() &&
		Stylus::isDown() &&
		!Game::getActiveEntity())
	{
		setActiveEntity(Stylus::getLeadPoint());
	}

	// See if the active entity should be disabled.
	if (Game::getActiveEntity())
	{
		std::vector<Entity*> cpEntity =
			World::getEntitiesAtPoint(
				dehomo(View::getTransformS2W() * homo(Stylus::getLeadPoint())));

		bool bControlEntity = false;

		for (int n = 0; n != cpEntity.size(); ++n)
		{
			// TODO use static variables to lookup entity type?
			if (cpEntity[n]->getType().getName() == tstring(_T("control")))
			{
				bControlEntity = true;
			}
		}
		if (!bControlEntity)
		{
			// This is so forces don't act on a thrown player entity for a few
			// frames, to allow them to escape.
			Game::getActiveEntity()->m_nForceFrame = getCurrentFrame();

			Game::setActiveEntity(0);
		}
	}

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

	// Autoscroll. (Not analogous, but must be after runFrame.)
	if (Game::isAutoscrollEnabled() &&
		Game::getAutoscrollEntity() &&
		!Game::getActiveEntity())
	{
		const int knBorder = 32;
		Rect rBounds = Game::getAutoscrollEntity()->getBounds();
		Rect rView = View::getWorldView();
		Vec2 vOrigin = rView.getMin();
		if (rBounds.getMin()[0] < rView.getMin()[0] + knBorder)
		{
			vOrigin[0] -= rView.getMin()[0] + knBorder - rBounds.getMin()[0];
		}
		else if (rView.getMax()[0] - knBorder < rBounds.getMax()[0])
		{
			vOrigin[0] += rBounds.getMax()[0] - rView.getMax()[0] + knBorder;
		}
		if (rBounds.getMin()[1] < rView.getMin()[1] + knBorder)
		{
			vOrigin[1] -= rView.getMin()[1] + knBorder - rBounds.getMin()[1];
		}
		else if (rView.getMax()[1] - knBorder < rBounds.getMax()[1])
		{
			vOrigin[1] += rBounds.getMax()[1] - rView.getMax()[1] + knBorder;
		}
		View::setWorldViewOrigin(vOrigin);
	}

	// The following are analogous to Quake 2's CL_Frame.

	// I need this to process keys that repeat, like +forward and -forward,
	// until I find out where Quake 2 does it.
	Key::processCommands();

	Screen::updateScreen();

	// Hack to go back to main menu
	{
		bool bBlockFound = false;
		for (int n = 0; n != Game::getNumberOfEntities(); ++n)
		{
			Entity& entity = Game::getEntity(n);
			if (entity.getType().getName() == tstring(_T("block")))
			{
				bBlockFound = true;
				break;
			}
		}
		if (!bBlockFound)
		{
			Game::endGame();
		}
	}

#if 1
	// TODO these timer methods need not be exposed, I can just make a method
	// for getting the desired times explicitly without exposing it.
	// Frame rate etc.
	TCHAR str[128];
	FLOAT nActualFrameTime = 0;
	CMyApplication::m_pMyApplication->getTimer().GetActualFrameTime(
		&nActualFrameTime);
	_stprintf(str, TEXT("%.2f ms"), nActualFrameTime);
	backBuffer.DrawText(120, 312, str, Screen::getSystemFont(),
		0, NULL, 0, NULL);
	FLOAT nFrameRate = 0;
	CMyApplication::m_pMyApplication->getTimer().GetActualFrameRate(
		&nFrameRate);
	_stprintf(str, TEXT("%.2f fps"), nFrameRate);
	backBuffer.DrawText(180, 312, str, Screen::getSystemFont(),
		0, NULL, 0, NULL);
#endif
}


/*******************************************************************************
*******************************************************************************/
void
ProgramState::GameStart::processFrame()
{
	// Simple switch for two frames.

	static bool bLoad = false;

	if (bLoad)
	{
		// Load level.
		Console::print(_T("Loading level\n"));
		Resourcex::loadLevel(Variable::first_level.getValue());
		ProgramState::setCurrentState(ProgramState::game_running);
	}
	else
	{
		// Draw loading screen.
		CGapiSurface& backBuffer = *Screen::getBackBuffer();
		backBuffer.BltFast(
			0,
			0,
			&Resourcex::getImage(_T("screen_loading")).getSurface(),
			NULL,
			0,
			NULL);
	}

	bLoad = !bLoad;
}


/*******************************************************************************
*******************************************************************************/
void
ProgramState::Quit::processFrame()
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();

	// TODO move these into configuration somewhere
	const int knFadeTime = 30;
	const int knFullTime = 30;
	static const int knFadeInFrame = getCurrentFrame();
	static const int knFullFrame = knFadeInFrame + knFadeTime;
	static const int knFadeOutFrame = knFullFrame + knFullTime;
	static const int knFinishFrame = knFadeOutFrame + knFadeTime;

	if (getCurrentFrame() < knFinishFrame)
	{
		backBuffer.FillRect(
			NULL,
			RGB(0, 0, 0),
			0,
			NULL);
		GDBLTFASTFX fx;
		if (getCurrentFrame() < knFullFrame)
		{
			// Fade in.
			fx.dwOpacity =
				static_cast<double>(getCurrentFrame() - knFadeInFrame) /
					knFadeTime * 255;
		}
		else if (knFadeOutFrame <= getCurrentFrame())
		{
			// Fade out.
			fx.dwOpacity =
				static_cast<double>(knFinishFrame - getCurrentFrame() - 1) /
					knFadeTime * 255;
		}
		else
		{
			fx.dwOpacity = 255;
		}
		backBuffer.BltFast(
			0,
			0,
			&Resourcex::getImage(_T("screen_nag")).getSurface(),
			NULL,
			GDBLTFAST_OPACITY,
			&fx);
	}
	else
	{
		CMyApplication::m_pMyApplication->Shutdown();
	}
}


/*******************************************************************************
*******************************************************************************/
void
ProgramState::Splash::processFrame()
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();

	// TODO move these into configuration somewhere
	const int knFadeTime = 30;
	const int knFullTime = 30;
	static const int knFadeInFrame = getCurrentFrame();
	static const int knFullFrame = knFadeInFrame + knFadeTime;
	static const int knFadeOutFrame = knFullFrame + knFullTime;
	static const int knFinishFrame = knFadeOutFrame + knFadeTime;

	if (getCurrentFrame() < knFinishFrame)
	{
		backBuffer.FillRect(
			NULL,
			RGB(0, 0, 0),
			0,
			NULL);
		GDBLTFASTFX fx;
		if (getCurrentFrame() < knFullFrame)
		{
			// Fade in.
			fx.dwOpacity =
				static_cast<double>(getCurrentFrame() - knFadeInFrame) /
					knFadeTime * 255;
		}
		else if (knFadeOutFrame <= getCurrentFrame())
		{
			// Fade out.
			fx.dwOpacity =
				static_cast<double>(knFinishFrame - getCurrentFrame() - 1) /
					knFadeTime * 255;
		}
		else
		{
			fx.dwOpacity = 255;
		}
		backBuffer.BltFast(
			0,
			0,
			&Resourcex::getImage(_T("screen_splash")).getSurface(),
			NULL,
			GDBLTFAST_OPACITY,
			&fx);
	}
	else
	{
		setCurrentState(ProgramState::title);
		Menu::setCurrentMenu(Menu::title_main);
		Menu::setCurrentItem(0);
	}
}


/*******************************************************************************
*******************************************************************************/
void
ProgramState::Title::processFrame()
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();

	CGapiSurface& image = Resourcex::getImage(_T("screen_title")).getSurface();
	backBuffer.BltFast(0, 0, &image, NULL, 0, NULL);

	Screen::updateScreen();
}
