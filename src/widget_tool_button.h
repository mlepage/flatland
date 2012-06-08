// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_TOOL_BUTTON
#define GUARD__WIDGET_TOOL_BUTTON


#include "widget_button.h"


/*******************************************************************************
*******************************************************************************/
class WidgetToolButton : public WidgetButton
{


public:

	WidgetToolButton()
	{
		constructCommon();
	}

	WidgetToolButton(
		const tstring& ksName) :
		WidgetButton(ksName)
	{
		constructCommon();
	}

	WidgetToolButton(
		Widget& parent) :
		WidgetButton(parent)
	{
		constructCommon();
	}

	WidgetToolButton(
		Widget& parent,
		const tstring& ksName) :
		WidgetButton(parent, ksName)
	{
		constructCommon();
	}

	WSurface&
	getIconSet() const
	{
		return *m_pIconSurface;
	}

	bool
	hasIconSet() const
	{
		return m_pIconSurface != 0;
	}

	void
	setIconSet(
		WSurface& iconSurface)
	{
		m_pIconSurface = &iconSurface;
	}


public:

	virtual
	void
	slotSetOff(
		const bool kbOff)
	{
		setOff(kbOff);
	}


protected:

	virtual
	void
	drawButton();


private:

	void
	constructCommon()
	{
		m_pIconSurface = 0;
		setSize(WDim(22, 22));
	}


private:

	// Not owned.
	WSurface* m_pIconSurface;


};


#endif // GUARD
