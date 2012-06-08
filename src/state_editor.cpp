// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "state_editor.h"

#include <algorithm>
#include <map>
#include <set>

#include "edit_model.h"
#include "edit_point.h"
#include "file.h"
#include "geometry.h"
#include "image.h"
#include "key.h"
#include "render.h"
#include "resourcex.h"
#include "screen.h"
#include "stylus.h"
#include "variable.h"
#include "view.h"


StateEditor
StateEditor::m_state;


// TEMP for testing rendering
static EditPoint* pEditPointSelection = 0;


namespace
{


void
drawPoint(
	const Vec2& kvScreenPoint,
	COLORREF colour);
void
drawToolOutline(
	const int knToolIndex,
	const bool kbDown);
COLORREF
getAnimatedSelectColour();


// Editor modes.
const int knModeSelect = 0;
const int knModeNewPoint = 1;
const int knModeNewLine = 2;
const int knModeNewPolygon = 3;
int nMode = knModeSelect;


// Tool colours.
COLORREF lightColour = RGB(255, 255, 255);
COLORREF midColour = RGB(214, 211, 206);
COLORREF darkColour = RGB(132, 130, 132);

// Another colour.
COLORREF modelColour = RGB(255, 255, 255);


// Toolbar.
int nToolIndex = -1;
int nStylusToolIndex = -1; // The tool the stylus is currently over.
int nFocusToolIndex = -1; // The tool the stylus originally came down on.
bool bToolEnabled[10] = { false /* all false */ };
bool bToolModal[10] = { false /* all false */ };
bool bToolMode[10] = { false /* all false */ };


// Dragging.
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
*******************************************************************************/
EditLine&
createLine(
	EditPoint& beginPoint,
	EditPoint& endPoint)
{
	EditModel& editModel = Resourcex::getEditModel(_T("edit_model"));

	EditLine& line = *new EditLine();
	line.setName(editModel.makeNewLineName());
	line.setBeginPoint(beginPoint);
	line.setEndPoint(endPoint);
	editModel.addLine(line);

	return line;
}
	
	
/*******************************************************************************
*******************************************************************************/
EditPoint&
createPoint(
	const Vec2& kvWorldPoint)
{
	EditModel& editModel = Resourcex::getEditModel(_T("edit_model"));

	EditPoint& point = *new EditPoint();
	point.setName(editModel.makeNewPointName());
	point.setPoint(kvWorldPoint);
	editModel.addPoint(point);

	return point;
}


/*******************************************************************************
*******************************************************************************/
EditPolygon&
createPolygon(
	const std::vector<EditPoint*> kcpPoint)
{
	EditModel& editModel = Resourcex::getEditModel(_T("edit_model"));

	EditPolygon& polygon = *new EditPolygon();
	polygon.setName(editModel.makeNewPolygonName());

	for (int nBeginIndex = kcpPoint.size() - 1, nEndIndex = 0;
		nEndIndex != kcpPoint.size();
		nBeginIndex = nEndIndex, ++nEndIndex)
	{
		EditPoint& beginPoint = *kcpPoint[nBeginIndex];
		EditPoint& endPoint = *kcpPoint[nEndIndex];

		// See if there is already a line joining the two points.
		EditLine* pEditLine = 0;
		for (int nLineIndex = 0;
			nLineIndex != beginPoint.getNumberOfLines(); ++nLineIndex)
		{
			EditLine& editLine = beginPoint.getLine(nLineIndex);
			if (&editLine.getBeginPoint() == &beginPoint &&
				&editLine.getEndPoint() == &endPoint ||
				&editLine.getBeginPoint() == &endPoint &&
				&editLine.getEndPoint() == &beginPoint)
			{
				pEditLine = &editLine;
				break;
			}
		}

		if (!pEditLine)
		{
			// There was not already a line so make one.
			pEditLine = new EditLine();
			pEditLine->setName(editModel.makeNewLineName());
			pEditLine->setBeginPoint(beginPoint);
			pEditLine->setEndPoint(endPoint);
			editModel.addLine(*pEditLine);
		}

		// Add the line to the polygon.
		polygon.addLine(*pEditLine);
	}

	editModel.addPolygon(polygon);

	return polygon;
}


/*******************************************************************************
*******************************************************************************/
void
createPolygonFromSelectedPoints()
{
	std::vector<EditPoint*> cpPoint;
	
	for (int nIndex = 0;
		nIndex != getNumberOfSelectedPoints();
		++nIndex)
	{
		cpPoint.push_back(&getSelectedPoint(nIndex));
	}

	createPolygon(cpPoint);
}


/*******************************************************************************
	There must be at least 6 selected points. The first must be the top left,
	the second must be the bottom right. There must be a point closest to each
	of the four edges.
*******************************************************************************/
void
createReversePolygonFromSelectedPoints()
{
	EditPoint* pTopLeft = &getSelectedPoint(0);
	EditPoint* pBottomRight = &getSelectedPoint(0);

	for (int nIndex = 1; nIndex != getNumberOfSelectedPoints(); ++nIndex)
	{
		EditPoint& editPoint = getSelectedPoint(nIndex);
		const Vec2& kvPoint = editPoint.getPoint();
		if (kvPoint[0] < pTopLeft->getPoint()[0] &&
			pTopLeft->getPoint()[1] < kvPoint[1])
		{
			pTopLeft = &editPoint;
		}
		else if (pBottomRight->getPoint()[0] < kvPoint[0] &&
			kvPoint[1] < pBottomRight->getPoint()[1])
		{
			pBottomRight = &editPoint;
		}
	}

	const Vec2 kvMin =
		Vec2(pTopLeft->getPoint()[0], pBottomRight->getPoint()[1]);
	const Vec2 kvMax =
		Vec2(pBottomRight->getPoint()[0], pTopLeft->getPoint()[1]);

	EditPoint* pTopRight = &createPoint(Vec2(kvMax[0], kvMax[1]));
	EditPoint* pBottomLeft = &createPoint(Vec2(kvMin[0], kvMin[1]));

	typedef std::map<int, EditPoint*>::iterator I;

	// Indexed by y coordinate.
	std::map<int, EditPoint*> mapIntToPointLeft;
	std::map<int, EditPoint*> mapIntToPointLeft2;
	std::map<int, EditPoint*> mapIntToPointRight;
	std::map<int, EditPoint*> mapIntToPointRight2;
	// Indexed by x coordinate.
	std::map<int, EditPoint*> mapIntToPointBottom;
	std::map<int, EditPoint*> mapIntToPointBottom2;
	std::map<int, EditPoint*> mapIntToPointTop;
	std::map<int, EditPoint*> mapIntToPointTop2;

	{for (int nIndex = 0; nIndex != getNumberOfSelectedPoints(); ++nIndex)
	{
		EditPoint& editPoint = getSelectedPoint(nIndex);

		if (&editPoint == pTopLeft ||
			&editPoint == pBottomRight)
		{
			continue;
		}

		const Vec2& kvPoint = editPoint.getPoint();

		// Determine the distance to each side.
		scalar fDistLeft = kvPoint[0] - kvMin[0];
		scalar fDistRight = kvMax[0] - kvPoint[0];
		scalar fDistBottom = kvPoint[1] - kvMin[1];
		scalar fDistTop = kvMax[1] - kvPoint[1];

		// Determine which side is closest.
		scalar fDistClosest = fDistLeft;
		if (fDistRight < fDistClosest)
		{
			fDistClosest = fDistRight;
		}
		if (fDistBottom < fDistClosest)
		{
			fDistClosest = fDistBottom;
		}
		if (fDistTop < fDistClosest)
		{
			fDistClosest = fDistTop;
		}

		// Store the point with its closest side. At the same time, create a
		// matching point on the actual side.
		if (fDistLeft == fDistClosest)
		{
			mapIntToPointLeft[kvPoint[1]] = &editPoint;
			mapIntToPointLeft2[kvPoint[1]] =
				&createPoint(Vec2(kvMin[0], kvPoint[1]));
		}
		else if (fDistRight == fDistClosest)
		{
			mapIntToPointRight[kvPoint[1]] = &editPoint;
			mapIntToPointRight2[kvPoint[1]] =
				&createPoint(Vec2(kvMax[0], kvPoint[1]));
		}
		else if (fDistBottom == fDistClosest)
		{
			mapIntToPointBottom[kvPoint[0]] = &editPoint;
			mapIntToPointBottom2[kvPoint[0]] =
				&createPoint(Vec2(kvPoint[0], kvMin[1]));
		}
		else if (fDistTop == fDistClosest)
		{
			mapIntToPointTop[kvPoint[0]] = &editPoint;
			mapIntToPointTop2[kvPoint[0]] =
				&createPoint(Vec2(kvPoint[0], kvMax[1]));
		}
	}}

	if (!mapIntToPointTop.size() ||
		!mapIntToPointBottom.size() ||
		!mapIntToPointLeft.size() ||
		!mapIntToPointRight.size())
	{
		// Bad points.
		return;
	}

	// Bottom left.
	{
		std::vector<EditPoint*> cpPoint;
		cpPoint.push_back(pBottomLeft);
		cpPoint.push_back(mapIntToPointBottom2.begin()->second);
		cpPoint.push_back(mapIntToPointBottom.begin()->second);
		cpPoint.push_back(mapIntToPointLeft.begin()->second);
		cpPoint.push_back(mapIntToPointLeft2.begin()->second);
		createPolygon(cpPoint);
	}

	// Bottom.
	{for (I i = ++mapIntToPointBottom.begin(),
		i2 = ++mapIntToPointBottom2.begin();
		i != mapIntToPointBottom.end();
		++i, ++i2)
	{
		std::vector<EditPoint*> cpPoint;
		cpPoint.push_back(i->second);
		cpPoint.push_back((--I(i))->second);
		cpPoint.push_back((--I(i2))->second);
		cpPoint.push_back(i2->second);
		createPolygon(cpPoint);
	}}

	// Bottom right.
	{
		std::vector<EditPoint*> cpPoint;
		cpPoint.push_back(pBottomRight);
		cpPoint.push_back(mapIntToPointRight2.begin()->second);
		cpPoint.push_back(mapIntToPointRight.begin()->second);
		cpPoint.push_back((--mapIntToPointBottom.end())->second);
		cpPoint.push_back((--mapIntToPointBottom2.end())->second);
		createPolygon(cpPoint);
	}

	// Right.
	{for (I i = ++mapIntToPointRight.begin(),
		i2 = ++mapIntToPointRight2.begin();
		i != mapIntToPointRight.end();
		++i, ++i2)
	{
		std::vector<EditPoint*> cpPoint;
		cpPoint.push_back(i->second);
		cpPoint.push_back((--I(i))->second);
		cpPoint.push_back((--I(i2))->second);
		cpPoint.push_back(i2->second);
		createPolygon(cpPoint);
	}}

	// Top right.
	{
		std::vector<EditPoint*> cpPoint;
		cpPoint.push_back(pTopRight);
		cpPoint.push_back((--mapIntToPointTop2.end())->second);
		cpPoint.push_back((--mapIntToPointTop.end())->second);
		cpPoint.push_back((--mapIntToPointRight.end())->second);
		cpPoint.push_back((--mapIntToPointRight2.end())->second);
		createPolygon(cpPoint);
	}

	// Top.
	{for (I i = ++mapIntToPointTop.begin(),
		i2 = ++mapIntToPointTop2.begin();
		i != mapIntToPointTop.end();
		++i, ++i2)
	{
		std::vector<EditPoint*> cpPoint;
		cpPoint.push_back((--I(i))->second);
		cpPoint.push_back(i->second);
		cpPoint.push_back(i2->second);
		cpPoint.push_back((--I(i2))->second);
		createPolygon(cpPoint);
	}}

	// Top left.
	{
		std::vector<EditPoint*> cpPoint;
		cpPoint.push_back(pTopLeft);
		cpPoint.push_back((--mapIntToPointLeft2.end())->second);
		cpPoint.push_back((--mapIntToPointLeft.end())->second);
		cpPoint.push_back(mapIntToPointTop.begin()->second);
		cpPoint.push_back(mapIntToPointTop2.begin()->second);
		createPolygon(cpPoint);
	}

	// Left.
	{for (I i = ++mapIntToPointLeft.begin(),
		i2 = ++mapIntToPointLeft2.begin();
		i != mapIntToPointLeft.end();
		++i, ++i2)
	{
		std::vector<EditPoint*> cpPoint;
		cpPoint.push_back((--I(i))->second);
		cpPoint.push_back(i->second);
		cpPoint.push_back(i2->second);
		cpPoint.push_back((--I(i2))->second);
		createPolygon(cpPoint);
	}}
}


/*******************************************************************************
*******************************************************************************/
void
doToolAction(
	const int knToolIndex)
{
	switch (knToolIndex)
	{
		case 0:
			nMode = knModeSelect;
			break;
		case 1:
			nMode = knModeNewPoint;
			break;
		case 2:
			nMode = knModeNewPolygon;
			break;
		case 3:
			if (getNumberOfSelectedPoints())
			{
				createPolygonFromSelectedPoints();
			}
			break;
		case 4:
			if (getNumberOfSelectedPoints())
			{
				createReversePolygonFromSelectedPoints();
			}
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			// Toggle wireframe rendering.
			if (Variable::render_editor_model.getFloatValue() == 0)
			{
				Variable::render_editor_model.setValue(_T("1"));
			}
			else
			{
				Variable::render_editor_model.setValue(_T("0"));
			}
			break;
		case 9:
			// Toggle image rendering.
			if (Variable::render_editor_image.getFloatValue() == 0)
			{
				Variable::render_editor_image.setValue(_T("0.5"));
			}
			else if (Variable::render_editor_image.getFloatValue() == 1)
			{
				Variable::render_editor_image.setValue(_T("0"));
			}
			else
			{
				Variable::render_editor_image.setValue(_T("1"));
			}
			break;
	}
}


/*******************************************************************************
	PERF Right now there is no bounds checking.
*******************************************************************************/
void
drawEditImage()
{
	if (!Variable::render_editor_image.getFloatValue())
	{
		return;
	}

	CGapiSurface& backBuffer = *Screen::getBackBuffer();
	const Rect& krScreenView = View::getScreenView();

	// TEMP Right now this just draws part of a hardcoded image.
	// TODO only draw image over view, with the proper scrolling

	GDBLTFASTFX fx;
	if (Variable::render_editor_image.getFloatValue() == 0.5)
	{
		fx.dwOpacity = 128;
	}
	else
	{
		fx.dwOpacity = 255;
	}

	// We want the image to be positioned in the world so that its bottom left
	// corner is at the origin. Therefore, we want to draw from its top left
	// corner, in screen coordinates.
	CGapiSurface& surface = Resourcex::getImage(_T("edit_model")).getSurface();
	const Vec2 kvWorldPoint = Vec2(0, surface.GetHeight());
	const Vec2 kvScreenPoint =
		Geometry::transformPoint(
			View::getTransformW2S(),
			kvWorldPoint);

	backBuffer.BltFast(
		kvScreenPoint[0],
		kvScreenPoint[1],
		&surface,
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
	if (!Variable::render_editor_model.getFloatValue())
	{
		return;
	}

	CGapiSurface& backBuffer = *Screen::getBackBuffer();
	const Rect& krScreenView = View::getScreenView();

	std::set<EditPoint*> drawnPointSet;

	EditModel& editModel = Resourcex::getEditModel(_T("edit_model"));

	for (int nLineIndex = 0;
		nLineIndex != editModel.getNumberOfLines(); ++nLineIndex)
	{
		EditLine& editLine = editModel.getLine(nLineIndex);

		EditPoint& beginPoint = editLine.getBeginPoint();
		EditPoint& endPoint = editLine.getEndPoint();

		drawnPointSet.insert(&beginPoint);
		drawnPointSet.insert(&endPoint);

		const Vec2 kvBeginPoint =
			Geometry::transformPoint(
				View::getTransformW2S(),
				beginPoint.getPoint());
		const Vec2 kvEndPoint =
			Geometry::transformPoint(
				View::getTransformW2S(),
				endPoint.getPoint());

		Render::drawLine(
			kvBeginPoint[0],
			kvBeginPoint[1],
			kvEndPoint[0],
			kvEndPoint[1],
			krScreenView.getMin()[0],
			krScreenView.getMax()[0],
			krScreenView.getMin()[1],
			krScreenView.getMax()[1],
			modelColour);
	}

	for (int nPointIndex = 0;
		nPointIndex != editModel.getNumberOfPoints(); ++nPointIndex)
	{
		EditPoint& editPoint = editModel.getPoint(nPointIndex);

		if (std::find(drawnPointSet.begin(), drawnPointSet.end(),
			&editPoint) == drawnPointSet.end())
		{
			// Draw point.
			const Vec2 kvScreenPoint =
				Geometry::transformPoint(
					View::getTransformW2S(),
					editPoint.getPoint());
			drawPoint(kvScreenPoint, modelColour);
		}
	}
}


/*******************************************************************************
*******************************************************************************/
void
drawPoint(
	const Vec2& kvScreenPoint,
	COLORREF colour)
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();

	const int knOffset = 2;

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
				colour);
		}
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

	EditModel& editModel = Resourcex::getEditModel(_T("edit_model"));

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

	for (int nPointIndex = 0;
		nPointIndex != getNumberOfSelectedPoints(); ++nPointIndex)
	{
		EditPoint& editPoint = getSelectedPoint(nPointIndex);

		const Vec2 kvScreenPoint =
			Geometry::transformPoint(
				View::getTransformW2S(),
				editPoint.getPoint());

		drawPoint(kvScreenPoint, selectColour);
	}
}


