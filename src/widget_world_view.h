// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WIDGET_WORLD_VIEW
#define GUARD__WIDGET_WORLD_VIEW


#include "matrix.h"
#include "widget.h"


/*******************************************************************************
	A lot of the coordinate system conversions assume that this widget is at
	position (0, 0) on the screen.
*******************************************************************************/
class WidgetWorldView : public Widget
{


public:

	WidgetWorldView()
	{
		constructCommon();
	}

	WidgetWorldView(
		const tstring& ksName) :
		Widget(ksName)
	{
		constructCommon();
	}

	WidgetWorldView(
		Widget& parent) :
		Widget(parent)
	{
		constructCommon();
	}

	WidgetWorldView(
		Widget& parent,
		const tstring& ksName) :
		Widget(parent, ksName)
	{
		constructCommon();
	}

	WPoint
	convertToScreen(
		const Vec2& kvPosition) const
	{
		return WPoint(
			static_cast<int>(kvPosition[0]) - m_worldPosition.getX(),
			m_worldPosition.getY() - static_cast<int>(kvPosition[1]));
	}

	WPoint
	convertToScreen(
		const WPoint& kPosition) const
	{
		return WPoint(
			kPosition.getX() - m_worldPosition.getX(),
			m_worldPosition.getY() - kPosition.getY());
	}

	WPoint
	convertToWorld(
		const WPoint& kPosition) const
	{
		// TODO Fix this conversion, it is incorrect.
		return WPoint(
			kPosition.getX() + m_worldPosition.getX(),
			m_worldPosition.getY() - kPosition.getY());
	}

	const WPoint&
	getWorldPosition() const
	{
		return m_worldPosition;
	}


protected:

	virtual
	void
	eventProcess();


private:

	void
	constructCommon()
	{
		m_worldPosition = WPoint(0, 320);
	}


private:

	// The top left point is (0, 0) in local coordinates. This is the
	// corresponding position in world coordinates. Remember in world
	// coordinates, y goes up not down.
	WPoint m_worldPosition;


};


#endif // GUARD
