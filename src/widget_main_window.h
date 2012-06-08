// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_MAIN_WINDOW
#define GUARD__WIDGET_MAIN_WINDOW


#include "widget.h"

#include "widget_menu_bar.h"
#include "widget_tool_bar.h"


/*******************************************************************************
*******************************************************************************/
class WidgetMainWindow : public Widget
{


public:

	WidgetMainWindow() :
		m_pContents(0),
		m_pMenuBar(0),
		m_pToolBar(0)
	{
		setSize(WDim(240, 320));
	}

	WidgetMainWindow(
		const tstring& ksName) :
		Widget(ksName),
		m_pContents(0),
		m_pMenuBar(0),
		m_pToolBar(0)
	{
		setSize(WDim(240, 320));
	}

	WidgetMainWindow(
		Widget& parent) :
		Widget(parent),
		m_pContents(0),
		m_pMenuBar(0),
		m_pToolBar(0)
	{
		setSize(WDim(240, 320));
	}

	WidgetMainWindow(
		Widget& parent,
		const tstring& ksName) :
		Widget(parent, ksName),
		m_pContents(0),
		m_pMenuBar(0),
		m_pToolBar(0)
	{
		setSize(WDim(240, 320));
	}

	Widget&
	getContents() const
	{
		return *m_pContents;
	}

	WidgetMenuBar&
	getMenuBar() const
	{
		return *m_pMenuBar;
	}

	WidgetToolBar&
	getToolBar() const
	{
		return *m_pToolBar;
	}

	bool
	hasContents() const
	{
		return m_pContents != 0;
	}

	bool
	hasMenuBar() const
	{
		return m_pMenuBar != 0;
	}

	bool
	hasToolBar() const
	{
		return m_pToolBar != 0;
	}

	void
	setContents(
		Widget& contents)
	{
		m_pContents = &contents;
		if (hasToolBar())
		{
			contents.setBounds(WRect(0, 0, getWidth(), getHeight() - 24));
		}
		else
		{
			contents.setBounds(WRect(0, 0, getWidth(), getHeight()));
		}
	}

	void
	setMenuBar(
		WidgetMenuBar& menuBar)
	{
		m_pMenuBar = &menuBar;
		menuBar.setBounds(WRect(0, 64, getWidth() / 2, getHeight() - 128));
	}

	void
	setToolBar(
		WidgetToolBar& toolBar)
	{
		m_pToolBar = &toolBar;
		toolBar.setBounds(WRect(0, getHeight() - 24, getWidth(), 24));
		if (hasContents())
		{
			getContents().setBounds(WRect(0, 0, getWidth(), getHeight() - 24));
		}
	}


private:

	Widget* m_pContents;

	WidgetMenuBar* m_pMenuBar;

	WidgetToolBar* m_pToolBar;


};


#endif // GUARD