/*******************************************************************************
*******************************************************************************/
void
drawToolBar()
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();

	RECT toolBarRect;
	toolBarRect.left = 0;
	toolBarRect.right = 240;
	toolBarRect.top = 320 - 24;
	toolBarRect.bottom = 320;

	// Draw background.
	backBuffer.FillRect(
		&toolBarRect,
		midColour,
		0,
		NULL);

	RECT toolImageRect;
	toolImageRect.top = 0;
	toolImageRect.bottom = 24;

	// Draw images.
	for (int nToolIndex = 0; nToolIndex != 10; ++nToolIndex)
	{
		toolImageRect.left = nToolIndex * 24;
		toolImageRect.right = (nToolIndex + 1) * 24;

		if (bToolEnabled[nToolIndex])
		{
			backBuffer.BltFast(
				toolImageRect.left,
				320 - 24,
				&Resourcex::getImage(_T("toolbar")).getSurface(),
				&toolImageRect,
				GDBLTFAST_KEYSRC,
				NULL);
		}
		else
		{
			GDBLTFASTFX fx;
			fx.dwFXcolor1 = RGB(255, 255, 255);
			backBuffer.BltFast(
				toolImageRect.left,
				320 - 24,
				&Resourcex::getImage(_T("toolbar")).getSurface(),
				&toolImageRect,
				GDBLTFAST_KEYSRC | GDBLTFASTFX_COLORIZE,
				&fx);
		}
	}

	// Draw outline.
	if (Stylus::isDown() && nFocusToolIndex != -1)
	{
		if (nFocusToolIndex == nStylusToolIndex)
		{
			// Redraw this one tool.
			// TODO This is ugly and probably off by some pixels.
			RECT toolScreenRect = toolBarRect;
			toolScreenRect.left = nFocusToolIndex * 24;
			toolScreenRect.right = (nFocusToolIndex + 1) * 24;
			RECT toolImageRect = toolScreenRect;
			toolImageRect.top = 0;
			toolImageRect.bottom = 24;
			backBuffer.FillRect(
				&toolScreenRect,
				midColour,
				0,
				NULL);
			backBuffer.BltFast(
				toolScreenRect.left + 1,
				toolScreenRect.top + 1,
				&Resourcex::getImage(_T("toolbar")).getSurface(),
				&toolImageRect,
				GDBLTFAST_KEYSRC,
				NULL);
		}
		drawToolOutline(nFocusToolIndex, nFocusToolIndex == nStylusToolIndex);
	}
	else if (!Stylus::isDown() && nStylusToolIndex != -1)
	{
		drawToolOutline(nStylusToolIndex, false);
	}
}


