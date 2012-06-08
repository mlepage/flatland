// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "editor/set_image_origin_dialog.h"

#include "image.h"
#include "resourcex.h"
#include "screen.h"
#include "widget_line_edit.h"
#include "widget_tool_button.h"


/*******************************************************************************
*******************************************************************************/
void
SetImageOriginDialog::constructCommon()
{
	const int knSpace = 70;
	setSize(WDim(240, 320 - knSpace - knSpace));
	setPosition(WPoint(0, knSpace));

	hideRejectButton();
	setAcceptButtonText(_T("OK"));

	m_pLineEditX = new WidgetLineEdit(*this, _T("line_edit_x"));
	m_pLineEditX->setBounds(WRect(64, 64, 32, 16));
	m_pLineEditX->setText(toString(m_imageOriginPosition.getX()));
	m_pLineEditY = new WidgetLineEdit(*this, _T("line_edit_y"));
	m_pLineEditY->setBounds(WRect(64, 88, 32, 16));
	m_pLineEditY->setText(toString(m_imageOriginPosition.getY()));

	{
		WidgetToolButton& button =
			*new WidgetToolButton(*this, _T("increase_x"));
		button.setBounds(WRect(96 + 8, 64, 16, 8));
		button.setIconSet(
			Resourcex::getImage(_T("arrow_up")).getSurface());
		button.clicked.connect(this, &SetImageOriginDialog::doIncreaseX);
	}
	{
		WidgetToolButton& button =
			*new WidgetToolButton(*this, _T("decrease_x"));
		button.setBounds(WRect(96 + 8, 64 + 8, 16, 8));
		button.setIconSet(
			Resourcex::getImage(_T("arrow_down")).getSurface());
		button.clicked.connect(this, &SetImageOriginDialog::doDecreaseX);
	}
	{
		WidgetToolButton& button =
			*new WidgetToolButton(*this, _T("increase_y"));
		button.setBounds(WRect(96 + 8, 88, 16, 8));
		button.setIconSet(
			Resourcex::getImage(_T("arrow_up")).getSurface());
		button.clicked.connect(this, &SetImageOriginDialog::doIncreaseY);
	}
	{
		WidgetToolButton& button =
			*new WidgetToolButton(*this, _T("decrease_y"));
		button.setBounds(WRect(96 + 8, 88 + 8, 16, 8));
		button.setIconSet(
			Resourcex::getImage(_T("arrow_down")).getSurface());
		button.clicked.connect(this, &SetImageOriginDialog::doDecreaseY);
	}

	layoutButtons();
}


/*******************************************************************************
*******************************************************************************/
void
SetImageOriginDialog::doDecreaseX()
{
	m_imageOriginPosition.translate(WPoint(-1, 0));
	updateLineEditText();
}


/*******************************************************************************
*******************************************************************************/
void
SetImageOriginDialog::doDecreaseY()
{
	m_imageOriginPosition.translate(WPoint(0, -1));
	updateLineEditText();
}


/*******************************************************************************
*******************************************************************************/
void
SetImageOriginDialog::doIncreaseX()
{
	m_imageOriginPosition.translate(WPoint(1, 0));
	updateLineEditText();
}


/*******************************************************************************
*******************************************************************************/
void
SetImageOriginDialog::doIncreaseY()
{
	m_imageOriginPosition.translate(WPoint(0, 1));
	updateLineEditText();
}


/*******************************************************************************
*******************************************************************************/
void
SetImageOriginDialog::eventPaint()
{
	// TODO Fix this.
	COLORREF colour = RGB(0, 0, 128);
	RECT screenRect = reinterpret_cast<const RECT&>(getGlobalBounds());

	const int knSpace = 70;

	getBackBuffer().FillRect(
		&screenRect,
		colour,
		0,
		NULL);

	// These two commands draw outside the widget. Bad.
	screenRect.top = 0;
	screenRect.bottom = knSpace;
	getBackBuffer().FillRect(
		&screenRect,
		RGB(0, 0, 0),
		0,
		NULL);
	screenRect.top = 320 - knSpace;
	screenRect.bottom = 320;
	getBackBuffer().FillRect(
		&screenRect,
		RGB(0, 0, 0),
		0,
		NULL);

	getBackBuffer().DrawText(
		16,
		knSpace + 24,
		_T("Set the bottom left point of the image."),
		Screen::getSystemFont(),
		0,
		NULL,
		0,
		NULL);

	getBackBuffer().DrawText(
		48,
		knSpace + 64 + 6,
		_T("X:"),
		Screen::getSystemFont(),
		0,
		NULL,
		0,
		NULL);
	getBackBuffer().DrawText(
		48,
		knSpace + 88 + 6,
		_T("Y:"),
		Screen::getSystemFont(),
		0,
		NULL,
		0,
		NULL);
}


/*******************************************************************************
*******************************************************************************/
void
SetImageOriginDialog::updateLineEditText()
{
	m_pLineEditX->setText(toString(m_imageOriginPosition.getX()));
	m_pLineEditY->setText(toString(m_imageOriginPosition.getY()));
}
