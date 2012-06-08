// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET
#define GUARD__WIDGET


#include <vector>
#include "sigslot.h"
#include "GapiDraw.h"

#include "tstring.h"


typedef COLORREF WColour;
typedef CGapiSurface WSurface;


/*******************************************************************************
*******************************************************************************/
class Colour
{

public:

	static
	WColour
	black()
	{
		return RGB(0, 0, 0);
	}

	static
	WColour
	highlight()
	{
		return RGB(128 + 64, 128 + 64, 0);
	}

	static
	WColour
	white()
	{
		return RGB(255, 255, 255);
	}


};


/*******************************************************************************
*******************************************************************************/
class WPoint
{


public:

	WPoint() :
		m_nX(0),
		m_nY(0)
	{
	}

	WPoint(
		const int knX,
		const int knY) :
		m_nX(knX),
		m_nY(knY)
	{
	}

	int
	getX() const
	{
		return m_nX;
	}

	int
	getY() const
	{
		return m_nY;
	}

	void
	setX(
		const int knX)
	{
		m_nX = knX;
	}

	void
	setY(
		const int knY)
	{
		m_nY = knY;
	}

	void
	translate(
		const WPoint& kPositionDelta)
	{
		m_nX += kPositionDelta.getX();
		m_nY += kPositionDelta.getY();
	}


private:

	int m_nX;
	int m_nY;


};


/*******************************************************************************
*******************************************************************************/
class WDim
{

public:

	WDim() :
		m_nWidth(0),
		m_nHeight(0)
	{
	}

	WDim(
		const int knWidth,
		const int knHeight) :
		m_nWidth(knWidth),
		m_nHeight(knHeight)
	{
	}

	int
	getHeight() const
	{
		return m_nHeight;
	}

	int
	getWidth() const
	{
		return m_nWidth;
	}

	void
	setHeight(
		const int knHeight)
	{
		m_nHeight = knHeight;
	}

	void
	setWidth(
		const int knWidth)
	{
		m_nWidth = knWidth;
	}


private:

	int m_nWidth;
	int m_nHeight;


};


/*******************************************************************************
*******************************************************************************/
class WRect
{


public:

	WRect() :
		m_nX1(0),
		m_nY1(0),
		m_nX2(0),
		m_nY2(0)
	{
	}

	WRect(
		const int knX,
		const int knY,
		const int knWidth,
		const int knHeight) :
		m_nX1(knX),
		m_nY1(knY),
		m_nX2(knX + knWidth),
		m_nY2(knY + knHeight)
	{
	}

	WRect(
		const WPoint& kPosition,
		const WDim& kSize) :
		m_nX1(kPosition.getX()),
		m_nY1(kPosition.getY()),
		m_nX2(kPosition.getX() + kSize.getWidth()),
		m_nY2(kPosition.getY() + kSize.getHeight())
	{
	}

	bool
	contains(
		const WPoint& kPosition) const
	{
		return
			m_nX1 <= kPosition.getX() && kPosition.getX() <= m_nX2 &&
			m_nY1 <= kPosition.getY() && kPosition.getY() <= m_nY2;
	}

	int
	getHeight() const
	{
		return m_nY2 - m_nY1;
	}

	WPoint
	getPosition() const
	{
		return WPoint(m_nX1, m_nY1);
	}

	WDim
	getSize() const
	{
		return WDim(m_nX2 - m_nX1, m_nY2 - m_nY1);
	}

	int
	getWidth() const
	{
		return m_nX2 - m_nX1;
	}

	int
	getX() const
	{
		return m_nX1;
	}

	int
	getY() const
	{
		return m_nY1;
	}

	void
	setHeight(
		const int knHeight)
	{
		m_nY2 = m_nY1 + knHeight;
	}

	void
	setPosition(
		const WPoint& kPosition)
	{
		m_nX2 += kPosition.getX() - m_nX1;
		m_nX1 = kPosition.getX();
		m_nY2 += kPosition.getY() - m_nY1;
		m_nY1 = kPosition.getY();
	}

	void
	setSize(
		const WDim& kSize)
	{
		m_nX2 = m_nX1 + kSize.getWidth();
		m_nY2 = m_nY1 + kSize.getHeight();
	}

	void
	setWidth(
		const int knWidth)
	{
		m_nX2 = m_nX1 + knWidth;
	}

	void
	setX(
		const int knX)
	{
		m_nX2 += knX - m_nX1;
		m_nX1 = knX;
	}

	void
	setY(
		const int knY)
	{
		m_nY2 += knY - m_nY1;
		m_nY1 = knY;
	}


private:

	int m_nX1;
	int m_nY1;
	int m_nX2;
	int m_nY2;


};


