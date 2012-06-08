// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__VIEW
#define GUARD__VIEW


#include "matrix.h"
#include "rect.h"


/*******************************************************************************
	The view is the part of the screen that will render the world.
  
	The view exists in two coordinate systems. First, it exists in the world
	coordinate system. Then, it exists in the screen coordinate system. It can
	move in either, but must be the same dimensions.

	See Foley 5.4.
*******************************************************************************/
class View
{


public:

	static
	const Rect&
	getScreenView();

	static
	const Mat3&
	getTransformW2S();

	static
	const Mat3&
	getTransformS2W();

	static
	const Rect&
	getWorldView();

	static
	void
	init();

	static
	void
	renderView();

	static
	void
	setScreenViewOrigin(
		const Vec2& kv);

	static
	void
	setViewSize(
		const Vec2& kv);

	static
	void
	setWorldViewOrigin(
		const Vec2& kv);

private:

	static
	void
	updateCombinedTransforms();

	static
	void
	updateScreenTransforms();

	static
	void
	updateWorldTransforms();

	// The view in world coordinates.
	static
	Rect m_rWorldView;

	// The view in screen coordinates.
	static
	Rect m_rScreenView;

	// Transformation matrices: world and view.
	static
	Mat3 m_mTransformW2V;
	static
	Mat3 m_mTransformV2W;

	// Transformation matrices: view and screen.
	static
	Mat3 m_mTransformV2S;
	static
	Mat3 m_mTransformS2V;

	// Transformation matrices: world and screen.
	static
	Mat3 m_mTransformW2S;
	static
	Mat3 m_mTransformS2W;

};


#endif // GUARD