/*******************************************************************************
*******************************************************************************/
void
drawToolOutline(
	const int knToolIndex,
	const bool kbDown)
{
	CGapiSurface& backBuffer = *Screen::getBackBuffer();

	RECT toolRect;
	toolRect.left = knToolIndex * 24;
	toolRect.right = (knToolIndex + 1) * 24 - 1;
	toolRect.top = 320 - 24 + 1;
	toolRect.bottom = 320 - 1;
	COLORREF& topColour =
		kbDown ? darkColour : lightColour;
	COLORREF& bottomColour =
		kbDown ? lightColour : darkColour;

	// Top line.
	backBuffer.DrawLine(
		toolRect.left,
		toolRect.top,
		toolRect.right,
		toolRect.top,
		topColour,
		0,
		NULL);

	// Left line.
	backBuffer.DrawLine(
		toolRect.left,
		toolRect.top + 1,
		toolRect.left,
		toolRect.bottom - 1,
		topColour,
		0,
		NULL);

	// Bottom line.
	backBuffer.DrawLine(
		toolRect.left,
		toolRect.bottom,
		toolRect.right,
		toolRect.bottom,
		bottomColour,
		0,
		NULL);

	// Right line.
	backBuffer.DrawLine(
		toolRect.right,
		toolRect.top,
		toolRect.right,
		toolRect.bottom - 1,
		bottomColour,
		0,
		NULL);
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
std::vector<EditPoint*>
getPoints(
	const Vec2& kvWorldPoint)
{
	std::vector<EditPoint*> cpPoint;

	EditModel& editModel = Resourcex::getEditModel(_T("edit_model"));

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
			cpPoint.push_back(&editPoint);
			break;
		}
	}

	return cpPoint;
}


