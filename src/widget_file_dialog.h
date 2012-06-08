// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_FILE_DIALOG
#define GUARD__WIDGET_FILE_DIALOG


#include "widget_dialog.h"


class WidgetLineEdit;


/*******************************************************************************
	Right now, use of this dialog is finnicky. It lives globally in the
	application. When shown, it goes modal and takes the entire screen.
	However, it does not run its own event loop, but just goes asynchronously
	like every other widget. So to use it, you need to connect slots to both
	the accepted and cancelled signals, do your handling there, and be sure
	to unconnect your slots when you are done so the dialog can be reused.
	Obviously it cannot be used reentrantly.
******************************************************************************/
class WidgetFileDialog : public WidgetDialog
{


public:

	WidgetFileDialog()
	{
		init();
	}

	WidgetFileDialog(
		Widget& parent) :
		WidgetDialog(parent)
	{
		init();
	}

	WidgetFileDialog(
		Widget& parent,
		const tstring& ksName) :
		WidgetDialog(parent, ksName)
	{
		init();
	}

	const tstring&
	getSelectedFileName() const;

	void
	init();

	virtual void slotHide()
	{
		hide();
	}

	virtual void slotShow()
	{
		show();
	}


public:

	sigslot::signal0<> accepted;

	sigslot::signal0<> cancelled;


protected:
	
	virtual
	void
	eventPaint();


private:

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


private:

	WidgetLineEdit* m_pLineEdit;


};


#endif // GUARD
