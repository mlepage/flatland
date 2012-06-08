// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget.h"

#include "widget_dialog.h"


CGapiSurface*
Widget::m_pBackBuffer;

std::vector<Widget*>
Widget::m_cpTopLevelWidget;

std::vector<WidgetDialog*>
Widget::m_cpDialog;

std::vector<WidgetDialog*>
Widget::m_cpVisibleDialog;

Widget*
Widget::m_pFocusWidget = 0;


namespace
{


Widget* pStylusDownWidget = 0;


} // namespace


/*******************************************************************************
******************************************************************************/
void
Widget::dispatchEventKeyPress(
	const int knKeyCode)
{
	if (!m_pFocusWidget)
	{
		return;
	}

	m_pFocusWidget->eventKeyPress(knKeyCode);
}


/*******************************************************************************
******************************************************************************/
void
Widget::dispatchEventKeyRelease(
	const int knKeyCode)
{
	if (!m_pFocusWidget)
	{
		return;
	}

	m_pFocusWidget->eventKeyRelease(knKeyCode);
}


/*******************************************************************************
******************************************************************************/
void
Widget::dispatchEventPaint()
{
	// Special dialog handling.
	if (m_cpVisibleDialog.size())
	{
		Widget& dialog = *m_cpVisibleDialog.back();
		dialog.eventPaint();
		dialog.paintChildren();
		return;
	}

	// Simply dispatch a paint event to all widgets. Crude but effective.
	for (int n = 0; n != m_cpTopLevelWidget.size(); ++n)
	{
		Widget& widget = *m_cpTopLevelWidget[n];
		if (widget.isVisible())
		{
			widget.eventPaint();
			widget.paintChildren();
		}
	}
}


/*******************************************************************************
	TODO Eventually I should rewrite these mouse event dispatch loops.
	I should create a findChild function which uses the new global bounds each
	widget has (faster and easier). I should also use the Widget& in the loops.
******************************************************************************/
void
Widget::dispatchEventStylusDoubleClick(
	const WPoint& kScreenPosition)
{
	// Special dialog handling.
	if (m_cpVisibleDialog.size())
	{
		Widget& widget = *m_cpVisibleDialog.back();
		if (Widget* pWidget =
			widget.getChild(
				widget.translateFromParent(
					kScreenPosition)))
		{
			pWidget->eventStylusDoubleClick(
				pWidget->translateFromGlobal(kScreenPosition));
		}
		return;
	}

	for (int n = 0; n != m_cpTopLevelWidget.size(); ++n)
	{
		Widget& widget = *m_cpTopLevelWidget[n];
		if (widget.isVisible())
		{
			if (Widget* pWidget =
				m_cpTopLevelWidget[n]->getChild(
					m_cpTopLevelWidget[n]->translateFromParent(
						kScreenPosition)))
			{
				pWidget->eventStylusDoubleClick(
					pWidget->translateFromGlobal(kScreenPosition));
				break;
			}
		}
	}
}


/*******************************************************************************
******************************************************************************/
void
Widget::dispatchEventStylusDown(
	const WPoint& kScreenPosition)
{
	// Special dialog handling.
	if (m_cpVisibleDialog.size())
	{
		Widget& widget = *m_cpVisibleDialog.back();
		if (Widget* pWidget =
			widget.getChild(
				widget.translateFromParent(
					kScreenPosition)))
		{
			pWidget->eventStylusDown(
				pWidget->translateFromGlobal(kScreenPosition));
			pStylusDownWidget = pWidget;
		}
		return;
	}

	for (int n = 0; n != m_cpTopLevelWidget.size(); ++n)
	{
		Widget& widget = *m_cpTopLevelWidget[n];
		if (widget.isVisible())
		{
			if (Widget* pWidget =
				m_cpTopLevelWidget[n]->getChild(
					m_cpTopLevelWidget[n]->translateFromParent(
						kScreenPosition)))
			{
				pWidget->eventStylusDown(
					pWidget->translateFromGlobal(kScreenPosition));
				pStylusDownWidget = pWidget;
				break;
			}
		}
	}
}


