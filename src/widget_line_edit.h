// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_LINE_EDIT
#define GUARD__WIDGET_LINE_EDIT


#include "widget.h"


/*******************************************************************************
*******************************************************************************/
class WidgetLineEdit : public Widget
{


public:

	WidgetLineEdit()
	{
	}

	WidgetLineEdit(
		const tstring& ksName) :
		Widget(ksName)
	{
	}

	WidgetLineEdit(
		Widget& parent) :
		Widget(parent)
	{
	}

	WidgetLineEdit(
		Widget& parent,
		const tstring& ksName) :
		Widget(parent, ksName)
	{
	}

	const tstring&
	getText() const
	{
		return m_sText;
	}

	void
	setText(
		const tstring& ksText)
	{
		m_sText = ksText;
	}


public:

	sigslot::signal0<> clicked;

	sigslot::signal0<> pressed;

	sigslot::signal0<> released;


protected:

	virtual
	void
	eventKeyPress(
		const int knKeyCode);

	virtual
	void
	eventPaint();


private:

	tstring m_sText;

};


#endif // GUARD
