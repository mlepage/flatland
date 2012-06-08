// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_BUTTON
#define GUARD__WIDGET_BUTTON


#include "widget.h"
#include "stylus.h"


/*******************************************************************************
*******************************************************************************/
class WidgetButton : public Widget
{


public:

	WidgetButton()
	{
		constructCommon();
	}

	WidgetButton(
		const tstring& ksName) :
		Widget(ksName)
	{
		constructCommon();
	}

	WidgetButton(
		Widget& parent) :
		Widget(parent)
	{
		constructCommon();
	}

	WidgetButton(
		Widget& parent,
		const tstring& ksName) :
		Widget(parent, ksName)
	{
		constructCommon();
	}

	const tstring&
	getText() const
	{
		return m_sText;
	}

	bool
	isDown() const
	{
		return m_bDown;
	}

	bool
	isOn() const
	{
		return m_bOn;
	}

	bool
	isToggleButton() const
	{
		return m_bToggle;
	}

	void
	setDown(
		const bool kbDown)
	{
		m_bDown = kbDown;
	}

	// This is to facilitate making button groups.
	void
	setOff(
		const bool kbOff)
	{
		m_bOn = !kbOff;
	}

	void
	setText(
		const tstring& ksText)
	{
		m_sText = ksText;
	}

	void
	setToggleButton(
		const bool kbToggle)
	{
		m_bToggle = kbToggle;
	}

	void
	toggle()
	{
		if (isToggleButton())
		{
			m_bOn = !m_bOn;
		}
	}


public:

	virtual
	void
	slotSetOff(
		const bool kbOff)
	{
		setOff(kbOff);
	}


public:

	sigslot::signal0<> clicked;

	sigslot::signal0<> pressed;

	sigslot::signal0<> released;

	sigslot::signal1<const bool> toggled;


protected:

	virtual
	void
	drawButton();

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
	eventStylusMove(
		const WPoint& kPosition)
	{
		if (Stylus::isDown())
		{
			setDown(WRect(WPoint(0, 0), getSize()).contains(kPosition));
		}
	}

	virtual
	void
	eventStylusUp(
		const WPoint& kPosition)
	{
		setDown(false);
		released();
		if (WRect(WPoint(0, 0), getSize()).contains(kPosition))
		{
			if (isToggleButton())
			{
				m_bOn = !m_bOn;
				toggled(isOn());
			}
			clicked();
		}
	}


private:

	void
	constructCommon()
	{
		m_bToggle = false;
		m_bDown = false;
		m_bOn = false;
	}


private:

	tstring m_sText;

	bool m_bToggle;

	bool m_bDown;

	bool m_bOn;


};


#endif // GUARD
