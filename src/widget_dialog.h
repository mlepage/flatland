// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_DIALOG
#define GUARD__WIDGET_DIALOG


#include "widget.h"


/*******************************************************************************
******************************************************************************/
class WidgetDialog : public Widget
{


public:

	WidgetDialog() :
		m_bModal(false)
	{
		Widget::addDialog(*this);
		setSize(WDim(240, 320));
	}

	WidgetDialog(
		Widget& parent) :
		Widget(parent),
		m_bModal(false)
	{
		Widget::addDialog(*this);
		setSize(WDim(240, 320));
	}

	WidgetDialog(
		Widget& parent,
		const tstring& ksName) :
		Widget(parent, ksName),
		m_bModal(false)
	{
		Widget::addDialog(*this);
		setSize(WDim(240, 320));
	}

	bool
	isModal() const
	{
		return m_bModal;
	}

	void
	setModal(
		const bool kbModal)
	{
		m_bModal = kbModal;
	}


private:

	bool m_bModal;

};


#endif // GUARD
