// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "state_game.h"

#include "console.h"
#include "entity_type.h"
#include "game.h"
#include "image.h"
#include "key.h"
#include "matrix.h"
#include "rect.h"
#include "render.h"
#include "resourcex.h"
#include "screen.h"
#include "stylus.h"
#include "variable.h"
#include "view.h"


StateGame
StateGame::m_state;


/*******************************************************************************
*******************************************************************************/
void
StateGame::enterState()
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
	Application::getDisplay().Flip();

	// Load level.
	Console::print(_T("Loading level\n"));
	Resourcex::loadLevel(Variable::first_level.getValue());

	// Center view on player entity.
	for (int n = 0; n != Game::getNumberOfEntities(); ++n)
	{
		Entity& testEntity = Game::getEntity(n);
		if (testEntity.getType().getName() == tstring(_T("player")))
		{
			View::setWorldViewOrigin(
				testEntity.getOrigin() -
					View::getViewSize() / 2);
			break;
		}
	}
}


/*******************************************************************************
	TODO This is obviously a mess. Refactor and clean up.
*******************************************************************************/
void
StateGame::processFrame()
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();

	// Try to set the active entity if in stylus easy mode.
	if (Variable::stylus_easy.getFloatValue() &&
		Stylus::isDown() &&
		!Game::getActiveEntity())
	{
		Application::setActiveEntity(Stylus::getLeadPoint());
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
			Game::getActiveEntity()->m_nForceFrame =
				Application::getActualFrameNumber();

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
	// for getting the desired times explicitly without exposing it. Also,
	// this should be in the screen module, not here.
	// Frame rate etc.
	if (Variable::render_frame_info.getFloatValue())
	{
		TCHAR str[128];
		float fFrameTime = Application::getActualFrameTime();
		_stprintf(str, TEXT("%.2f ms"), fFrameTime);
		backBuffer.DrawText(120, 312, str, Screen::getSystemFont(),
			0, NULL, 0, NULL);
		float fFrameRate = Application::getActualFrameRate();
		_stprintf(str, TEXT("%.2f fps"), fFrameRate);
		backBuffer.DrawText(180, 312, str, Screen::getSystemFont(),
			0, NULL, 0, NULL);
	}
#endif
}
