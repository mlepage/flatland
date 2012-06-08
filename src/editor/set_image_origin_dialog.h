// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_SET_IMAGE_ORIGIN_DIALOG
#define GUARD__WIDGET_SET_IMAGE_ORIGIN_DIALOG


#include "widget_dialog.h"


class WidgetLineEdit;


/*******************************************************************************
*******************************************************************************/
class SetImageOriginDialog : public WidgetDialog
{


public:

	SetImageOriginDialog()
	{
		constructCommon();
	}

	SetImageOriginDialog(
		const tstring& ksName) :
		WidgetDialog(ksName)
	{
		constructCommon();
	}

	SetImageOriginDialog(
		Widget& parent) :
		WidgetDialog(parent)
	{
		constructCommon();
	}

	SetImageOriginDialog(
		Widget& parent,
		const tstring& ksName) :
		WidgetDialog(parent, ksName)
	{
		constructCommon();
	}

	void
	doDecreaseX();

	void
	doDecreaseY();

	void
	doIncreaseX();

	void
	doIncreaseY();

	const WPoint&
	getImageOrigin() const
	{
		return m_imageOriginPosition;
	}

	void
	setImageOrigin(
		const WPoint& kPosition)
	{
		m_imageOriginPosition = kPosition;
		updateLineEditText();
	}


protected:

	virtual
	void
	eventPaint();


private:

	void
	constructCommon();

	void
	updateLineEditText();


private:

	WPoint m_imageOriginPosition;

	WidgetLineEdit* m_pLineEditX;
	WidgetLineEdit* m_pLineEditY;


};


#endif // GUARD