/*******************************************************************************
*******************************************************************************/
int
getToolIndex(
	const Vec2& kvScreenPoint)
{
	if (!Geometry::isPointInRect(kvScreenPoint,
		Rect(Vec2(0, 320 - 24), Vec2(240, 320))))
	{
		return -1;
	}

	return static_cast<int>(kvScreenPoint[0]) / 24;
}


/*******************************************************************************
*******************************************************************************/
bool
isPointSelected(
	EditPoint& editPoint)
{
	for (int nPointIndex = 0;
		nPointIndex != getNumberOfSelectedPoints(); ++nPointIndex)
	{
		if (&editPoint == &getSelectedPoint(nPointIndex))
		{
			return true;
		}
	}

	return false;
}


/*******************************************************************************
*******************************************************************************/
void
selectLines(
	const Vec2& kvWorldPoint)
{
	EditModel& editModel = Resourcex::getEditModel(_T("edit_model"));

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
	EditModel& editModel = Resourcex::getEditModel(_T("edit_model"));

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


/*******************************************************************************
*******************************************************************************/
void
toolbarEnable()
{
	bToolEnabled[0] = true;
	bToolEnabled[1] = true;
	bToolEnabled[2] = true;

	bToolEnabled[3] = 3 <= getNumberOfSelectedPoints();
	bToolEnabled[4] = 6 <= getNumberOfSelectedPoints();

	bToolEnabled[5] =
		getNumberOfSelectedPoints() ||
		getNumberOfSelectedLines() ||
		getNumberOfSelectedPolygons();

	bToolEnabled[6] = false;
	bToolEnabled[7] = false;

	bToolEnabled[8] = true;
	bToolEnabled[9] = true;
}


} // namespace


