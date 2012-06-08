// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_PUSH_BUTTON
#define GUARD__WIDGET_PUSH_BUTTON


#include "widget_button.h"


/*******************************************************************************
*******************************************************************************/
class WidgetPushButton : public WidgetButton
{


public:

	WidgetPushButton()
	{
	}

	WidgetPushButton(
		const tstring& ksName) :
		WidgetButton(ksName)
	{
	}

	WidgetPushButton(
		Widget& parent) :
		WidgetButton(parent)
	{
	}

	WidgetPushButton(
		Widget& parent,
		const tstring& ksName) :
		WidgetButton(parent, ksName)
	{
	}


protected:

	virtual
	void
	drawButton();


};


#endif // GUARD
