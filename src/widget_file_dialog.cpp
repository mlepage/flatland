// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget_file_dialog.h"

#include "widget_keyboard.h"
#include "widget_line_edit.h"
#include "widget_push_button.h"

#include "screen.h" // for text drawing font, for now


/*******************************************************************************
******************************************************************************/
void
WidgetFileDialog::eventPaint()
{
	// TODO Fix this.
	COLORREF colour = RGB(0, 0, 128);
	RECT screenRect = reinterpret_cast<const RECT&>(getGlobalBounds());
	getBackBuffer().FillRect(
		&screenRect,
		colour,
		0,
		NULL);

	// TEMP Draw dummy title text.
	getBackBuffer().DrawText(
		8,
		8,
		_T("Save Model As"),
		Screen::getSystemFont(),
		0,
		NULL,
		0,
		NULL);

	// TEMP Draw a dummy browser rect.
	RECT browserRect;
	browserRect.left = 32;
	browserRect.right = 240 - 32;
	browserRect.top = 32;
	browserRect.bottom = 192;
	getBackBuffer().FillRect(
		&browserRect,
		RGB(0, 0, 64),
		0,
		NULL);
	// TEMP Draw dummy text in browser.
	getBackBuffer().DrawText(
		34,
		68,
		_T("This area will be a file browser."),
		Screen::getSystemFont(),
		0,
		NULL,
		0,
		NULL);
	getBackBuffer().DrawText(
		34,
		78,
		_T("Until then, manually enter"),
		Screen::getSystemFont(),
		0,
		NULL,
		0,
		NULL);
	getBackBuffer().DrawText(
		34,
		88,
		_T("the file name."),
		Screen::getSystemFont(),
		0,
		NULL,
		0,
		NULL);
	getBackBuffer().DrawText(
		34,
		98,
		_T("Do not add any extension."),
		Screen::getSystemFont(),
		0,
		NULL,
		0,
		NULL);
	getBackBuffer().DrawText(
		34,
		108,
		_T("You will *not* be prompted"),
		Screen::getSystemFont(),
		0,
		NULL,
		0,
		NULL);
	getBackBuffer().DrawText(
		34,
		118,
		_T("for confirmation if you"),
		Screen::getSystemFont(),
		0,
		NULL,
		0,
		NULL);
	getBackBuffer().DrawText(
		34,
		128,
		_T("overwrite an existing file."),
		Screen::getSystemFont(),
		0,
		NULL,
		0,
		NULL);
}


/*******************************************************************************
******************************************************************************/
const tstring&
WidgetFileDialog::getSelectedFileName() const
{
	return m_pLineEdit->getText();
}


/*******************************************************************************
******************************************************************************/
void
WidgetFileDialog::init()
{
	WidgetKeyboard& keyboard = *new WidgetKeyboard(*this, _T("keyboard"));
	keyboard.setPosition(
		WPoint((getWidth() - keyboard.getWidth()) / 2,
			getHeight() - keyboard.getHeight() - 32));
	keyboard.show();

	WidgetLineEdit& lineEdit = *new WidgetLineEdit(*this, _T("line_edit"));
	lineEdit.setBounds(
		WRect(keyboard.getX(), keyboard.getY() - 24, keyboard.getWidth(), 16));
	lineEdit.show();
	// TEMP Set focus, need to do this properly eventually.
	Widget::setFocusWidget(lineEdit);
	m_pLineEdit = &lineEdit;

	WidgetPushButton& buttonCancel = *new WidgetPushButton(*this, _T("Cancel"));
	buttonCancel.setBounds(
		WRect(120 - 32 + 64 + 12, 320 - 24, 64, 16));
	buttonCancel.show();

	WidgetPushButton& buttonOK = *new WidgetPushButton(*this, _T("OK"));
	buttonOK.setBounds(
		WRect(120 - 32, 320 - 24, 64, 16));
	buttonOK.show();

	// Connect signals/slots.
	buttonCancel.clicked.connect(this, &WidgetFileDialog::slotHide);
	buttonCancel.clicked.connect(this, &WidgetFileDialog::fireCancelled);
	buttonOK.clicked.connect(this, &WidgetFileDialog::slotHide);
	buttonOK.clicked.connect(this, &WidgetFileDialog::fireAccepted);
}