/*******************************************************************************
******************************************************************************/
void
Widget::dispatchEventStylusMove(
	const WPoint& kScreenPosition)
{
	if (pStylusDownWidget)
	{
		pStylusDownWidget->eventStylusMove(
				pStylusDownWidget->translateFromGlobal(kScreenPosition));
		return;
	}

	// Special dialog handling.
	if (m_cpVisibleDialog.size())
	{
		Widget& widget = *m_cpVisibleDialog.back();
		if (Widget* pWidget =
			widget.getChild(
				widget.translateFromParent(
					kScreenPosition)))
		{
			pWidget->eventStylusMove(
				pWidget->translateFromGlobal(kScreenPosition));
		}
		return;
	}

	for (int n = 0; n != m_cpTopLevelWidget.size(); ++n)
	{
		Widget& widget = *m_cpTopLevelWidget[n];
		if (widget.isVisible())
		{
			if (Widget* pWidget =
				m_cpTopLevelWidget[n]->getChild(
					m_cpTopLevelWidget[n]->translateFromParent(
						kScreenPosition)))
			{
				pWidget->eventStylusMove(
					pWidget->translateFromGlobal(kScreenPosition));
				break;
			}
		}
	}
}


/*******************************************************************************
******************************************************************************/
void
Widget::dispatchEventStylusUp(
	const WPoint& kScreenPosition)
{
	if (pStylusDownWidget)
	{
		pStylusDownWidget->eventStylusUp(
				pStylusDownWidget->translateFromGlobal(kScreenPosition));
		pStylusDownWidget = 0;
	}
}


/*******************************************************************************
******************************************************************************/
Widget*
Widget::getChild(
	const WPoint& kPosition)
{
	if (!WRect(WPoint(), getSize()).contains(kPosition))
	{
		return 0;
	}

	for (int nIndex = 0; nIndex != getNumberOfChildren(); ++nIndex)
	{
		Widget& child = getChild(nIndex);
		Widget* pChild = child.getChild(child.translateFromParent(kPosition));
		if (pChild)
		{
			return pChild;
		}
	}

	return this;
}


/*******************************************************************************
******************************************************************************/
void
Widget::hide()
{
	if (isHidden())
	{
		return;
	}

	m_bHidden = true;

	if (!isVisible())
	{
		// Already not visible.
		return;
	}

	m_bVisible = false;

	hideChildren();

	// Special dialog handling.
	for (std::vector<WidgetDialog*>::iterator i = m_cpVisibleDialog.begin();
		i != m_cpVisibleDialog.end(); ++i)
	{
		if (this == *i)
		{
			m_cpVisibleDialog.erase(i);
			break;
		}
	}
}


/*******************************************************************************
******************************************************************************/
void
Widget::hideChildren()
{
	for (int nChildIndex = 0;
		nChildIndex != getNumberOfChildren(); ++nChildIndex)
	{
		getChild(nChildIndex).m_bVisible = false;
		getChild(nChildIndex).hideChildren();
	}
}


/*******************************************************************************
******************************************************************************/
void
Widget::paintChildren()
{
	// TODO Check visibility.

	for (int nChildIndex = 0;
		nChildIndex != getNumberOfChildren(); ++nChildIndex)
	{
		getChild(nChildIndex).eventPaint();
		getChild(nChildIndex).paintChildren();
	}
}


/*******************************************************************************
******************************************************************************/
void
Widget::show()
{
	if (isVisible())
	{
		return;
	}

	m_bHidden = false;

	if (hasParent() && !getParent().isVisible())
	{
		// Don't set visible because parent is not visible.
		return;
	}

	m_bVisible = true;

	showChildren();

	// Special dialog handling.
	for (int nDialogIndex = 0;
		nDialogIndex != m_cpDialog.size(); ++nDialogIndex)
	{
		if (this == m_cpDialog[nDialogIndex])
		{
			m_cpVisibleDialog.push_back(static_cast<WidgetDialog*>(this));
			break;
		}
	}
}


/*******************************************************************************
******************************************************************************/
void
Widget::showChildren()
{
	for (int nChildIndex = 0;
		nChildIndex != getNumberOfChildren(); ++nChildIndex)
	{
		getChild(nChildIndex).show();
	}
}


/*******************************************************************************
******************************************************************************/
WPoint
Widget::translateFromGlobal(
	const WPoint& kPosition) const
{
	WPoint position(kPosition);
	const Widget* pkWidget = this;
	while (pkWidget->hasParent())
	{
		position = pkWidget->translateFromParent(position);
		pkWidget = &pkWidget->getParent();
	}
	return pkWidget->translateFromParent(position);
}
