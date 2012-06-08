// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#include "render.h"

#include "game.h"
#include "geometry.h"
#include "entity.h"
#include "model.h"
#include "rect.h"
#include "screen.h"
#include "variable.h"
#include "view.h"
#include "world.h"

// testing
#include <cmath>


typedef unsigned int outcode;
enum
{
	TOP = 0x1,
	BOTTOM = 0x2,
	RIGHT = 0x4,
	LEFT = 0x8
};


outcode
compOutCode(
	double x,
	double y,
	double xmin,
	double xmax,
	double ymin,
	double ymax)
{
	outcode code = 0;

	if (y > ymax)
		code |= TOP;
	else if (y < ymin)
		code |= BOTTOM;

	if (x > xmax)
		code |= RIGHT;
	else if (x < xmin)
		code |= LEFT;

	return code;
}


/*******************************************************************************
	Cohen-Sutherland clip and draw line. Foley 3.12.3.
*******************************************************************************/
void
Render::drawLine(
	double x0,
	double y0,
	double x1,
	double y1,
	const double xmin,
	const double xmax,
	const double ymin,
	const double ymax,
	COLORREF dwColor)
{
#ifdef FLATLAND_ENABLE_CUSTOM_LINE_CLIPPING
	outcode outcode0;
	outcode outcode1;
	outcode outcodeOut;

	bool accept(false);
	bool done(false);

	outcode0 =
		compOutCode(
			x0, y0, xmin, xmax, ymin, ymax);
	outcode1 =
		compOutCode(
			x1, y1, xmin, xmax, ymin, ymax);

	do
	{
		if (!(outcode0 | outcode1))
		{
			// Trivial accept and exit.
			accept = true;
			done = true;
		}
		else if (outcode0 & outcode1)
		{
			// Logical and is true, so trivial reject and exit.
			done = true;
		}
		else
		{
			// Failed both tests, so calculate the line segment to clip,
			// from an outside point to an intersection with clip edge.

			double x;
			double y;

			// At least one endpoint is outside the clip rectangle; pick it.
			outcodeOut = outcode0 ? outcode0 : outcode1;

			// Now find intersection point.
			if (outcodeOut & TOP)
			{
				// Divide line at top edge of clip rect.
				x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
				y = ymax;
			}
			else if (outcodeOut & BOTTOM)
			{
				// Divide line at bottom edge of clip rect.
				x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
				y = ymin;
			}
			else if (outcodeOut & RIGHT)
			{
				// Divide line at right edge of clip rect.
				y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
				x = xmax;
			}
			else
			{
				// Divide line at left edge of clip rect.
				y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
				x = xmin;
			}

			// Now we move outside point to intersection point to clip,
			// and get ready for next pass.
			if (outcodeOut == outcode0)
			{
				x0 = x;
				y0 = y;
				outcode0 = compOutCode(x0, y0, xmin, xmax, ymin, ymax);
			}
			else
			{
				x1 = x;
				y1 = y;
				outcode1 = compOutCode(x1, y1, xmin, xmax, ymin, ymax);
			}
		}
	} while (done == false);

	if (accept)
	{
		CGapiSurface* pBackBuffer = Screen::getBackBuffer();
		pBackBuffer->DrawLine(
			x0,
			y0,
			x1,
			y1,
			dwColor,
			0,
			NULL);
	}
#else
		// Rely on built-in line clipping.
		CGapiSurface* pBackBuffer = Screen::getBackBuffer();
		pBackBuffer->DrawLine(
			x0,
			y0,
			x1,
			y1,
			dwColor,
			0,
			NULL);
#endif
}


/*******************************************************************************
*******************************************************************************/
void
Render::init()
{
}


