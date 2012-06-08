// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget.h"

#include <algorithm>


WSurface*
Widget::m_pBackBuffer;

std::vector<Widget*>
Widget::m_cpTopLevelWidget;

Widget*
Widget::m_pActiveWindow = 0;

Widget*
Widget::m_pFocusWidget = 0;

Widget*
Widget::m_pGrabKeyboardWidget = 0;


namespace
{


Widget* pStylusDownWidget = 0;


} // namespace


/*******************************************************************************
*******************************************************************************/
void
Widget::constructCommon()
{
	m_nFocusWidgetIndex = 0;
	m_bEnabled = true;
	m_bVisible = false;
	m_bHidden = false;
	m_bFocusEnabled = false;
}


/*******************************************************************************
*******************************************************************************/
void
Widget::dispatchEventKeyPress(
	const int knKeyCode)
{
	// TODO Check destination widget is enabled and visible.

	if (m_pGrabKeyboardWidget)
	{
		m_pGrabKeyboardWidget->eventKeyPress(knKeyCode);
		return;
	}

	if (!Widget::hasActiveWindow())
	{
		return;
	}

	Widget::getActiveWindow().getFocusWidgetForTree().eventKeyPress(
		knKeyCode);
}


/*******************************************************************************
*******************************************************************************/
void
Widget::dispatchEventKeyRelease(
	const int knKeyCode)
{
	// TODO Check destination widget is enabled and visible.

	if (m_pGrabKeyboardWidget)
	{
		m_pGrabKeyboardWidget->eventKeyRelease(knKeyCode);
		return;
	}

	if (!Widget::hasActiveWindow())
	{
		return;
	}

	Widget::getActiveWindow().getFocusWidgetForTree().eventKeyRelease(
		knKeyCode);
}


/*******************************************************************************
*******************************************************************************/
void
Widget::dispatchEventPaint()
{
	if (Widget::hasActiveWindow())
	{
		Widget::getActiveWindow().eventPaint();
		Widget::getActiveWindow().paintChildren();
	}
}


/*******************************************************************************
*******************************************************************************/
void
Widget::dispatchEventProcess()
{
	if (Widget::hasActiveWindow())
	{
		Widget::getActiveWindow().eventProcess();
		Widget::getActiveWindow().processChildren();
	}
}


/*******************************************************************************
*******************************************************************************/
void
Widget::dispatchEventStylusDoubleClick(
	const WPoint& kScreenPosition)
{
	if (!Widget::hasActiveWindow())
	{
		return;
	}

	Widget* pWidget =
		Widget::getActiveWindow().findGlobalChild(kScreenPosition);

	if (pWidget)
	{
		pWidget->eventStylusDoubleClick(
			pWidget->translateFromGlobal(kScreenPosition));
		pStylusDownWidget = pWidget;
	}
}


/*******************************************************************************
*******************************************************************************/
void
Widget::dispatchEventStylusDown(
	const WPoint& kScreenPosition)
{
	if (!Widget::hasActiveWindow())
	{
		return;
	}

	Widget* pWidget =
		Widget::getActiveWindow().findGlobalChild(kScreenPosition);

	if (pWidget)
	{
		pWidget->eventStylusDown(
			pWidget->translateFromGlobal(kScreenPosition));
		pStylusDownWidget = pWidget;
	}
}


/*******************************************************************************
*******************************************************************************/
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

	if (!Widget::hasActiveWindow())
	{
		return;
	}

	Widget* pWidget =
		Widget::getActiveWindow().findGlobalChild(kScreenPosition);

	if (pWidget)
	{
		pWidget->eventStylusMove(
			pWidget->translateFromGlobal(kScreenPosition));
		pStylusDownWidget = pWidget;
	}
}


/*******************************************************************************
*******************************************************************************/
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
*******************************************************************************/
Widget*
Widget::findGlobalChild(
	const WPoint& kPosition)
{
	if (!getGlobalBounds().contains(kPosition) ||
		!isEnabled() || !isVisible())
	{
		return 0;
	}

	for (int nIndex = getNumberOfChildren() - 1; nIndex != -1; --nIndex)
	{
		Widget* pFound = getChild(nIndex).findGlobalChild(kPosition);
		if (pFound && pFound->isEnabled() && pFound->isVisible())
		{
			return pFound;
		}
	}

	return this;
}


