// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "stylus.h"


bool
Stylus::m_bDown = false;

Vec2
Stylus::m_vAnchorPoint;

Vec2
Stylus::m_vLeadPoint;


/*******************************************************************************
*******************************************************************************/
void
Stylus::down()
{
	m_bDown = true;
}


/*******************************************************************************
*******************************************************************************/
const Vec2&
Stylus::getAnchorPoint()
{
	return m_vAnchorPoint;
}


/*******************************************************************************
*******************************************************************************/
const Vec2&
Stylus::getLeadPoint()
{
	return m_vLeadPoint;
}


/*******************************************************************************
*******************************************************************************/
bool
Stylus::isDown()
{
	return m_bDown;
}


/*******************************************************************************
*******************************************************************************/
void
Stylus::setAnchorPoint(
	const Vec2& kvPoint)
{
	m_vAnchorPoint = kvPoint;
}


/*******************************************************************************
*******************************************************************************/
void
Stylus::setLeadPoint(
	const Vec2& kvPoint)
{
	m_vLeadPoint = kvPoint;
}


/*******************************************************************************
*******************************************************************************/
void
Stylus::up()
{
	m_bDown = false;
}