/*******************************************************************************
*******************************************************************************/
void
StateEditor::compileModel(
	const tstring& ksName)
{
	EditModel& editModel = Resourcex::getEditModel(_T("edit_model"));

	FILE* pFile = File::openFile(ksName + _T(".txt"), _T("w"));

	_ftprintf(pFile, _T("model\n"));
	_ftprintf(pFile, _T("{\n"));

	for (int nPolygonIndex = 0;
		nPolygonIndex != editModel.getNumberOfPolygons();
		++nPolygonIndex)
	{
		EditPolygon& editPolygon = editModel.getPolygon(nPolygonIndex);

		// Use this to sort the points.
		std::vector<EditPoint*> cpPoint(editPolygon.getNumberOfLines());
		{
			EditLine& editLine = editPolygon.getLine(0);
			EditPoint& beginPoint = editLine.getBeginPoint();
			EditPoint& endPoint = editLine.getEndPoint();
			cpPoint[0] = &beginPoint;
			cpPoint[1] = &endPoint;
		}

		for (int nPointIndex = 2; nPointIndex != cpPoint.size(); ++nPointIndex)
		{
			for (int nLineIndex = 1;
				nLineIndex != editPolygon.getNumberOfLines();
				++nLineIndex)
			{
				EditLine& editLine = editPolygon.getLine(nLineIndex);
				EditPoint& beginPoint = editLine.getBeginPoint();
				EditPoint& endPoint = editLine.getEndPoint();
				if (cpPoint[nPointIndex - 1] == &beginPoint &&
					cpPoint[nPointIndex - 2] != &endPoint)
				{
					cpPoint[nPointIndex] = &endPoint;
					break;
				}
				else if (cpPoint[nPointIndex - 1] == &endPoint &&
					cpPoint[nPointIndex - 2] != &beginPoint)
				{
					cpPoint[nPointIndex] = &beginPoint;
					break;
				}

			}
		}

		// At this point, the vector contains the points in order, though we
		// don't know if it is inside out or not.
		{
			const Vec2 v1 = cpPoint[1]->getPoint() - cpPoint[0]->getPoint();
			const Vec2 v2 = cpPoint[2]->getPoint() - cpPoint[1]->getPoint();
			if (v1[0] * v2[1] < v2[0] * v1[1])
			{
				// Reverse.
				std::reverse(cpPoint.begin(), cpPoint.end());
			}
		}

		_ftprintf(pFile, _T("\tbrush\n"));
		_ftprintf(pFile, _T("\t{\n"));

		for (int nIndex = 0; nIndex != cpPoint.size(); ++nIndex)
		{
			_ftprintf(
				pFile,
				_T("\t\t<%f, %f>\n"),
				cpPoint[nIndex]->getPoint()[0],
				cpPoint[nIndex]->getPoint()[1]);
		}

		_ftprintf(pFile, _T("\t}\n"));
	}

	_ftprintf(pFile, _T("}\n"));

	File::closeFile(pFile);
}


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

	toolbarEnable();
	drawToolBar();

	// TEMP Draw lasso.
	if (nMode == knModeSelect &&
		Stylus::isDown() &&
		nFocusToolIndex == -1 &&
		pDragLine == 0 &&
		pDragPoint == 0 &&
		pDragPolygon == 0)
	{
		RECT screenRect;
		screenRect.left =
			Stylus::getAnchorPoint()[0];
		screenRect.right =
			Stylus::getLeadPoint()[0];
		screenRect.bottom =
			Stylus::getAnchorPoint()[1];
		screenRect.top =
			Stylus::getLeadPoint()[1];
		backBuffer.DrawRect(
			&screenRect,
			getAnimatedSelectColour(),
			0,
			NULL);
	}

	if (Variable::render_console.getFloatValue())
	{
		Screen::drawConsole();
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

	// Toolbar handling.
	nStylusToolIndex = getToolIndex(kvScreenPoint);
	nFocusToolIndex = nStylusToolIndex;

	// If we are in the toolbar, don't do anything else.
	if (nStylusToolIndex != -1)
	{
		return;
	}

	// New point handling. (TEMP)
	if (nMode == knModeNewPoint)
	{
		EditModel& editModel = Resourcex::getEditModel(_T("edit_model"));

		EditPoint& point = *new EditPoint();
		point.setName(editModel.makeNewPointName());
		point.setPoint(kvWorldPoint);
		editModel.addPoint(point);
	}

	// New polygon handling. (TEMP)
	if (nMode == knModeNewPolygon)
	{
		EditModel& editModel = Resourcex::getEditModel(_T("edit_model"));

		EditPoint& point0 = *new EditPoint();
		point0.setName(editModel.makeNewPointName());
		point0.setPoint(kvWorldPoint + Vec2(3, 10));
		editModel.addPoint(point0);

		EditPoint& point1 = *new EditPoint();
		point1.setName(editModel.makeNewPointName());
		point1.setPoint(kvWorldPoint + Vec2(7, -4));
		editModel.addPoint(point1);

		EditPoint& point2 = *new EditPoint();
		point2.setName(editModel.makeNewPointName());
		point2.setPoint(kvWorldPoint + Vec2(-5, -2));
		editModel.addPoint(point2);

		EditLine& line0 = *new EditLine();
		line0.setName(editModel.makeNewLineName());
		line0.setBeginPoint(point0);
		line0.setEndPoint(point1);
		editModel.addLine(line0);

		EditLine& line1 = *new EditLine();
		line1.setName(editModel.makeNewLineName());
		line1.setBeginPoint(point1);
		line1.setEndPoint(point2);
		editModel.addLine(line1);

		EditLine& line2 = *new EditLine();
		line2.setName(editModel.makeNewLineName());
		line2.setBeginPoint(point2);
		line2.setEndPoint(point0);
		editModel.addLine(line2);

		EditPolygon& polygon = *new EditPolygon();
		polygon.setName(editModel.makeNewPolygonName());
		polygon.addLine(line0);
		polygon.addLine(line1);
		polygon.addLine(line2);
		editModel.addPolygon(polygon);
	}

	if (nMode == knModeSelect)
	{
		vDragScreenPoint = kvScreenPoint;
		pDragPoint = 0;
		pDragLine = 0;
		pDragPolygon = 0;

		std::vector<EditPoint*> cpPoint = getPoints(kvWorldPoint);

		if (!Key::isKeyDown(Key::m_knKeyA))
		{
			if (cpPoint.size() == 0 ||
				!isPointSelected(*cpPoint[0]))
			{
				clearSelectedPoints();
				clearSelectedLines();
				clearSelectedPolygons();
			}
		}

		selectPoints(kvWorldPoint);
		if (getNumberOfSelectedPoints() == 0)
		{
			selectLines(kvWorldPoint);
		}
	}
}


