// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__MENU
#define GUARD__MENU


#include <vector>
#include "rect.h"
#include "tstring.h"


/*******************************************************************************
	I'd like to have a general widget toolkit that sits on top of the game. So
	these items would be buttons, but I'd also have radio buttons, check boxes,
	text fields, all the rest.

	For now, just consider menu items as button-like objects.
*******************************************************************************/
class Menu
{

public:

	class Item;


public:

	static
	void
	clearCurrentMenu();

	static
	int
	getCurrentItem();

	static
	Menu&
	getCurrentMenu();

	static
	bool
	hasCurrentMenu();

	static
	void
	init();

	static
	void
	setCurrentItem(
		const int knIndex);

	static
	void
	setCurrentMenu(
		Menu& menu);


public:

#define DECL_VAR(name) static Menu name
	DECL_VAR(editor);
	DECL_VAR(game_main);
	DECL_VAR(options);
	DECL_VAR(title_main);
#undef DECL_VAR


public:

	void
	addItem(
		Item& item);

	const Item&
	getItem(
		const int knIndex) const;

	const tstring&
	getName() const;

	int
	getNumberOfItems() const;


private:

	static
	Menu* m_pCurrentMenu;

	static
	int m_nCurrentItem;


private:

	Menu(
		const tstring& ksName);


private:

	// The name.
	tstring m_sName;

	// The items.
	std::vector<Item> m_cItem;


};


/*******************************************************************************
	I didn't make this a member of class Menu because it might be used
	elsewhere.
*******************************************************************************/
class Command
{

public:

	// TODO pure virtual?
	virtual
	~Command();

	virtual
	void
	execute() = 0;


};


/*******************************************************************************
*******************************************************************************/
class Menu::Item
{


public:

	Item(
		const tstring& ksName);

	Item(
		const tstring& ksName,
		const Rect& krBounds,
		Command& command);

	void
	doCommand() const;

	const Rect&
	getBounds() const;

	const tstring&
	getName() const;

	void
	setBounds(
		const Rect& krBounds);


private:

	// The name.
	tstring m_sName;

	Rect m_rBounds;

	Command* m_pCommand;

};


/*******************************************************************************
*******************************************************************************/
inline
Command::~Command()
{
}


/*******************************************************************************
*******************************************************************************/
inline
Menu::Menu(
	const tstring& ksName) :
	m_sName(ksName)
{
}


/*******************************************************************************
*******************************************************************************/
inline
void
Menu::addItem(
	Item& item)
{
	m_cItem.push_back(item);
}


/*******************************************************************************
*******************************************************************************/
inline
void
Menu::clearCurrentMenu()
{
	m_pCurrentMenu = 0;
}


/*******************************************************************************
*******************************************************************************/
inline
int
Menu::getCurrentItem()
{
	return m_nCurrentItem;
}


/*******************************************************************************
*******************************************************************************/
inline
Menu&
Menu::getCurrentMenu()
{
	return *m_pCurrentMenu;
}


/*******************************************************************************
*******************************************************************************/
inline
const Menu::Item&
Menu::getItem(
	const int knIndex) const
{
	return m_cItem[knIndex];
}


/*******************************************************************************
*******************************************************************************/
inline
int
Menu::getNumberOfItems() const
{
	return m_cItem.size();
}


/*******************************************************************************
*******************************************************************************/
inline
const tstring&
Menu::getName() const
{
	return m_sName;
}


/*******************************************************************************
*******************************************************************************/
inline
bool
Menu::hasCurrentMenu()
{
	return m_pCurrentMenu != 0;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Menu::setCurrentItem(
	const int knIndex)
{
	m_nCurrentItem = knIndex;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Menu::setCurrentMenu(
	Menu& menu)
{
	m_pCurrentMenu = &menu;
}


/*******************************************************************************
*******************************************************************************/
inline
Menu::Item::Item(
	const tstring& ksName) :
	m_sName(ksName)
{
}


/*******************************************************************************
*******************************************************************************/
inline
Menu::Item::Item(
	const tstring& ksName,
	const Rect& krBounds,
	Command& command) :
	m_sName(ksName),
	m_rBounds(krBounds),
	m_pCommand(&command)
{
}


/*******************************************************************************
*******************************************************************************/
inline
void
Menu::Item::doCommand() const
{
	// TODO check if there is a command
	m_pCommand->execute();
}


/*******************************************************************************
*******************************************************************************/
inline
const Rect&
Menu::Item::getBounds() const
{
	return m_rBounds;
}


/*******************************************************************************
*******************************************************************************/
inline
const tstring&
Menu::Item::getName() const
{
	return m_sName;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Menu::Item::setBounds(
	const Rect& krBounds)
{
	m_rBounds = krBounds;
}


#endif // GUARD