/*******************************************************************************
*******************************************************************************/
void
Widget::focusNext()
{
	Widget& topLevel = getTopLevelAncestor();

	topLevel.m_nFocusWidgetIndex =
		(topLevel.m_nFocusWidgetIndex + 1) % topLevel.m_cpFocusWidget.size();
}


/*******************************************************************************
*******************************************************************************/
Widget*
Widget::getChild(
	const WPoint& kPosition)
{
	
	if (!WRect(WPoint(), getSize()).contains(kPosition))
	{
		return 0;
	}

	for (int nIndex = getNumberOfChildren() - 1; nIndex != -1; --nIndex)
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
*******************************************************************************/
Widget&
Widget::getFocusWidgetForTree()
{
	Widget& topLevel = getTopLevelAncestor();

	return *topLevel.m_cpFocusWidget[topLevel.m_nFocusWidgetIndex];
}


/*******************************************************************************
*******************************************************************************/
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

	// Focus.
	if (&Widget::getFocusWidget() == this)
	{
		focusNext();
	}

	eventHide();

	hideChildren();

	if (isTopLevel())
	{
		updateActiveWindow();
	}
}


/*******************************************************************************
*******************************************************************************/
void
Widget::hideChildren()
{
	for (int nChildIndex = 0;
		nChildIndex != getNumberOfChildren(); ++nChildIndex)
	{
		Widget& child = getChild(nChildIndex);
		if (child.isShown())
		{
			child.m_bVisible = false;
			child.hideChildren();
			child.eventHide();
		}
	}
}


/*******************************************************************************
*******************************************************************************/
void
Widget::paintChildren()
{
	for (int nChildIndex = 0;
		nChildIndex != getNumberOfChildren(); ++nChildIndex)
	{
		Widget& child = getChild(nChildIndex);
		if (child.isVisible())
		{
			child.eventPaint();
			child.paintChildren();
		}
	}
}


/*******************************************************************************
*******************************************************************************/
void
Widget::processChildren()
{
	for (int nChildIndex = 0;
		nChildIndex != getNumberOfChildren(); ++nChildIndex)
	{
		Widget& child = getChild(nChildIndex);
		if (child.isVisible())
		{
			child.eventProcess();
			child.processChildren();
		}
	}
}


/*******************************************************************************
*******************************************************************************/
void
Widget::raise()
{
	if (isTopLevel())
	{
		m_cpTopLevelWidget.erase(
			std::find(
				m_cpTopLevelWidget.begin(),
				m_cpTopLevelWidget.end(),
				this));
		m_cpTopLevelWidget.push_back(this);
		updateActiveWindow();
	}
	else
	{
		// TODO Implement raising for child widgets.
	}
}


/*******************************************************************************
*******************************************************************************/
void
Widget::setFocusEnabled(
	const bool kbFocusEnabled)
{
	if (m_bFocusEnabled == kbFocusEnabled)
	{
		return;
	}

	Widget& topLevel = getTopLevelAncestor();
	if (kbFocusEnabled)
	{
		// Add to focus order.
		topLevel.m_cpFocusWidget.push_back(this);
	}
	else
	{
		// Remove from focus order.
		topLevel.m_cpFocusWidget.erase(
			std::find(
				topLevel.m_cpFocusWidget.begin(),
				topLevel.m_cpFocusWidget.end(),
				this));
	}

	m_bFocusEnabled = kbFocusEnabled;
}


/*******************************************************************************
*******************************************************************************/
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

	eventShow();

	if (isTopLevel())
	{
		updateActiveWindow();
	}
}


/*******************************************************************************
*******************************************************************************/
void
Widget::showChildren()
{
	for (int nChildIndex = 0;
		nChildIndex != getNumberOfChildren(); ++nChildIndex)
	{
		Widget& child = getChild(nChildIndex);
		if (child.isShown())
		{
			child.show();
		}
	}
}


/*******************************************************************************
*******************************************************************************/
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


/*******************************************************************************
*******************************************************************************/
void
Widget::updateActiveWindow()
{
	m_pActiveWindow = 0;

	for (int n = m_cpTopLevelWidget.size() - 1; n != -1; --n)
	{
		if (m_cpTopLevelWidget[n]->isVisible())
		{
			Widget::setActiveWindow(*m_cpTopLevelWidget[n]);
			break;
		}
	}

	// TODO Update focus widget.
}