/*******************************************************************************
*******************************************************************************/
void
StateEditor::stylusMove(
	const Vec2& kvScreenPoint)
{
	// Toolbar handling.
	nStylusToolIndex = getToolIndex(kvScreenPoint);
#ifdef _WIN32_WCE
	// Can't have rollover effect with stylus.
	if (nStylusToolIndex != -1 && nStylusToolIndex != nFocusToolIndex)
	{
		nFocusToolIndex = nStylusToolIndex;
	}
#endif

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
	// TEMP Select lasso.
	if (nMode == knModeSelect &&
		nFocusToolIndex == -1 &&
		pDragLine == 0 &&
		pDragPoint == 0 &&
		pDragPolygon == 0)
	{
		EditModel& editModel = Resourcex::getEditModel(_T("edit_model"));
		Vec2 vWorldAnchorPoint =
			Geometry::transformPoint(
				View::getTransformS2W(),
				Stylus::getAnchorPoint());
		Vec2 vWorldLeadPoint =
			Geometry::transformPoint(
				View::getTransformS2W(),
				Stylus::getLeadPoint());
		if (vWorldLeadPoint[0] < vWorldAnchorPoint[0])
		{
			std::swap(vWorldLeadPoint[0], vWorldAnchorPoint[0]);
		}
		if (vWorldLeadPoint[1] < vWorldAnchorPoint[1])
		{
			std::swap(vWorldLeadPoint[1], vWorldAnchorPoint[1]);
		}
		for (int nPointIndex = 0;
			nPointIndex != editModel.getNumberOfPoints(); ++nPointIndex)
		{
			EditPoint& editPoint = editModel.getPoint(nPointIndex);

			const Vec2 kvPoint = editPoint.getPoint();

			if (vWorldAnchorPoint[0] <= kvPoint[0] &&
				kvPoint[0] <= vWorldLeadPoint[0] &&
				vWorldAnchorPoint[1] <= kvPoint[1] &&
				kvPoint[1] <= vWorldLeadPoint[1])
			{
				// Select point.
				addSelectedPoint(editPoint);
			}
		}
	}

	pDragPoint = 0;
	pDragLine = 0;
	pDragPolygon = 0;

	// Toolbar handling.
	nStylusToolIndex = getToolIndex(kvScreenPoint);
	if (nStylusToolIndex != -1 && nFocusToolIndex == nStylusToolIndex)
	{
		doToolAction(nFocusToolIndex);
	}
#ifdef _WIN32_WCE
	// Can't have rollover effect with stylus.
	nStylusToolIndex = -1;
#endif
	nFocusToolIndex = -1;
}
