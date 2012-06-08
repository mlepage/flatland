// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__RENDER
#define GUARD__RENDER


#include "GapiDraw.h"


/*******************************************************************************
	The render engine renders the view of the world into the viewport on the
	screen. Basically, it draws the brushes of each entity, including the world.

	(Q2 is a bit different, the world is drawn separately, I still have to
	figure it out.)

	TODO does drawLine need to be in the header?
*******************************************************************************/
class Render
{


public:

	static
	void
	drawLine(
		double x0,
		double y0,
		double x1,
		double y1,
		const double xmin,
		const double xmax,
		const double ymin,
		const double ymax,
		COLORREF dwColor);

	static
	void
	renderFrame();


private:


};


#endif // GUARD