/*******************************************************************************
	Geometry is in the coordinate system of the parent; global geometry is in
	the global coordinate system.
*******************************************************************************/
class Widget : public sigslot::has_slots<>
{


public:

	static
	void
	dispatchEventKeyPress(
		const int knKeyCode);

	static
	void
	dispatchEventKeyRelease(
		const int knKeyCode);

	static
	void
	dispatchEventPaint();

	static
	void
	dispatchEventProcess();

	static
	void
	dispatchEventStylusDoubleClick(
		const WPoint& kScreenPosition);

	static
	void
	dispatchEventStylusDown(
		const WPoint& kScreenPosition);

	static
	void
	dispatchEventStylusMove(
		const WPoint& kScreenPosition);

	static
	void
	dispatchEventStylusUp(
		const WPoint& kScreenPosition);

	static
	Widget&
	getActiveWindow()
	{
		return *m_pActiveWindow;
	}

	static
	WSurface&
	getBackBuffer()
	{
		return *m_pBackBuffer;
	}

	static
	Widget&
	getFocusWidget()
	{
		return *m_pFocusWidget;
	}

	static
	bool
	hasActiveWindow()
	{
		return m_pActiveWindow != 0;
	}

	// This is basically a hack for now to determine whether to send events to
	// non-widget event processing.
	static
	bool
	isOtherEventDispatchingEnabled()
	{
		return m_pActiveWindow == 0;
	}

	static
	void
	setActiveWindow(
		Widget& widget)
	{
		m_pActiveWindow = &widget;
	}

	static
	void
	setBackBuffer(
		WSurface& backBuffer)
	{
		m_pBackBuffer = &backBuffer;
	}


public:

	Widget() :
		m_pParent(0)
	{
		constructCommon();
		addTopLevelWidget(*this);
	}

	Widget(
		const tstring& ksName) :
		m_pParent(0)
	{
		constructCommon();
		addTopLevelWidget(*this);
	}

	Widget(
		Widget& parent) :
		m_pParent(&parent)
	{
		constructCommon();
		parent.addChild(*this);
	}

	Widget(
		Widget& parent,
		const tstring& ksName) :
		m_pParent(&parent),
		m_sName(ksName)
	{
		constructCommon();
		parent.addChild(*this);
	}

	~Widget()
	{
		// TODO Clean up.
	}

	Widget*
	findGlobalChild(
		const WPoint& kPosition);

	void
	focusNext();

	WRect
	getBounds() const
	{
		return WRect(m_position, m_size);
	}

	Widget&
	getChild(
		const int knIndex) const
	{
		return *m_cpChild[knIndex];
	}

	Widget*
	getChild(
		const WPoint& kPosition);

	WRect
	getGlobalBounds() const
	{
		return WRect(m_positionGlobal, m_size);
	}

	WPoint
	getGlobalPosition() const
	{
		return m_positionGlobal;
	}

	int
	getGlobalX() const
	{
		return m_positionGlobal.getX();
	}

	int
	getGlobalY() const
	{
		return m_positionGlobal.getY();
	}

	int
	getHeight() const
	{
		return m_size.getHeight();
	}

	const tstring&
	getName() const
	{
		return m_sName;
	}

	int
	getNumberOfChildren() const
	{
		return m_cpChild.size();
	}

	Widget&
	getParent() const
	{
		return *m_pParent;
	}

	WPoint
	getPosition() const
	{
		return m_position;
	}

	WDim
	getSize() const
	{
		return m_size;
	}

	Widget&
	getTopLevelAncestor()
	{
		Widget* pWidget = this;
		while (!pWidget->isTopLevel())
		{
			pWidget = &pWidget->getParent();
		}
		return *pWidget;
	}

	int
	getWidth() const
	{
		return m_size.getWidth();
	}

	int
	getX() const
	{
		return m_position.getX();
	}

	int
	getY() const
	{
		return m_position.getY();
	}

	void
	grabKeyboard()
	{
		Widget::m_pGrabKeyboardWidget = this;
	}

	bool
	hasChildren() const
	{
		return !m_cpChild.empty();
	}

	bool
	hasParent() const
	{
		return m_pParent != 0;
	}

	void
	hide();

	bool
	isEnabled() const
	{
		return m_bEnabled;
	}

	bool
	isFocusEnabled() const
	{
		return m_bFocusEnabled;
	}

	bool
	isHidden() const
	{
		return m_bHidden;
	}

	bool
	isShown() const
	{
		return !m_bHidden;
	}

	bool
	isVisible() const
	{
		return m_bVisible;
	}

	bool
	isTopLevel() const
	{
		// For now, a top level widget is one without a parent.
		return !hasParent();
	}

	void
	raise();

	void
	releaseKeyboard()
	{
		Widget::m_pGrabKeyboardWidget = 0;
	}

