// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "state_editor.h"

#include <algorithm>
#include <set>

#include "edit_model.h"
#include "edit_point.h"
#include "geometry.h"
#include "image.h"
#include "key.h"
#include "render.h"
#include "resourcex.h"
#include "screen.h"
#include "stylus.h"
#include "view.h"


StateEditor
StateEditor::m_state;


// TEMP for testing rendering
static EditPoint* pEditPointSelection = 0;


namespace
{


COLORREF
getAnimatedSelectColour();


Vec2 vDragScreenPoint;
EditLine* pDragLine = 0;
EditPoint* pDragPoint = 0;
EditPolygon* pDragPolygon = 0;


std::vector<EditLine*> cpLineSelect;
std::vector<EditPoint*> cpPointSelect;
std::vector<EditPolygon*> cpPolygonSelect;


void
addSelectedLine(
	EditLine& editLine)
{
	if (std::find(cpLineSelect.begin(), cpLineSelect.end(),
		&editLine) == cpLineSelect.end())
	{
		cpLineSelect.push_back(&editLine);
	}
}


void
addSelectedPoint(
	EditPoint& editPoint)
{
	if (std::find(cpPointSelect.begin(), cpPointSelect.end(),
		&editPoint) == cpPointSelect.end())
	{
		cpPointSelect.push_back(&editPoint);
	}
}


void
addSelectedPolygon(
	EditPolygon& editPolygon)
{
	if (std::find(cpPolygonSelect.begin(), cpPolygonSelect.end(),
		&editPolygon) == cpPolygonSelect.end())
	{
		cpPolygonSelect.push_back(&editPolygon);
	}
}


void
clearSelectedLines()
{
	cpLineSelect.clear();
}


void
clearSelectedPoints()
{
	cpPointSelect.clear();
}


void
clearSelectedPolygons()
{
	cpPolygonSelect.clear();
}


int
getNumberOfSelectedLines()
{
	return cpLineSelect.size();
}


int
getNumberOfSelectedPoints()
{
	return cpPointSelect.size();
}


int
getNumberOfSelectedPolygons()
{
	return cpPolygonSelect.size();
}


EditLine&
getSelectedLine(
	const int knIndex)
{
	return *cpLineSelect[knIndex];
}


EditPoint&
getSelectedPoint(
	const int knIndex)
{
	return *cpPointSelect[knIndex];
}


EditPolygon&
getSelectedPolygon(
	const int knIndex)
{
	return *cpPolygonSelect[knIndex];
}


/*******************************************************************************
	PERF Right now there is no bounds checking.
*******************************************************************************/
void
drawEditImage()
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();
	const Rect& krScreenView = View::getScreenView();

	// TEMP Right now this just draws part of a hardcoded image.
	// TODO only draw image over view, with the proper scrolling

	GDBLTFASTFX fx;
	fx.dwOpacity = 128;

	backBuffer.BltFast(
		-View::getWorldView().getMin()[0],
		View::getWorldView().getMin()[1],
		&Resourcex::getImage(_T("edit_world")).getSurface(),
		NULL,
		GDBLTFAST_OPACITY,
		&fx);
}


/*******************************************************************************
	PERF Right now there is no bounds checking.
*******************************************************************************/
void
drawEditModel()
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();
	const Rect& krScreenView = View::getScreenView();
	COLORREF lineColour = RGB(255, 255, 255);

	EditModel& editModel = Resourcex::getEditModel(_T("test"));

	for (int nLineIndex = 0;
		nLineIndex != editModel.getNumberOfLines(); ++nLineIndex)
	{
		EditLine& editLine = editModel.getLine(nLineIndex);

		const Vec2 kvBeginPoint =
			Geometry::transformPoint(
				View::getTransformW2S(),
				editLine.getBeginPoint().getPoint());
		const Vec2 kvEndPoint =
			Geometry::transformPoint(
				View::getTransformW2S(),
				editLine.getEndPoint().getPoint());

		Render::drawLine(
			kvBeginPoint[0],
			kvBeginPoint[1],
			kvEndPoint[0],
			kvEndPoint[1],
			krScreenView.getMin()[0],
			krScreenView.getMax()[0],
			krScreenView.getMin()[1],
			krScreenView.getMax()[1],
			lineColour);
	}
}