/*******************************************************************************
	In Q2, this function takes a refdef_t which tells which entities to draw,
	the screen viewport to draw into, the world position to draw, etc.

	For expediency, I will just grab them from where they are. I don't have
	quite as much modularization since I don't need it.
*******************************************************************************/
void
Render::renderFrame()
{
	CGapiSurface* pBackBuffer = Screen::getBackBuffer();

	const Rect& krScreenView = View::getScreenView();
	const Rect& krWorldView = View::getWorldView();
	const Mat3& kmTransformW2S =
		View::getTransformW2S();

	for (int nEntityIndex(0); nEntityIndex != Game::getNumberOfEntities();
		++nEntityIndex)
	{
		const Entity& kEntity = Game::getEntity(nEntityIndex);

		// Check bounding box against world view.
		if (!Geometry::isRectTouchingRect(kEntity.getBounds(), krWorldView))
		{
			continue;
		}

if (Variable::render_entity_bounds.getFloatValue())
{
		// TEST draw entity bounds.
		{
			COLORREF dwBoundsColour = RGB(63, 0, 0);
			Vec2 v1 =
				Geometry::transformPoint(
					View::getTransformW2S(),
					kEntity.getBounds().getMin());
			Vec2 v2 =
				Geometry::transformPoint(
					View::getTransformW2S(),
					Vec2(
						kEntity.getBounds().getMin()[0],
						kEntity.getBounds().getMax()[1]));
			Vec2 v3 =
				Geometry::transformPoint(
					View::getTransformW2S(),
					kEntity.getBounds().getMax());
			Vec2 v4 =
				Geometry::transformPoint(
					View::getTransformW2S(),
					Vec2(
						kEntity.getBounds().getMax()[0],
						kEntity.getBounds().getMin()[1]));
				drawLine(
					v1[0],
					v1[1],
					v2[0],
					v2[1],
					krScreenView.getMin()[0],
					krScreenView.getMax()[0],
					krScreenView.getMin()[1],
					krScreenView.getMax()[1],
					dwBoundsColour);
				drawLine(
					v2[0],
					v2[1],
					v3[0],
					v3[1],
					krScreenView.getMin()[0],
					krScreenView.getMax()[0],
					krScreenView.getMin()[1],
					krScreenView.getMax()[1],
					dwBoundsColour);
				drawLine(
					v3[0],
					v3[1],
					v4[0],
					v4[1],
					krScreenView.getMin()[0],
					krScreenView.getMax()[0],
					krScreenView.getMin()[1],
					krScreenView.getMax()[1],
					dwBoundsColour);
				drawLine(
					v4[0],
					v4[1],
					v1[0],
					v1[1],
					krScreenView.getMin()[0],
					krScreenView.getMax()[0],
					krScreenView.getMin()[1],
					krScreenView.getMax()[1],
					dwBoundsColour);
		}
}

		// PERF
		// For immobile entities, it may be faster to skip the entity bounds
		// check and just go straight into the model bounds check (since they
		// are the same). That way we won't needlessly repeat a check.

		const Model& kModel = kEntity.getModel();

		const Mat3& kmTransformM2W = kEntity.getTransformM2W();
		const Mat3 kmTransformM2S =
			kEntity.isMobile() ?
				kmTransformW2S * kmTransformM2W :
				kmTransformW2S;

		// TODO need a way to check against model bounding box.

if (Variable::render_model_bounds.getFloatValue())
{
		// TEST draw model bounds.
		{
				COLORREF dwBoundsColour = RGB(0, 63, 0);
				const Rect& rBounds = kModel.getBounds();
				Vec2 v1 =
					Geometry::transformPoint(kmTransformM2S, rBounds.getMin());
				Vec2 v2 =
					Geometry::transformPoint(
						kmTransformM2S,
						Vec2(rBounds.getMin()[0], rBounds.getMax()[1]));
				Vec2 v3 =
					Geometry::transformPoint(kmTransformM2S, rBounds.getMax());
				Vec2 v4 =
					Geometry::transformPoint(
						kmTransformM2S,
						Vec2(rBounds.getMax()[0], rBounds.getMin()[1]));
				drawLine(
					v1[0],
					v1[1],
					v2[0],
					v2[1],
					krScreenView.getMin()[0],
					krScreenView.getMax()[0],
					krScreenView.getMin()[1],
					krScreenView.getMax()[1],
					dwBoundsColour);
				drawLine(
					v2[0],
					v2[1],
					v3[0],
					v3[1],
					krScreenView.getMin()[0],
					krScreenView.getMax()[0],
					krScreenView.getMin()[1],
					krScreenView.getMax()[1],
					dwBoundsColour);
				drawLine(
					v3[0],
					v3[1],
					v4[0],
					v4[1],
					krScreenView.getMin()[0],
					krScreenView.getMax()[0],
					krScreenView.getMin()[1],
					krScreenView.getMax()[1],
					dwBoundsColour);
				drawLine(
					v4[0],
					v4[1],
					v1[0],
					v1[1],
					krScreenView.getMin()[0],
					krScreenView.getMax()[0],
					krScreenView.getMin()[1],
					krScreenView.getMax()[1],
					dwBoundsColour);
		}
}

if (Variable::render_entity_image.getFloatValue())
{
	// Entity image.
	CGapiSurface* pSurface =
		const_cast<CGapiSurface*>(&kEntity.getModel().getImage());
	// Screen origin and size.
	Vec2 vScreenOrigin = View::getScreenView().getMin();
	Vec2 vScreenViewSize =
		View::getScreenView().getMax() - View::getScreenView().getMin();
	// Set the source rect to the view in image coordinates.
	RECT srcRect;
	srcRect.top =
		kEntity.getBounds().getMax()[1] -
		View::getWorldView().getMax()[1];
	srcRect.left =
		View::getWorldView().getMin()[0] -
		kEntity.getBounds().getMin()[0];
	srcRect.bottom = srcRect.top + vScreenViewSize[1];
	srcRect.right = srcRect.left + vScreenViewSize[0];
	// Clip the source rect to the image.
	if (srcRect.top < 0)
	{
		vScreenOrigin[1] -= srcRect.top;
		srcRect.top = 0;
	}
	if (srcRect.left < 0)
	{
		vScreenOrigin[0] -= srcRect.left;
		srcRect.left = 0;
	}
	if (pSurface->GetHeight() < srcRect.bottom)
	{
		srcRect.bottom = pSurface->GetHeight();
	}
	if (pSurface->GetWidth() < srcRect.right)
	{
		srcRect.right = pSurface->GetWidth();
	}
	Screen::getBackBuffer()->BltFast(
		vScreenOrigin[0],
		vScreenOrigin[1],
		pSurface,
		&srcRect,
		GDBLTFAST_KEYSRC,
		NULL);
}

		COLORREF dwColor =
			(&kEntity == Game::getActiveEntity()) ?
				RGB(255, 255, 255) :
				RGB(
					kEntity.getColour()[0],
					kEntity.getColour()[1],
					kEntity.getColour()[2]);

		// Draw entity identifier and velocity.
		if (kEntity.isMobile())
		{
			COLORREF dwBoundsColour = RGB(0, 0, 63);

			Vec2 vOrigin =
				Geometry::transformPoint(
					kmTransformM2S,
					Vec2(0, 0));

if (Variable::render_entity_velocity.getFloatValue())
{
			Vec2 vVelocitySecond =
				kEntity.getVelocity() * 30;
			Vec2 vEnd =
				Vec2(
					vOrigin[0] + vVelocitySecond[0],
					vOrigin[1] - vVelocitySecond[1]);

			drawLine(
				vOrigin[0],
				vOrigin[1],
				vEnd[0],
				vEnd[1],
				krScreenView.getMin()[0],
				krScreenView.getMax()[0],
				krScreenView.getMin()[1],
				krScreenView.getMax()[1],
				dwBoundsColour);
}

if (Variable::render_entity_identifier.getFloatValue())
{
			_TCHAR an[16];
			_stprintf(
				an,
				_T("%d"),
				kEntity.getIdentifier());

			Screen::getBackBuffer()->DrawText(
				vOrigin[0],
				vOrigin[1] - 2,
				an,
				Screen::getSystemFont(),
				GDDRAWTEXT_CENTER,
				NULL,
				0,
				NULL);
}
		}

		for (int nBrushIndex(0);
			nBrushIndex != kModel.getNumberOfBrushes();
			++nBrushIndex)
		{
			const Brush& kBrush = kModel.getBrush(nBrushIndex);

			// TODO need a way to check against brush bounding box.

if (Variable::render_brush_bounds.getFloatValue())
{
			// TEST draw brush bounds.
			{
				COLORREF dwBoundsColour = RGB(0, 0, 63);
				const Rect& rBounds = kBrush.getBounds();
				Vec2 v1 =
					Geometry::transformPoint(kmTransformM2S, rBounds.getMin());
				Vec2 v2 =
					Geometry::transformPoint(
						kmTransformM2S,
						Vec2(rBounds.getMin()[0], rBounds.getMax()[1]));
				Vec2 v3 =
					Geometry::transformPoint(kmTransformM2S, rBounds.getMax());
				Vec2 v4 =
					Geometry::transformPoint(
						kmTransformM2S,
						Vec2(rBounds.getMax()[0], rBounds.getMin()[1]));
				drawLine(
					v1[0],
					v1[1],
					v2[0],
					v2[1],
					krScreenView.getMin()[0],
					krScreenView.getMax()[0],
					krScreenView.getMin()[1],
					krScreenView.getMax()[1],
					dwBoundsColour);
				drawLine(
					v2[0],
					v2[1],
					v3[0],
					v3[1],
					krScreenView.getMin()[0],
					krScreenView.getMax()[0],
					krScreenView.getMin()[1],
					krScreenView.getMax()[1],
					dwBoundsColour);
				drawLine(
					v3[0],
					v3[1],
					v4[0],
					v4[1],
					krScreenView.getMin()[0],
					krScreenView.getMax()[0],
					krScreenView.getMin()[1],
					krScreenView.getMax()[1],
					dwBoundsColour);
				drawLine(
					v4[0],
					v4[1],
					v1[0],
					v1[1],
					krScreenView.getMin()[0],
					krScreenView.getMax()[0],
					krScreenView.getMin()[1],
					krScreenView.getMax()[1],
					dwBoundsColour);
			}
}

if (Variable::render_brush_outline.getFloatValue())
{
			// PERF
			// Right now we are clipping and drawing each line in succession.
			// That means each point is clipped twice. It may be faster to
			// make a clip and draw poly routine.

			Vec2 v1 =
				Geometry::transformPoint(
					kmTransformM2S,
					kBrush.getVertex(kBrush.getNumberOfVertices() - 1));
			Vec2 v2;

			for (int n = 0; n != kBrush.getNumberOfVertices(); ++n)
			{
				v2 =
					Geometry::transformPoint(
						kmTransformM2S,
						kBrush.getVertex(n));

				drawLine(
					v1[0],
					v1[1],
					v2[0],
					v2[1],
					krScreenView.getMin()[0],
					krScreenView.getMax()[0],
					krScreenView.getMin()[1],
					krScreenView.getMax()[1],
					dwColor);

				v1 = v2;
			}
}
		}
	}
}
