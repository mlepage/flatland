// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "menu.h"

#include "game.h"
#include "resourcex.h"
#include "state_nag.h"
#include "state_title.h"
#include "widget_file_dialog.h" // for showing file dialog
#include "widget_main_window.h"


#define DEF_VAR(name) Menu Menu::name(_T(#name))
	DEF_VAR(editor);
	DEF_VAR(game_main);
	DEF_VAR(options);
	DEF_VAR(title_main);
#undef DEF_VAR

Menu*
Menu::m_pCurrentMenu = 0;

int
Menu::m_nCurrentItem = -1;


namespace
{


struct EndGameCommand : public Command
{
	virtual
	void
	execute()
	{
		Game::endGame();
	};
} endGameCommand;


struct levelEditorCommand : public Command
{
	virtual
	void
	execute()
	{
	};
} levelEditorCommand;


struct modelEditorCommand : public Command
{
	virtual
	void
	execute()
	{
		Application::getModelEditorMainWindow().show();
		Menu::clearCurrentMenu();
	};
} modelEditorCommand;


struct NewGameCommand : public Command
{
	virtual
	void
	execute()
	{
		Game::newGame();
	};
} newGameCommand;


struct NullCommand : public Command
{
	virtual
	void
	execute()
	{
	};
} nullCommand;


struct QuitCommand : public Command
{
	virtual
	void
	execute()
	{
		Menu::clearCurrentMenu();
		StateNag::changeState();
	};
} quitCommand;


struct ResumeGameCommand : public Command
{
	virtual
	void
	execute()
	{
		// Menu.
		Menu::clearCurrentMenu();
		Game::setPaused(false);
	};
} resumeGameCommand;


struct TitleCommand : public Command
{
	virtual
	void
	execute()
	{
		StateTitle::changeState();
		Menu::setCurrentMenu(Menu::title_main);
	};
} titleCommand;


} // namespace


/*******************************************************************************
*******************************************************************************/
void
Menu::init()
{
	// TODO There is an order of evaluation problem here.
	// Don't use this technique.
	const int knX1 = 120;
	const int knX2 = 220;
	const int knY = 110;
	const int knHeight = 20;
	const int knVSpace = 10;
	Rect rBounds;

	// Title main.
	rBounds = Rect(Vec2(knX1, knY), Vec2(knX2, knY + knHeight));
	{
		rBounds.getMin()[1] += knHeight + knVSpace;
		rBounds.getMax()[1] += knHeight + knVSpace;
		Menu::Item item(
			_T("new game"),
			rBounds,
			newGameCommand);
		Menu::title_main.addItem(item);
	}
	{
		rBounds.getMin()[1] += knHeight + knVSpace;
		rBounds.getMax()[1] += knHeight + knVSpace;
		Menu::Item item(
			_T("load game"),
			rBounds,
			nullCommand);
		Menu::title_main.addItem(item);
	}
	{
		rBounds.getMin()[1] += knHeight + knVSpace;
		rBounds.getMax()[1] += knHeight + knVSpace;
		Menu::Item item(
			_T("model editor"),
			rBounds,
			modelEditorCommand);
			Menu::title_main.addItem(item);
	}
	{
		rBounds.getMin()[1] += knHeight + knVSpace;
		rBounds.getMax()[1] += knHeight + knVSpace;
		Menu::Item item(
			_T("level editor"),
			rBounds,
			levelEditorCommand);
		Menu::title_main.addItem(item);
	}
	{
		rBounds.getMin()[1] += knHeight + knVSpace;
		rBounds.getMax()[1] += knHeight + knVSpace;
		Menu::Item item(
			_T("quit"),
			rBounds,
			quitCommand);
		Menu::title_main.addItem(item);
	}

	// Game main.
	rBounds = Rect(Vec2(knX1, knY), Vec2(knX2, knY + knHeight));
	{
		rBounds.getMin()[1] += knHeight + knVSpace;
		rBounds.getMax()[1] += knHeight + knVSpace;
		Menu::Item item(
			_T("resume game"),
			rBounds,
			resumeGameCommand);
		Menu::game_main.addItem(item);
	}
	{
		rBounds.getMin()[1] += knHeight + knVSpace;
		rBounds.getMax()[1] += knHeight + knVSpace;
		Menu::Item item(
			_T("end game"),
			rBounds,
			endGameCommand);
		Menu::game_main.addItem(item);
	}
	{
		rBounds.getMin()[1] += knHeight + knVSpace;
		rBounds.getMax()[1] += knHeight + knVSpace;
		Menu::Item item(
			_T("quit"),
			rBounds,
			quitCommand);
		Menu::game_main.addItem(item);
	}
}
