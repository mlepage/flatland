// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "widget_world_view.h"

#include "key.h"


/*******************************************************************************
*******************************************************************************/
void
WidgetWorldView::eventProcess()
{
	const int knScroll = 10;

	if (Key::isKeyDown(KeyCode::up()))
	{
		m_worldPosition.translate(WPoint(0, knScroll));
	}
	if (Key::isKeyDown(KeyCode::down()))
	{
		m_worldPosition.translate(WPoint(0, -knScroll));
	}
	if (Key::isKeyDown(KeyCode::left()))
	{
		m_worldPosition.translate(WPoint(-knScroll, 0));
	}
	if (Key::isKeyDown(KeyCode::right()))
	{
		m_worldPosition.translate(WPoint(knScroll, 0));
	}
}
