// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_TOOL_BUTTON
#define GUARD__WIDGET_TOOL_BUTTON


#include "widget_button.h"


/*******************************************************************************
******************************************************************************/
class WidgetToolButton : public WidgetButton
{


public:

	WidgetToolButton()
	{
	}

	WidgetToolButton(
		Widget& parent) :
		WidgetButton(parent)
	{
	}

	WidgetToolButton(
		Widget& parent,
		const tstring& ksName) :
		WidgetButton(parent, ksName)
	{
	}


private:

	virtual
	void
	drawButton();


};


#endif // GUARD
