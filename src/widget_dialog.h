// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_DIALOG
#define GUARD__WIDGET_DIALOG


#include "widget.h"


class WidgetPushButton;


/*******************************************************************************
*******************************************************************************/
class WidgetDialog : public Widget
{


public:

	WidgetDialog()
	{
		constructCommon();
	}

	WidgetDialog(
		const tstring& ksName) :
		Widget(ksName)
	{
		constructCommon();
	}

	WidgetDialog(
		Widget& parent) :
		Widget(parent)
	{
		constructCommon();
	}

	WidgetDialog(
		Widget& parent,
		const tstring& ksName) :
		Widget(parent, ksName)
	{
		constructCommon();
	}

	void
	hideRejectButton();

	void
	setAcceptButtonText(
		const tstring& ksText);

	void
	setCancelButtonText(
		const tstring& ksText);

	void
	setRejectButtonText(
		const tstring& ksText);

	virtual
	void
	slotHide()
	{
		hide();
	}


public:

	sigslot::signal0<> accepted;

	sigslot::signal0<> cancelled;

	sigslot::signal0<> rejected;


protected:

	// TODO This really should just respond to move and resize events.
	void
	layoutButtons();


private:

	void
	constructCommon();

	void
	fireAccepted()
	{
		accepted();
	}

	void
	fireCancelled()
	{
		cancelled();
	}

	void
	fireRejected()
	{
		rejected();
	}


private:

	WidgetPushButton* m_pButtonAccept;

	WidgetPushButton* m_pButtonCancel;

	WidgetPushButton* m_pButtonReject;


};


#endif // GUARD
