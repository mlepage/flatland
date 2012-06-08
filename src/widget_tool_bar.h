// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_TOOL_BAR
#define GUARD__WIDGET_TOOL_BAR


#include "widget.h"


/*******************************************************************************
******************************************************************************/
class WidgetToolBar : public Widget
{


public:

	WidgetToolBar()
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


};


#endif // GUARD
