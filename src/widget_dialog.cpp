// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget_dialog.h"

#include "widget_push_button.h"


/*******************************************************************************
*******************************************************************************/
void
WidgetDialog::constructCommon()
{
	m_pButtonCancel = new WidgetPushButton(*this, _T("cancel"));
	m_pButtonReject = new WidgetPushButton(*this, _T("reject"));
	m_pButtonAccept = new WidgetPushButton(*this, _T("accept"));

	setCancelButtonText(_T("Cancel"));
	setRejectButtonText(_T("No"));
	setAcceptButtonText(_T("Yes"));

	setSize(WDim(240, 320));
	layoutButtons();

	// Connect signals/slots.
	m_pButtonCancel->clicked.connect(this, &WidgetDialog::slotHide);
	m_pButtonCancel->clicked.connect(this, &WidgetDialog::fireCancelled);
	m_pButtonReject->clicked.connect(this, &WidgetDialog::slotHide);
	m_pButtonReject->clicked.connect(this, &WidgetDialog::fireRejected);
	m_pButtonAccept->clicked.connect(this, &WidgetDialog::slotHide);
	m_pButtonAccept->clicked.connect(this, &WidgetDialog::fireAccepted);
}


/*******************************************************************************
*******************************************************************************/
void
WidgetDialog::hideRejectButton()
{
	m_pButtonReject->hide();
	layoutButtons();
}


/*******************************************************************************
*******************************************************************************/
void
WidgetDialog::layoutButtons()
{
	m_pButtonCancel->setSize(WDim(64, 16));
	m_pButtonReject->setSize(WDim(64, 16));
	m_pButtonAccept->setSize(WDim(64, 16));

	int nPosition = 2;

	if (m_pButtonCancel->isShown())
	{
		m_pButtonCancel->setPosition(
			WPoint(12 + nPosition * 76, getHeight() - 24));
		--nPosition;
	}

	if (m_pButtonReject->isShown())
	{
		m_pButtonReject->setPosition(
			WPoint(12 + nPosition * 76, getHeight() - 24));
		--nPosition;
	}

	if (m_pButtonAccept->isShown())
	{
		m_pButtonAccept->setPosition(
			WPoint(12 + nPosition * 76, getHeight() - 24));
		--nPosition;
	}
}


/*******************************************************************************
*******************************************************************************/
void
WidgetDialog::setAcceptButtonText(
	const tstring& ksText)
{
	m_pButtonAccept->setText(ksText);
}


/*******************************************************************************
*******************************************************************************/
void
WidgetDialog::setCancelButtonText(
	const tstring& ksText)
{
	m_pButtonCancel->setText(ksText);
}


/*******************************************************************************
*******************************************************************************/
void
WidgetDialog::setRejectButtonText(
	const tstring& ksText)
{
	m_pButtonReject->setText(ksText);
}