	void
	setBounds(
		const WRect& kBounds)
	{
		setPosition(kBounds.getPosition());
		setSize(kBounds.getSize());
	}

	void
	setEnabled(
		const bool kbEnabled)
	{
		m_bEnabled = kbEnabled;
	}

	void
	setFocusEnabled(
		const bool kbFocusEnabled);

	void
	setHeight(
		const int knHeight)
	{
		m_size.setHeight(knHeight);
	}

	void
	setHidden(
		const bool kbHidden)
	{
		if (kbHidden)
		{
			hide();
		}
		else
		{
			show();
		}
	}

	void
	setPosition(
		const WPoint& kPosition)
	{
		const WPoint kPositionDelta(
			kPosition.getX() - m_position.getX(),
			kPosition.getY() - m_position.getY());
		translateGlobalPosition(kPositionDelta);
		m_position = kPosition;
	}

	void
	setShown(
		const bool kbShown)
	{
		if (kbShown)
		{
			show();
		}
		else
		{
			hide();
		}
	}

	void
	setSize(
		const WDim& kSize)
	{
		m_size = kSize;
	}

	void
	setWidth(
		const int knWidth)
	{
		m_size.setWidth(knWidth);
	}

	void
	setX(
		const int knX)
	{
		setPosition(WPoint(knX, getY()));
	}

	void
	setY(
		const int knY)
	{
		setPosition(WPoint(getX(), knY));
	}

	void
	show();

	virtual void slotHide()
	{
		hide();
	}

	virtual void slotShow()
	{
		show();
	}

	WPoint
	translateFromGlobal(
		const WPoint& kPosition) const;

	WPoint
	translateFromParent(
		const WPoint& kPosition) const
	{
		return WPoint(kPosition.getX() - getX(), kPosition.getY() - getY());
	}


protected:

	// Not used.
	virtual
	void
	eventFocusIn() {};

	// Not used.
	virtual
	void
	eventFocusOut() {};

	virtual
	void
	eventHide() {};

	virtual
	void
	eventKeyPress(
		const int knKeyCode) {};

	virtual
	void
	eventKeyRelease(
		const int knKeyCode) {};

	virtual
	void
	eventPaint() {};

	virtual
	void
	eventProcess() {};

	virtual
	void
	eventShow() {};

	virtual
	void
	eventStylusDoubleClick(
		const WPoint& kPosition) {};

	virtual
	void
	eventStylusDown(
		const WPoint& kPosition) {};

	virtual
	void
	eventStylusMove(
		const WPoint& kPosition) {};

	virtual
	void
	eventStylusUp(
		const WPoint& kPosition) {};


private:

	static
	void
	addTopLevelWidget(
		Widget& widget)
	{
		m_cpTopLevelWidget.push_back(&widget);
	}

	static
	void
	updateActiveWindow();


private:

	static WSurface* m_pBackBuffer;

	// PERF If performance checking for visibility seems to be an issue, I can
	// just maintain a separate list of visible top level widgets.
	static std::vector<Widget*> m_cpTopLevelWidget;

	static Widget* m_pActiveWindow;

	static Widget* m_pFocusWidget;

	static Widget* m_pGrabKeyboardWidget;


private:

	void
	addChild(
		Widget& child)
	{
		m_cpChild.push_back(&child);
		WPoint positionGlobal = m_positionGlobal;
		positionGlobal.translate(child.getPosition());
		child.setGlobalPosition(positionGlobal);
	}

	void
	constructCommon();

	Widget&
	getFocusWidgetForTree();

	void
	hideChildren();

	void
	paintChildren();

	void
	processChildren();

	void
	setGlobalPosition(
		const WPoint& kPosition)
	{
		translateGlobalPosition(
			WPoint(
				kPosition.getX() - m_positionGlobal.getX(),
				kPosition.getY() - m_positionGlobal.getY()));
	}

	void
	showChildren();

	void
	translateGlobalPosition(
		const WPoint& kPositionDelta)
	{
		m_positionGlobal.translate(kPositionDelta);
		for (int nChildIndex = 0;
			nChildIndex != getNumberOfChildren(); ++nChildIndex)
		{
			getChild(nChildIndex).translateGlobalPosition(kPositionDelta);
		}
	}


private:

	Widget* m_pParent;
	// Children in z-order, so last child is on top.
	std::vector<Widget*> m_cpChild;
	tstring m_sName;

	// Geometry.
	WPoint m_positionGlobal;
	WPoint m_position;
	WDim m_size;

	// Focus. (Only for top level widgets.)
	int m_nFocusWidgetIndex;
	std::vector<Widget*> m_cpFocusWidget;

	bool m_bEnabled;
	bool m_bVisible;
	bool m_bHidden;
	bool m_bFocusEnabled;


};


#endif // GUARD
