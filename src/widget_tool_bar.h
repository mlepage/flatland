// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_TOOL_BAR
#define GUARD__WIDGET_TOOL_BAR


#include "widget.h"


/*******************************************************************************
	Mini style guide: tool bar should be 24 pixels high, with buttons spaced
	24 pixels apart. Buttons should be 22x22 pixels.
*******************************************************************************/
class WidgetToolBar : public Widget
{


public:

	WidgetToolBar()
	{
	};

	WidgetToolBar(
		const tstring& ksName) :
		Widget(ksName)
	{
	};

	WidgetToolBar(
		Widget& parent) :
		Widget(parent)
	{
	};

	WidgetToolBar(
		Widget& parent,
		const tstring& ksName) :
		Widget(parent, ksName)
	{
	};


protected:
	
	virtual
	void
	eventPaint();


};


#endif // GUARD
