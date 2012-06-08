// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "editor/model_editor.h"

#include <map>

#include "application.h"
#include "edit_model.h"
#include "file.h"
#include "image.h" // Is this needed?
#include "key.h"
#include "menu.h" // TODO Remove this when I redo menu module.
#include "resourcex.h"
#include "widget_file_dialog.h"
#include "widget_main_window.h"
#include "widget_message_dialog.h"
#include "widget_tool_button.h"
#include "editor/set_image_origin_dialog.h"


SetImageOriginDialog*
ModelEditor::m_pSetImageOriginDialog;


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::closeEditor()
{
	// TODO More deletion and freeing of resources.
	delete m_pModel;
	m_pModel = 0;
	m_bModelDirty = false;

	Application::getModelEditorMainWindow().getMenuBar().hide();
	Application::getModelEditorMainWindow().hide();
	Menu::setCurrentMenu(Menu::title_main);
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::compileModel()
{
	EditModel& editModel = getModel();

	FILE* pFile =
		File::openFile(
			tstring(_T("data\\models\\")) + getModel().getName() + _T(".txt"),
			_T("w"));

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
	Will reuse existing lines, but will still create a new polygon even if
	one exists with all of those lines.

	There must be at least 6 selected points. The first must be the top left,
	the second must be the bottom right. There must be a point closest to each
	of the four edges.

	Currently, the created points are selected.
*******************************************************************************/
void
ModelEditor::createInversePolygon(
	const std::vector<EditPoint*> kcpPoint)
{
	m_bModelDirty = true;

	EditPoint* pTopLeft = kcpPoint[0];
	EditPoint* pBottomRight = kcpPoint[0];

	for (int nIndex = 1; nIndex != kcpPoint.size(); ++nIndex)
	{
		EditPoint& editPoint = *kcpPoint[nIndex];
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

	EditPoint* pTopRight = &createPoint(WPoint(kvMax[0], kvMax[1]));
	m_pointSelection.select(*pTopRight);
	EditPoint* pBottomLeft = &createPoint(WPoint(kvMin[0], kvMin[1]));
	m_pointSelection.select(*pBottomLeft);

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

	{for (int nIndex = 0; nIndex != kcpPoint.size(); ++nIndex)
	{
		EditPoint& editPoint = *kcpPoint[nIndex];

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
			EditPoint& shadowPoint = createPoint(WPoint(kvMin[0], kvPoint[1]));
			mapIntToPointLeft[kvPoint[1]] = &editPoint;
			mapIntToPointLeft2[kvPoint[1]] = &shadowPoint;
			m_pointSelection.select(shadowPoint);
		}
		else if (fDistRight == fDistClosest)
		{
			EditPoint& shadowPoint = createPoint(WPoint(kvMax[0], kvPoint[1]));
			mapIntToPointRight[kvPoint[1]] = &editPoint;
			mapIntToPointRight2[kvPoint[1]] = &shadowPoint;
			m_pointSelection.select(shadowPoint);
		}
		else if (fDistBottom == fDistClosest)
		{
			EditPoint& shadowPoint = createPoint(WPoint(kvPoint[0], kvMin[1]));
			mapIntToPointBottom[kvPoint[0]] = &editPoint;
			mapIntToPointBottom2[kvPoint[0]] = &shadowPoint;
			m_pointSelection.select(shadowPoint);
		}
		else if (fDistTop == fDistClosest)
		{
			EditPoint& shadowPoint = createPoint(WPoint(kvPoint[0], kvMax[1]));
			mapIntToPointTop[kvPoint[0]] = &editPoint;
			mapIntToPointTop2[kvPoint[0]] = &shadowPoint;
			m_pointSelection.select(shadowPoint);
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
ModelEditor::doCreateInversePolygonFromSelectedPoints()
{
	// TODO Why can't I use a range algorithm to copy into the vector?
	std::vector<EditPoint*> cpPoint;
	for (Selection<EditPoint>::Iterator i = m_pointSelection.begin();
		i != m_pointSelection.end(); ++i)
	{
		cpPoint.push_back(*i);
	}
	createInversePolygon(cpPoint);
}


/*******************************************************************************
	Returns an existing line if one exists between the two points (in any
	order).
*******************************************************************************/
EditLine&
ModelEditor::createLine(
	EditPoint& beginPoint,
	EditPoint& endPoint)
{
	for (int nLineIndex = 0;
		nLineIndex != beginPoint.getNumberOfLines(); ++nLineIndex)
	{
		EditLine& editLine = beginPoint.getLine(nLineIndex);
		if (&editLine.getBeginPoint() == &beginPoint &&
			&editLine.getEndPoint() == &endPoint ||
			&editLine.getBeginPoint() == &endPoint &&
			&editLine.getEndPoint() == &beginPoint)
		{
			// Return existing line.
			return editLine;
		}
	}

	m_bModelDirty = true;

	EditLine& editLine = *new EditLine();
	editLine.setName(getModel().makeNewLineName());
	editLine.setBeginPoint(beginPoint);
	editLine.setEndPoint(endPoint);
	getModel().addLine(editLine);

	return editLine;
}


/*******************************************************************************
*******************************************************************************/
EditPoint&
ModelEditor::createPoint(
	const WPoint& kWorldPosition)
{
	m_bModelDirty = true;

	EditPoint& editPoint = *new EditPoint();
	editPoint.setName(getModel().makeNewPointName());
	editPoint.setPoint(Vec2(kWorldPosition.getX(), kWorldPosition.getY()));
	getModel().addPoint(editPoint);

	return editPoint;
}


/*******************************************************************************
	Will reuse existing lines, but will still create a new polygon even if
	one exists with all of those lines.
*******************************************************************************/
EditPolygon&
ModelEditor::createPolygon(
	const std::vector<EditPoint*> kcpPoint)
{
	m_bModelDirty = true;

	EditPolygon& editPolygon = *new EditPolygon();
	editPolygon.setName(getModel().makeNewPolygonName());

	for (int nBeginIndex = kcpPoint.size() - 1, nEndIndex = 0;
		nEndIndex != kcpPoint.size();
		nBeginIndex = nEndIndex, ++nEndIndex)
	{
		EditPoint& beginPoint = *kcpPoint[nBeginIndex];
		EditPoint& endPoint = *kcpPoint[nEndIndex];

		EditLine& editLine = createLine(beginPoint, endPoint);

		// Add the line to the polygon.
		editPolygon.addLine(editLine);
	}

	getModel().addPolygon(editPolygon);

	return editPolygon;
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doClose()
{
	m_bPendingClose = true;

	if (isModelDirty())
	{
		doSaveChanges();
		return;
	}

	pending(true);
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doCompileModel()
{
	// TODO If menu bar knew how to hide itself this wouldn't be necessary.
	Application::getModelEditorMainWindow().getMenuBar().hide();

	m_bPendingCompile = true;

	if (isModelDirty())
	{
		doSaveChanges();
		return;
	}

	pending(true);
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doCreatePolygonFromSelectedPoints()
{
	// TODO Why can't I use a range algorithm to copy into the vector?
	std::vector<EditPoint*> cpPoint;
	for (Selection<EditPoint>::Iterator i = m_pointSelection.begin();
		i != m_pointSelection.end(); ++i)
	{
		cpPoint.push_back(*i);
	}
	createPolygon(cpPoint);
}


/*******************************************************************************
	This one's pretty simple. We have to take each selected point, find all
	the lines it is part of, then find all polygons they are part of. Then,
	delete those polygons (thus deleting the lines and points). The points
	have to be removed from the selection (i.e. clear the point selection).
*******************************************************************************/
void
ModelEditor::doDeleteSelectedPoints()
{
	// TODO Why can't I use a range algorithm to copy into the vector?
	std::vector<EditPoint*> cpPoint;
	for (Selection<EditPoint>::Iterator i = m_pointSelection.begin();
		i != m_pointSelection.end(); ++i)
	{
		cpPoint.push_back(*i);
	}
	getModel().destroyPoints(cpPoint);
	m_pointSelection.clear();
	m_bModelDirty = true;
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doLoadImage()
{
	// TODO If menu bar knew how to hide itself this wouldn't be necessary.
	Application::getModelEditorMainWindow().getMenuBar().hide();

	// Configure.
	Application::getFileDialog().setCaption(_T("Load Image"));
	Application::getFileDialog().setAcceptButtonText(_T("Load"));
	Application::getFileDialog().setFileName(_T(""));

	// Connect signals/slots.
	Application::getFileDialog().cancelled.connect(
		this, &ModelEditor::doLoadImageCancelled);
	Application::getFileDialog().accepted.connect(
		this, &ModelEditor::doLoadImageAccepted);

	// Show.
	Application::getFileDialog().raise();
	Application::getFileDialog().show();
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doLoadImageAccepted()
{
	// Disconnect signals/slots.
	Application::getFileDialog().cancelled.disconnect_all();
	Application::getFileDialog().accepted.disconnect_all();

	m_sImageName = Application::getFileDialog().getSelectedFileName();

	// Make two points in the top left and bottom right.
	WSurface& surface =
		Resourcex::getImage(m_sImageName.c_str()).getSurface();
	createPoint(WPoint(0, surface.GetHeight()));
	createPoint(WPoint(surface.GetWidth(), 0));
	m_bModelDirty = true;
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doLoadImageCancelled()
{
	// Disconnect signals/slots.
	Application::getFileDialog().cancelled.disconnect_all();
	Application::getFileDialog().accepted.disconnect_all();
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doLoadModel()
{
	// TODO If menu bar knew how to hide itself this wouldn't be necessary.
	Application::getModelEditorMainWindow().getMenuBar().hide();

	// Configure.
	Application::getFileDialog().setCaption(_T("Load Model"));
	Application::getFileDialog().setAcceptButtonText(_T("Load"));
	Application::getFileDialog().setFileName(_T(""));

	// Connect signals/slots.
	Application::getFileDialog().cancelled.connect(
		this, &ModelEditor::doLoadModelCancelled);
	Application::getFileDialog().accepted.connect(
		this, &ModelEditor::doLoadModelAccepted);

	// Show.
	Application::getFileDialog().raise();
	Application::getFileDialog().show();
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doLoadModelAccepted()
{
	// Disconnect signals/slots.
	Application::getFileDialog().cancelled.disconnect_all();
	Application::getFileDialog().accepted.disconnect_all();

	// TODO First, check if specified file is found. If not, handle that error.

	// Remember the load model name.
	m_sLoadModelName = Application::getFileDialog().getSelectedFileName();

	m_bPendingLoad = true;

	if (isModelDirty())
	{
		doSaveChanges();
		return;
	}

	pending(true);
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doLoadModelCancelled()
{
	// Disconnect signals/slots.
	Application::getFileDialog().cancelled.disconnect_all();
	Application::getFileDialog().accepted.disconnect_all();
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doNewModel()
{
	m_bPendingNew = true;

	if (isModelDirty())
	{
		doSaveChanges();
		return;
	}

	pending(true);
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doQuit()
{
	m_bPendingQuit = true;

	if (isModelDirty())
	{
		doSaveChanges();
		return;
	}

	pending(true);
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSaveChanges()
{
	// TODO If menu bar knew how to hide itself this wouldn't be necessary.
	Application::getModelEditorMainWindow().getMenuBar().hide();

	// Configure.
	// TODO Set proper model name.
	Application::getSaveChangesDialog().setText(
		_T("Save changes to modelname?"));

	// Connect signals/slots.
	Application::getSaveChangesDialog().cancelled.connect(
		this, &ModelEditor::doSaveChangesCancelled);
	Application::getSaveChangesDialog().rejected.connect(
		this, &ModelEditor::doSaveChangesRejected);
	Application::getSaveChangesDialog().accepted.connect(
		this, &ModelEditor::doSaveChangesAccepted);

	// Show.
	Application::getSaveChangesDialog().raise();
	Application::getSaveChangesDialog().show();
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSaveChangesAccepted()
{
	// Disconnect signals/slots.
	Application::getSaveChangesDialog().cancelled.disconnect_all();
	Application::getSaveChangesDialog().rejected.disconnect_all();
	Application::getSaveChangesDialog().accepted.disconnect_all();

	doSaveModel();
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSaveChangesCancelled()
{
	// Disconnect signals/slots.
	Application::getSaveChangesDialog().cancelled.disconnect_all();
	Application::getSaveChangesDialog().rejected.disconnect_all();
	Application::getSaveChangesDialog().accepted.disconnect_all();

	pending(false);
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSaveChangesRejected()
{
	// Disconnect signals/slots.
	Application::getSaveChangesDialog().cancelled.disconnect_all();
	Application::getSaveChangesDialog().rejected.disconnect_all();
	Application::getSaveChangesDialog().accepted.disconnect_all();

	pending(true);
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSaveModel()
{
	// TODO If menu bar knew how to hide itself this wouldn't be necessary.
	Application::getModelEditorMainWindow().getMenuBar().hide();

	if (!isModelDirty())
	{
		return;
	}

	if (isModelNew())
	{
		doSaveModelAs();
	}
	else
	{
		saveModel(getModel().getName());
		pending(true);
	}
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSaveModelAs()
{
	// TODO If menu bar knew how to hide itself this wouldn't be necessary.
	Application::getModelEditorMainWindow().getMenuBar().hide();

	// Configure.
	Application::getFileDialog().setCaption(_T("Save Model As"));
	Application::getFileDialog().setAcceptButtonText(_T("Save"));
	Application::getFileDialog().setFileName(_T(""));

	// Connect signals/slots.
	Application::getFileDialog().cancelled.connect(
		this,
		&ModelEditor::doSaveModelAsCancelled);
	Application::getFileDialog().accepted.connect(
		this,
		&ModelEditor::doSaveModelAsAccepted);

	// Show.
	Application::getFileDialog().raise();
	Application::getFileDialog().show();
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSaveModelAsAccepted()
{
	// Disconnect signals/slots.
	Application::getFileDialog().cancelled.disconnect_all();
	Application::getFileDialog().accepted.disconnect_all();

	saveModel(Application::getFileDialog().getSelectedFileName());

	pending(true);
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSaveModelAsCancelled()
{
	// Disconnect signals/slots.
	Application::getFileDialog().cancelled.disconnect_all();
	Application::getFileDialog().accepted.disconnect_all();

	pending(false);
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSetImageOrigin()
{
	// TODO If menu bar knew how to hide itself this wouldn't be necessary.
	Application::getModelEditorMainWindow().getMenuBar().hide();

	// Configure.
	ModelEditor::getSetImageOriginDialog().setImageOrigin(m_imageOrigin);

	// Show.
	ModelEditor::getSetImageOriginDialog().raise();
	ModelEditor::getSetImageOriginDialog().show();
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSetImageOriginAccepted()
{
	// No need to disconnect signals/slots.

	// Set the image origin from the dialog.
	m_imageOrigin = ModelEditor::getSetImageOriginDialog().getImageOrigin();
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::drawImage()
{
	if (m_sImageName.empty())
	{
		return;
	}

	// We want the image to be positioned in the world so that its bottom left
	// corner is at the origin. Therefore, we want to draw from its top left
	// corner, in screen coordinates.
	WSurface& surface =
		Resourcex::getImage(m_sImageName.c_str()).getSurface();
	const WPoint kWorldTopLeft =
		WPoint(m_imageOrigin.getX(),
		m_imageOrigin.getY() + surface.GetHeight());
	const WPoint kScreenTopLeft = convertToScreen(kWorldTopLeft);

	getBackBuffer().BltFast(
		kScreenTopLeft.getX(),
		kScreenTopLeft.getY(),
		&surface,
		NULL,
		0,
		NULL);
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::drawLasso()
{
	if (!m_bStylusDragging || m_pStylusDownPoint || m_nActiveToolIndex != 0)
	{
		return;
	}

	WPoint downScreenPosition = convertToScreen(m_stylusDownWorldPosition);
	WPoint dragScreenPosition = convertToScreen(m_stylusDragWorldPosition);

	RECT screenRect;
	screenRect.top = downScreenPosition.getY();
	screenRect.left = downScreenPosition.getX();
	screenRect.bottom = dragScreenPosition.getY();
	screenRect.right = dragScreenPosition.getX();

	getBackBuffer().DrawRect(
		&screenRect,
		Colour::highlight(),
		0,
		NULL);
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::drawModelLines()
{
	for (int nLineIndex = 0;
		nLineIndex != getModel().getNumberOfLines(); ++nLineIndex)
	{
		EditLine& editLine = getModel().getLine(nLineIndex);

		EditPoint& beginPoint = editLine.getBeginPoint();
		EditPoint& endPoint = editLine.getEndPoint();

		const WPoint kBeginPosition =
			convertToScreen(beginPoint.getPoint());
		const WPoint kEndPosition =
			convertToScreen(endPoint.getPoint());

		getBackBuffer().DrawLine(
			kBeginPosition.getX(),
			kBeginPosition.getY(),
			kEndPosition.getX(),
			kEndPosition.getY(),
			m_lineSelection.isSelected(editLine) ?
				Colour::highlight() :
				Colour::white(),
			0,
			NULL);
	}
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::drawModelPoint(
	EditPoint& editPoint)
{
	const WPoint kScreenPosition =
		convertToScreen(editPoint.getPoint());

	// This is the size we are going to draw.
	const int knOffset = 2;

	for (int nY = kScreenPosition.getY() - knOffset;
		nY <= kScreenPosition.getY() + knOffset;
		++nY)
	{
		for (int nX = kScreenPosition.getX() - knOffset;
			nX <= kScreenPosition.getX() + knOffset;
			++nX)
		{
			getBackBuffer().SetPixel(
				nX,
				nY,
				m_pointSelection.isSelected(editPoint) ?
					Colour::highlight() :
					Colour::white());
		}
	}
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::drawModelPoints()
{
	for (int nPointIndex = 0;
		nPointIndex != getModel().getNumberOfPoints(); ++nPointIndex)
	{
		drawModelPoint(getModel().getPoint(nPointIndex));
	}
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::eventKeyPress(
	const int knKeyCode)
{
	if (knKeyCode == KeyCode::menu())
	{
		WidgetMenuBar& menuBar =
			static_cast<WidgetMainWindow&>(getParent()).getMenuBar();
		menuBar.show();
	}
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::eventPaint()
{
	// TODO Fix this.
	COLORREF colour = RGB(0, 0, 0);
	RECT screenRect = reinterpret_cast<const RECT&>(getGlobalBounds());
	getBackBuffer().FillRect(
		&screenRect,
		colour,
		0,
		NULL);

	drawImage();
	drawModelLines();
	drawModelPoints();
	drawLasso();
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::eventProcess()
{
	WidgetWorldView::eventProcess();

	// TODO For now, just update the toolbar state here. Later, find a better
	// place to do this, or at least clean up the code.
	Application::getModelEditorMainWindow().getToolBar().
		getChild(2).setEnabled(3 <= m_pointSelection.getSize());
	Application::getModelEditorMainWindow().getToolBar().
		getChild(3).setEnabled(6 <= m_pointSelection.getSize());
	Application::getModelEditorMainWindow().getToolBar().
		getChild(4).setEnabled(!m_pointSelection.isEmpty());
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::eventStylusDown(
	const WPoint& kPosition)
{
	m_stylusDownWorldPosition = convertToWorld(kPosition);

	if (m_nActiveToolIndex == 0)
	{
		selectDown();
	}
	else if (m_nActiveToolIndex == 1)
	{
		createPoint(m_stylusDownWorldPosition);
	}
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::eventStylusMove(
	const WPoint& kPosition)
{
	m_bStylusDragging = true;
	m_stylusDragWorldPosition = convertToWorld(kPosition);

	if (m_nActiveToolIndex == 0)
	{
		if (m_pStylusDownPoint)
		{
			moveDrag();
		}
		else
		{
			selectDrag();
		}
	}
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::eventStylusUp(
	const WPoint& kPosition)
{
	m_stylusUpWorldPosition = convertToWorld(kPosition);

	if (m_nActiveToolIndex == 0)
	{
		selectUp();
	}

	m_bStylusDragging = false;
}


/*******************************************************************************
*******************************************************************************/
std::vector<EditLine*>
ModelEditor::getLines(
	const WPoint& kWorldPosition)
{
	std::vector<EditLine*> cpLine;

	const int knOffset = 4;
	const Vec2 kvWorldPoint(kWorldPosition.getX(), kWorldPosition.getY());

	for (int nLineIndex = 0;
		nLineIndex != getModel().getNumberOfLines(); ++nLineIndex)
	{
		EditLine& editLine = getModel().getLine(nLineIndex);

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

		if (-knOffset <= kfDotNormal &&
			kfDotNormal <= knOffset &&
			0 <= kfDotDirection &&
			kfDotDirection <= kfDotLine)
		{
			cpLine.push_back(&editLine);
		}
	}

	return cpLine;
}


/*******************************************************************************
*******************************************************************************/
std::vector<EditPoint*>
ModelEditor::getPoints(
	const WPoint& kWorldPosition)
{
	std::vector<EditPoint*> cpPoint;

	const int knOffset = 4;

	for (int nPointIndex = 0;
		nPointIndex != getModel().getNumberOfPoints(); ++nPointIndex)
	{
		EditPoint& editPoint = getModel().getPoint(nPointIndex);

		const WPoint kDistance =
			WPoint(
				static_cast<int>(editPoint.getPoint()[0]) -
					kWorldPosition.getX(),
				static_cast<int>(editPoint.getPoint()[1]) -
					kWorldPosition.getY());

		if (-knOffset <= kDistance.getX() &&
			kDistance.getX() <= knOffset &&
			-knOffset <= kDistance.getY() &&
			kDistance.getY() <= knOffset)
		{
			cpPoint.push_back(&editPoint);
		}
	}

	return cpPoint;
}


/*******************************************************************************
*******************************************************************************/
std::vector<EditPoint*>
ModelEditor::getPoints(
	const WRect& kWorldBounds)
{
	std::vector<EditPoint*> cpPoint;

	for (int nPointIndex = 0;
		nPointIndex != getModel().getNumberOfPoints(); ++nPointIndex)
	{
		EditPoint& editPoint = getModel().getPoint(nPointIndex);

		const WPoint kWorldPosition =
			WPoint(editPoint.getPoint()[0], editPoint.getPoint()[1]);

		if (kWorldBounds.contains(kWorldPosition))
		{
			cpPoint.push_back(&editPoint);
		}
	}

	return cpPoint;
}


/*******************************************************************************
*******************************************************************************/
SetImageOriginDialog&
ModelEditor::getSetImageOriginDialog()
{
	if (!m_pSetImageOriginDialog)
	{
		m_pSetImageOriginDialog =
			new SetImageOriginDialog(_T("set_image_origin_dialog"));
		m_pSetImageOriginDialog->accepted.connect(
			&static_cast<ModelEditor&>(
				Application::getModelEditorMainWindow().getContents()),
			&ModelEditor::doSetImageOriginAccepted);
	}

	return *m_pSetImageOriginDialog;
}


/*******************************************************************************
	Helper for getting tool buttons by name from the tool bar. Will blow up if
	not found!
*******************************************************************************/
WidgetToolButton&
ModelEditor::getToolButton(
	const tstring& ksName)
{
	WidgetToolBar& toolBar =
		Application::getModelEditorMainWindow().getToolBar();

	for (int nChildIndex = 0;
		nChildIndex != toolBar.getNumberOfChildren(); ++nChildIndex)
	{
		WidgetToolButton& toolButton =
			static_cast<WidgetToolButton&>(toolBar.getChild(nChildIndex));
		if (toolButton.getName() == ksName)
		{
			return toolButton;
		}
	}

	// Shouldn't get here.
	WidgetToolButton* pDummy = 0;
	return *pDummy;
}

		
/*******************************************************************************
*******************************************************************************/
void
ModelEditor::loadModel(
	const tstring& ksModelName)
{
	m_pModel = &Resourcex::loadEditModel(ksModelName);
	m_bModelDirty = false;
	m_bModelNew = false;
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::moveDrag()
{
	m_bModelDirty = true;

	// One extra thing we have to do here. If the item we came down on wasn't
	// selected, and we are using the control key, select it first.
	if (Key::isKeyDown(KeyCode::control()) &&
		!m_pointSelection.isSelected(*m_pStylusDownPoint))
	{
		m_pointSelection.select(*m_pStylusDownPoint);
	}

	// Assume we are moving from the down point position to the current
	// drag position.

	const Vec2 kvDelta(
		m_stylusDragWorldPosition.getX() - m_pStylusDownPoint->getPoint()[0],
		m_stylusDragWorldPosition.getY() - m_pStylusDownPoint->getPoint()[1]);

	for (Selection<EditPoint>::Iterator i = m_pointSelection.begin();
		i != m_pointSelection.end(); ++i)
	{
		(*i)->setPoint((*i)->getPoint() + kvDelta);
	}
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::newModel()
{
	if (m_pModel)
	{
		delete m_pModel;
	}

	m_pModel = new EditModel();
	m_bModelDirty = false;
	m_bModelNew = true;
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::pending(
	const bool kbExecute)
{
	if (m_bPendingClose)
	{
		if (kbExecute)
		{
			closeEditor();
		}
		m_bPendingClose = false;
	}
	else if (m_bPendingCompile)
	{
		if (kbExecute)
		{
			compileModel();
		}
		m_bPendingCompile = false;
	}
	else if (m_bPendingLoad)
	{
		if (kbExecute)
		{
			loadModel(m_sLoadModelName);
		}
		m_bPendingLoad = false;
	}
	else if (m_bPendingNew)
	{
		if (kbExecute)
		{
			newModel();
		}
		m_bPendingNew = false;
	}
	else if (m_bPendingQuit)
	{
		if (kbExecute)
		{
			Application::quit();
		}
		m_bPendingQuit = false;
	}
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::saveModel(
	const tstring& ksModelName)
{
	getModel().setName(ksModelName);

	m_bModelDirty = false;
	m_bModelNew = false;

	// TODO This file must go into the proper directory.

	FILE* pFile = File::openFile(
		tstring(_T("data\\edit_models\\")) +
			ksModelName + _T(".txt"), _T("w"));

	_ftprintf(pFile, _T("edit_model\n"));
	_ftprintf(pFile, _T("{\n"));

	_ftprintf(pFile, _T("\tedit_points\n"));
	_ftprintf(pFile, _T("\t{\n"));
	for (int nPointIndex = 0;
		nPointIndex != getModel().getNumberOfPoints(); ++nPointIndex)
	{
		EditPoint& editPoint = getModel().getPoint(nPointIndex);
		_ftprintf(pFile, _T("\t\t%s = <%f, %f>\n"),
			editPoint.getName().c_str(),
			editPoint.getPoint()[0],
			editPoint.getPoint()[1]);
	}
	_ftprintf(pFile, _T("\t}\n"));

	_ftprintf(pFile, _T("\tedit_lines\n"));
	_ftprintf(pFile, _T("\t{\n"));
	for (int nLineIndex = 0;
		nLineIndex != getModel().getNumberOfLines(); ++nLineIndex)
	{
		EditLine& editLine = getModel().getLine(nLineIndex);
		_ftprintf(pFile, _T("\t\t%s = %s -> %s\n"),
			editLine.getName().c_str(),
			editLine.getBeginPoint().getName().c_str(),
			editLine.getEndPoint().getName().c_str());
	}
	_ftprintf(pFile, _T("\t}\n"));

	for (int nPolygonIndex = 0;
		nPolygonIndex != getModel().getNumberOfPolygons(); ++nPolygonIndex)
	{
		EditPolygon& editPolygon = getModel().getPolygon(nPolygonIndex);
		_ftprintf(pFile, _T("\tedit_poly\n"));
		_ftprintf(pFile, _T("\t{\n"));
		for (int nLineIndex = 0;
			nLineIndex != editPolygon.getNumberOfLines(); ++nLineIndex)
		{
			EditLine& editLine = editPolygon.getLine(nLineIndex);
			_ftprintf(pFile, _T("\t\t%s\n"),
				editLine.getName().c_str());
		}
		_ftprintf(pFile, _T("\t}\n"));
	}

	_ftprintf(pFile, _T("}\n"));

	File::closeFile(pFile);
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::selectDown()
{
	m_pStylusDownPoint = 0;
	m_pStylusDownLine = 0;
	
	std::vector<EditPoint*> cpPoint = getPoints(m_stylusDownWorldPosition);

	if (!cpPoint.empty())
	{
		// We clicked on a point.

		if (!m_pointSelection.isSelected(*cpPoint[0]))
		{
			// The point was not selected.
			if (Key::isKeyDown(KeyCode::shift()))
			{
				m_pointSelection.select(cpPoint);
			}
			else if (!Key::isKeyDown(KeyCode::control()))
			{
				m_pointSelection.clear();
				m_pointSelection.select(cpPoint);
			}
		}

		// Remember the point we clicked on.
		m_pStylusDownPoint = cpPoint[0];
	}

#if 0
	if (!m_pointSelection.isEmpty())
	{
		m_pDragPoint = cpPoint[0];
		m_lineSelection.clear();
		return;
	}
	else if (!cpPoint.empty())
	{
		return;
	}

	std::vector<EditLine*> cpLine = getLines(kWorldPosition);

	if (Key::isKeyDown(KeyCode::control()))
	{
		m_lineSelection.toggle(cpLine);
	}
	else
	{
		m_lineSelection.select(cpLine);
	}

	if (!m_lineSelection.isEmpty())
	{
		m_pDragLine = cpLine[0];
		return;
	}
	else if (!cpLine.empty())
	{
		return;
	}
#endif
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::selectDrag()
{
	if (!Key::isKeyDown(KeyCode::shift()) &&
		!Key::isKeyDown(KeyCode::control()))
	{
		m_pointSelection.clear();
	}
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::selectUp()
{
	int nMinX, nMinY, nMaxX, nMaxY;
	if (m_stylusDownWorldPosition.getX() <= m_stylusUpWorldPosition.getX())
	{
		nMinX = m_stylusDownWorldPosition.getX();
		nMaxX = m_stylusUpWorldPosition.getX();
	}
	else
	{
		nMinX = m_stylusUpWorldPosition.getX();
		nMaxX = m_stylusDownWorldPosition.getX();
	}
	if (m_stylusDownWorldPosition.getY() <= m_stylusUpWorldPosition.getY())
	{
		nMinY = m_stylusDownWorldPosition.getY();
		nMaxY = m_stylusUpWorldPosition.getY();
	}
	else
	{
		nMinY = m_stylusUpWorldPosition.getY();
		nMaxY = m_stylusDownWorldPosition.getY();
	}
	WRect dragRect(nMinX, nMinY, nMaxX - nMinX, nMaxY - nMinY);

	if (m_pStylusDownPoint)
	{
		if (Key::isKeyDown(KeyCode::control()) && !m_bStylusDragging)
		{
			// Toggle.
			m_pointSelection.toggle(*m_pStylusDownPoint);
		}
	}
	else
	{
		if (Key::isKeyDown(KeyCode::shift()))
		{
			// Add.
			m_pointSelection.select(getPoints(dragRect));
		}
		else if (Key::isKeyDown(KeyCode::control()))
		{
			// Toggle.
			m_pointSelection.toggle(getPoints(dragRect));
		}
		else
		{
			m_pointSelection.clear();
			m_pointSelection.select(getPoints(dragRect));
		}
	}
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::toolToggle(
	const bool kbOn)
{
	WidgetToolBar& toolBar =
		static_cast<WidgetMainWindow&>(getParent()).getToolBar();

	WidgetToolButton& activeToolButton =
		static_cast<WidgetToolButton&>(toolBar.getChild(m_nActiveToolIndex));

	// We have to toggle the active button no matter what.
	activeToolButton.toggle();

	if (kbOn)
	{
		// An inactive tool was toggled on. We toggled the active tool off.
		// Now find the new active tool.
		for (int nToolIndex = 0; nToolIndex != 3; ++nToolIndex)
		{
			WidgetToolButton& button =
				static_cast<WidgetToolButton&>(
					toolBar.getChild(nToolIndex));
			if (button.isOn())
			{
				m_nActiveToolIndex = nToolIndex;
				break;
			}
		}
	}
	else
	{
		// The active tool was toggled off. We toggled it back on.
	}
}