/*******************************************************************************
	PERF Right now there is no bounds checking.
*******************************************************************************/
void
drawSelectedLines()
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();
	const Rect& krScreenView = View::getScreenView();
	COLORREF selectColour = getAnimatedSelectColour();

	EditModel& editModel = Resourcex::getEditModel(_T("test"));

	// TEMP for testing rendering
	for (int nLineIndex = 0;
		nLineIndex != getNumberOfSelectedLines(); ++nLineIndex)
	{
		EditLine& editLine = getSelectedLine(nLineIndex);

		const Vec2 kvBeginPoint =
			Geometry::transformPoint(
				View::getTransformW2S(),
				editLine.getBeginPoint().getPoint());
		const Vec2 kvEndPoint =
			Geometry::transformPoint(
				View::getTransformW2S(),
				editLine.getEndPoint().getPoint());

		Render::drawLine(
			kvBeginPoint[0],
			kvBeginPoint[1],
			kvEndPoint[0],
			kvEndPoint[1],
			krScreenView.getMin()[0],
			krScreenView.getMax()[0],
			krScreenView.getMin()[1],
			krScreenView.getMax()[1],
			selectColour);
	}
}


/*******************************************************************************
	PERF Right now there is no bounds checking.
*******************************************************************************/
void
drawSelectedPoints()
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();
	const Rect& krScreenView = View::getScreenView();
	COLORREF selectColour = getAnimatedSelectColour();

	// Make this offset configurable? Put it in a better place?
	const int knOffset = 2;

	for (int nPointIndex = 0;
		nPointIndex != getNumberOfSelectedPoints(); ++nPointIndex)
	{
		EditPoint& editPoint = getSelectedPoint(nPointIndex);

		const Vec2 kvScreenPoint =
			Geometry::transformPoint(
				View::getTransformW2S(),
				editPoint.getPoint());

		for (int nY = kvScreenPoint[1] - knOffset;
			nY <= kvScreenPoint[1] + knOffset;
			++nY)
		{
			for (int nX = kvScreenPoint[0] - knOffset;
				nX <= kvScreenPoint[0] + knOffset;
				++nX)
			{
				backBuffer.SetPixel(
					nX,
					nY,
					selectColour);
			}
		}
	}
}


/*******************************************************************************
*******************************************************************************/
COLORREF
getAnimatedSelectColour()
{
	COLORREF selectColour;
	switch (Application::getActualFrameNumber() % 6)
	{
		case 0:
			selectColour = RGB(255, 0, 0);
			break;
		case 1:
			selectColour = RGB(255, 255, 0);
			break;
		case 2:
			selectColour = RGB(0, 255, 0);
			break;
		case 3:
			selectColour = RGB(0, 255, 255);
			break;
		case 4:
			selectColour = RGB(0, 0, 255);
			break;
		case 5:
			selectColour = RGB(255, 0, 255);
			break;
	}
	return selectColour;
}


/*******************************************************************************
*******************************************************************************/
void
selectLines(
	const Vec2& kvWorldPoint)
{
	EditModel& editModel = Resourcex::getEditModel(_T("test"));

	for (int nLineIndex = 0;
		nLineIndex != editModel.getNumberOfLines(); ++nLineIndex)
	{
		EditLine& editLine = editModel.getLine(nLineIndex);

		const Vec2 kvPointDirection =
			kvWorldPoint -
			editLine.getBeginPoint().getPoint();
		const Vec2 kvLineDirection =
			editLine.getEndPoint().getPoint() -
			editLine.getBeginPoint().getPoint();
		const Vec2 kvLineNormal =
			Vec2(kvLineDirection[1], -kvLineDirection[0]);

		const Vec2 kvNormalizedNormal =
			kvLineNormal / sqrt(kvLineNormal % kvLineNormal);

		const scalar kfDotDirection =
			kvPointDirection % kvLineDirection;
		const scalar kfDotLine =
			kvLineDirection % kvLineDirection;
		const scalar kfDotNormal =
			kvPointDirection % kvNormalizedNormal;

		const int knOffset = 4;
		if (-knOffset <= kfDotNormal &&
			kfDotNormal <= knOffset &&
			0 <= kfDotDirection &&
			kfDotDirection <= kfDotLine)
		{
			// Select line.
			pDragLine = &editLine;
			addSelectedLine(editLine);
			break;
		}
	}
}


