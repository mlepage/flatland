// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_MENU_BAR
#define GUARD__WIDGET_MENU_BAR


#include "widget.h"


/*******************************************************************************
*******************************************************************************/
class WidgetMenuBar : public Widget
{


public:

	WidgetMenuBar()
	{
	}

	WidgetMenuBar(
		const tstring& ksName) :
		Widget(ksName)
	{
	}

	WidgetMenuBar(
		Widget& parent) :
		Widget(parent)
	{
	}

	WidgetMenuBar(
		Widget& parent,
		const tstring& ksName) :
		Widget(parent, ksName)
	{
	}


protected:
	
	virtual
	void
	eventHide();

	virtual
	void
	eventKeyPress(
		const int knKeyCode);

	virtual
	void
	eventPaint();

	virtual
	void
	eventShow();


};


#endif // GUARD
