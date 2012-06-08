// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_KEYBOARD
#define GUARD__WIDGET_KEYBOARD


#include <vector>

#include "widget.h"


class WidgetButton;


/*******************************************************************************
*******************************************************************************/
class WidgetKeyboard : public Widget
{


public:

	enum
	{
		m_knButtonSpacing = 16
	};


public:

	WidgetKeyboard()
	{
		init();
	}

	WidgetKeyboard(
		const tstring& ksName) :
		Widget(ksName)
	{
		init();
	}

	WidgetKeyboard(
		Widget& parent) :
		Widget(parent)
	{
		init();
	}

	WidgetKeyboard(
		Widget& parent,
		const tstring& ksName) :
		Widget(parent, ksName)
	{
		init();
	}


private:

	void
	init();


private:

	std::vector<WidgetButton*> m_cpButton;


};


#endif // GUARD
