// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#include "view.h"

#include "render.h"


Rect
View::m_rWorldView;

Rect
View::m_rScreenView;

Mat3
View::m_mTransformW2V;
Mat3
View::m_mTransformV2W;

Mat3
View::m_mTransformV2S;
Mat3
View::m_mTransformS2V;

Mat3
View::m_mTransformW2S;
Mat3
View::m_mTransformS2W;


/*******************************************************************************
*******************************************************************************/
const Rect&
View::getScreenView()
{
	return m_rScreenView;
}


/*******************************************************************************
*******************************************************************************/
const Mat3&
View::getTransformS2W()
{
	return m_mTransformS2W;
}


/*******************************************************************************
*******************************************************************************/
const Mat3&
View::getTransformW2S()
{
	return m_mTransformW2S;
}


/*******************************************************************************
*******************************************************************************/
const Rect&
View::getWorldView()
{
	return m_rWorldView;
}


/*******************************************************************************
*******************************************************************************/
void
View::init()
{
	setViewSize(Vec2(239, 319));
	setScreenViewOrigin(Vec2(0, 0));
	setWorldViewOrigin(Vec2(0, 0));
}


/*******************************************************************************
*******************************************************************************/
void
View::renderView()
{
	// In Q2:
	// clear scene (assume they mean data and not screen?)
	// add entities
		// doesn't add world
		// but adds all other entities
		// goes through cl_parse_entities somehow?

	Render::renderFrame();
}


/*******************************************************************************
*******************************************************************************/
void
View::setScreenViewOrigin(
	const Vec2& kv)
{
	m_rScreenView.getMin() = kv;
	m_rScreenView.getMax() =
		kv + m_rWorldView.getMax() - m_rWorldView.getMin();

	updateScreenTransforms();
}


/*******************************************************************************
*******************************************************************************/
void
View::setViewSize(
	const Vec2& kv)
{
	// PERF this will unnecessarily call the final matrix multiplication twice,
	// but this operation should be uncommon

	m_rWorldView.getMax() = m_rWorldView.getMin() + kv;
	updateWorldTransforms();

	m_rScreenView.getMax() = m_rScreenView.getMin() + kv;
	updateScreenTransforms();
}


/*******************************************************************************
	PERF presumably this is the one that will be called most often.
*******************************************************************************/
void
View::setWorldViewOrigin(
	const Vec2& kv)
{
	m_rWorldView.getMin() = kv;
	m_rWorldView.getMax() =
		kv + m_rScreenView.getMax() - m_rScreenView.getMin();

	updateWorldTransforms();
}


/*******************************************************************************
*******************************************************************************/
void
View::updateCombinedTransforms()
{
	m_mTransformW2S = m_mTransformV2S * m_mTransformW2V;
	m_mTransformS2W = m_mTransformV2W * m_mTransformS2V;
}


/*******************************************************************************
*******************************************************************************/
void
View::updateScreenTransforms()
{
	const scalar kfHalfScreenViewHeight =
		(m_rScreenView.getMax()[1] - m_rScreenView.getMin()[1]) / 2;

	m_mTransformV2S =
		// This matrix translates the y axis back into place, then translates
		// both x and y axes to the screen view origin.
		Mat3(
			1, 0, m_rScreenView.getMin()[0],
			0, 1, m_rScreenView.getMin()[1] + kfHalfScreenViewHeight,
			0, 0, 1) *
		// This matrix translates the y axis to prepare for reflection, then
		// reflects about the y axis.
		Mat3(
			1, 0, 0,
			0, -1, kfHalfScreenViewHeight,
			0, 0, 1);

	m_mTransformS2V =
		Mat3(
			1, 0, 0,
			0, 1, kfHalfScreenViewHeight,
			0, 0, 1) *
		Mat3(
			1, 0, -m_rScreenView.getMin()[0],
			0, -1, m_rScreenView.getMin()[1] + kfHalfScreenViewHeight,
			0, 0, 1);

	updateCombinedTransforms();
}


/*******************************************************************************
*******************************************************************************/
void
View::updateWorldTransforms()
{
	m_mTransformW2V =
		Mat3(
			1, 0, -m_rWorldView.getMin()[0],
			0, 1, -m_rWorldView.getMin()[1],
			0, 0, 1);

	m_mTransformV2W =
		Mat3(
			1, 0, m_rWorldView.getMin()[0],
			0, 1, m_rWorldView.getMin()[1],
			0, 0, 1);

	updateCombinedTransforms();
}
