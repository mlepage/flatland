// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_MESSAGE_DIALOG
#define GUARD__WIDGET_MESSAGE_DIALOG


#include "widget_dialog.h"


/*******************************************************************************
*******************************************************************************/
class WidgetMessageDialog : public WidgetDialog
{


public:

	WidgetMessageDialog()
	{
		constructCommon();
	}

	WidgetMessageDialog(
		const tstring& ksName) :
		WidgetDialog(ksName)
	{
		constructCommon();
	}

	WidgetMessageDialog(
		Widget& parent) :
		WidgetDialog(parent)
	{
		constructCommon();
	}

	WidgetMessageDialog(
		Widget& parent,
		const tstring& ksName) :
		WidgetDialog(parent, ksName)
	{
		constructCommon();
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


protected:

	virtual
	void
	eventPaint();


private:

	void
	constructCommon();


private:

	tstring m_sText;


};


#endif // GUARD
