// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_BUTTON
#define GUARD__WIDGET_BUTTON


#include "widget.h"


/*******************************************************************************
******************************************************************************/
class WidgetButton : public Widget
{


public:

	WidgetButton() :
		m_bDown(false)
	{
	}

	WidgetButton(
		Widget& parent) :
		Widget(parent),
		m_bDown(false)
	{
	}

	WidgetButton(
		Widget& parent,
		const tstring& ksName) :
		Widget(parent, ksName),
		m_bDown(false)
	{
	}

	bool
	isDown() const
	{
		return m_bDown;
	}

	void
	setDown(
		const bool kbDown)
	{
		m_bDown = kbDown;
	}


public:

	sigslot::signal0<> clicked;

	sigslot::signal0<> pressed;

	sigslot::signal0<> released;


protected:

	virtual
	void
	eventPaint()
	{
		drawButton();
	}

	virtual
	void
	eventStylusDown(
		const WPoint& kPosition)
	{
		setDown(WRect(WPoint(0, 0), getSize()).contains(kPosition));
		pressed();
	}

	virtual
	void
	eventStylusUp(
		const WPoint& kPosition)
	{
		setDown(false);
		released();
		// TODO This should only occur if mouse up is within button.
		clicked();
	}


private:

	virtual
	void
	drawButton() = 0;


private:

	bool m_bDown;

};


#endif // GUARD