/*******************************************************************************
*******************************************************************************/
void
selectPoints(
	const Vec2& kvWorldPoint)
{
	EditModel& editModel = Resourcex::getEditModel(_T("test"));

	for (int nPointIndex = 0;
		nPointIndex != editModel.getNumberOfPoints(); ++nPointIndex)
	{
		EditPoint& editPoint = editModel.getPoint(nPointIndex);

		const Vec2& kvEditPoint = editPoint.getPoint();

		const Vec2 kvDistance = kvWorldPoint - kvEditPoint;

		const int knOffset = 4;
		if (-knOffset <= kvDistance[0] &&
			kvDistance[0] <= knOffset &&
			-knOffset <= kvDistance[1] &&
			kvDistance[1] <= knOffset)
		{
			// Select point.
			pDragPoint = &editPoint;
			addSelectedPoint(editPoint);
			break;
		}
	}
}


} // namespace


/*******************************************************************************
*******************************************************************************/
void
StateEditor::enterState()
{
	View::setScreenViewOrigin(Vec2(0, 0));
	View::setViewSize(Vec2(240, 296));
}


/*******************************************************************************
*******************************************************************************/
void
StateEditor::processFrame()
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();

	Screen::clear();

	// I need this to process keys that repeat, like +forward and -forward,
	// until I find out where Quake 2 does it.
	Key::processCommands();

	drawEditImage();
	drawEditModel();

	drawSelectedPoints();
	drawSelectedLines();

	// TEMP draw a dummy toolbar
	{
		COLORREF buttonColour = RGB(128, 128, 128);
		COLORREF outlineColour = RGB(255, 255, 255);
		RECT buttonRect;
		buttonRect.top = 320 - 24;
		buttonRect.bottom = 320;
		for (int n = 0; n != 10; ++n)
		{
			buttonRect.right = n * 24;
			buttonRect.left = (n + 1) * 24;
			backBuffer.FillRect(
				&buttonRect,
				buttonColour,
				0,
				NULL);
			backBuffer.DrawRect(
				&buttonRect,
				outlineColour,
				0,
				NULL);
		}
	}
}


/*******************************************************************************
*******************************************************************************/
void
StateEditor::stylusDown(
	const Vec2& kvScreenPoint)
{
	const Vec2 kvWorldPoint =
		Geometry::transformPoint(
			View::getTransformS2W(),
			kvScreenPoint);

	vDragScreenPoint = kvScreenPoint;
	pDragPoint = 0;
	pDragLine = 0;
	pDragPolygon = 0;

	if (!Key::isKeyDown(Key::m_knKeyA))
	{
		clearSelectedPoints();
		clearSelectedLines();
		clearSelectedPolygons();
	}

	selectPoints(kvWorldPoint);
	if (getNumberOfSelectedPoints() == 0)
	{
		selectLines(kvWorldPoint);
	}
}


/*******************************************************************************
*******************************************************************************/
void
StateEditor::stylusMove(
	const Vec2& kvScreenPoint)
{
	Vec2 vDrag = kvScreenPoint - vDragScreenPoint;
	vDragScreenPoint = kvScreenPoint;

	// Flip y.
	vDrag[1] = -vDrag[1];

	if (pDragPoint)
	{
		// We are dragging a point. But we want to move all selected points.
		for (int nPointIndex = 0; nPointIndex != getNumberOfSelectedPoints();
			++nPointIndex)
		{
			getSelectedPoint(nPointIndex).setPoint(
				getSelectedPoint(nPointIndex).getPoint() + vDrag);
		}
	}

	if (pDragLine)
	{
		// We are dragging a line. But we want to move all selected lines.
		// And we only want to move individual points once.
		std::set<EditPoint*> pointSet;
		for (int nLineIndex = 0; nLineIndex != getNumberOfSelectedLines();
			++nLineIndex)
		{
			EditLine& editLine = getSelectedLine(nLineIndex);
			{
				EditPoint& editPoint = editLine.getBeginPoint();
				if (std::find(pointSet.begin(), pointSet.end(),
					&editPoint) == pointSet.end())
				{
					editPoint.setPoint(
						editPoint.getPoint() + vDrag);
					pointSet.insert(&editPoint);
				}
			}
			{
				EditPoint& editPoint = editLine.getEndPoint();
				if (std::find(pointSet.begin(), pointSet.end(),
					&editPoint) == pointSet.end())
				{
					editPoint.setPoint(
						editPoint.getPoint() + vDrag);
					pointSet.insert(&editPoint);
				}
			}
		}
	}
}


/*******************************************************************************
*******************************************************************************/
void
StateEditor::stylusUp(
	const Vec2& kvScreenPoint)
{
	pDragPoint = 0;
	pDragLine = 0;
	pDragPolygon = 0;
}
