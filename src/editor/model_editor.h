// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__MODEL_EDITOR
#define GUARD__MODEL_EDITOR


#include <vector>

#include "widget_world_view.h"
#include "editor/selection.h"


class EditLine;
class EditModel;
class EditPoint;
class EditPolygon;
class SetImageOriginDialog;
class WidgetToolButton;


/*******************************************************************************
*******************************************************************************/
class ModelEditor : public WidgetWorldView
{


public:

	static
	SetImageOriginDialog&
	getSetImageOriginDialog();


public:

	ModelEditor()
	{
		constructCommon();
	}

	ModelEditor(
		const tstring& ksName) :
		WidgetWorldView(ksName)
	{
		constructCommon();
	}

	ModelEditor(
		Widget& parent) :
		WidgetWorldView(parent)
	{
		constructCommon();
	}

	ModelEditor(
		Widget& parent,
		const tstring& ksName) :
		WidgetWorldView(parent, ksName)
	{
		constructCommon();
	}

	EditModel&
	getModel() const
	{
		return *m_pModel;
	}

	bool
	isModelDirty() const
	{
		return m_bModelDirty;
	}

	bool
	isModelNew() const
	{
		return m_bModelNew;
	}

	void
	toolToggle(
		const bool kbOn);


public:

	void
	doClose();

	void
	doCompileModel();

	void
	doCreateInversePolygonFromSelectedPoints();

	void
	doCreatePolygonFromSelectedPoints();

	void
	doDeleteSelectedPoints();

	void
	doLoadImage();

	void
	doLoadModel();

	void
	doNewModel();

	void
	doQuit();

	void
	doSaveModel();

	void
	doSaveModelAs();

	void
	doSetImageOrigin();


protected:
	
	virtual
	void
	eventKeyPress(
		const int knKeyCode);

	virtual
	void
	eventPaint();

	virtual
	void
	eventProcess();

	virtual
	void
	eventShow()
	{
		doNewModel();
	}

	virtual
	void
	eventStylusDown(
		const WPoint& kPosition);

	virtual
	void
	eventStylusMove(
		const WPoint& kPosition);

	virtual
	void
	eventStylusUp(
		const WPoint& kPosition);


private:

	static
	SetImageOriginDialog* m_pSetImageOriginDialog;


private:

	void
	closeEditor();

	void
	compileModel();

	void
	constructCommon()
	{
		m_pModel = 0;
		m_bModelDirty = false;
		m_bModelNew = false;
		m_nActiveToolIndex = 0;
		m_pImage = 0;
		m_bPendingClose = false;
		m_bPendingCompile = false;
		m_bPendingLoad = false;
		m_bPendingNew = false;
		m_bPendingQuit = false;
		m_bStylusDragging = false;
		m_pStylusDownPoint = 0;
		m_pStylusDownLine = 0;
	}

	void
	createInversePolygon(
		const std::vector<EditPoint*> kcpPoint);

	EditLine&
	createLine(
		EditPoint& beginPoint,
		EditPoint& endPoint);

	EditPoint&
	createPoint(
		const WPoint& kWorldPosition);

	EditPolygon&
	createPolygon(
		const std::vector<EditPoint*> kcpPoint);

	void
	doLoadImageAccepted();

	void
	doLoadImageCancelled();

	void
	doLoadModelAccepted();

	void
	doLoadModelCancelled();

	void
	doSaveChanges();

	void
	doSaveChangesAccepted();

	void
	doSaveChangesCancelled();

	void
	doSaveChangesRejected();

	void
	doSaveModelAsAccepted();

	void
	doSaveModelAsCancelled();

	void
	doSetImageOriginAccepted();

	void
	drawImage();

	void
	drawLasso();

	void
	drawModelPoint(
		EditPoint& editPoint);

	void
	drawModelPoints();

	void
	drawModelLines();

	// Returns the lines near a position.
	std::vector<EditLine*>
	getLines(
		const WPoint& kWorldPosition);

	// Returns the points near a position.
	std::vector<EditPoint*>
	getPoints(
		const WPoint& kWorldPosition);

	// Returns the points in a rectangle.
	std::vector<EditPoint*>
	getPoints(
		const WRect& kWorldBounds);

	WidgetToolButton&
	getToolButton(
		const tstring& ksName);

	void
	loadModel(
		const tstring& ksModelName);

	void
	moveDrag();

	void
	newModel();

	void
	pending(
		const bool kbExecute);

	void
	saveModel(
		const tstring& ksModelName);

	void
	selectDown();

	void
	selectDrag();

	void
	selectUp();


private:

	// Owned.
	EditModel* m_pModel;
	bool m_bModelDirty;
	bool m_bModelNew;

	int m_nActiveToolIndex;

	WSurface* m_pImage;
	WPoint m_imageOrigin;
	tstring m_sImageName;

	// Use this to determine whether we are supposed to be loading a model,
	// and if so, what name.
	tstring m_sLoadModelName;
	bool m_bPendingClose;
	bool m_bPendingCompile;
	bool m_bPendingLoad;
	bool m_bPendingNew;
	bool m_bPendingQuit;

	// Selection.
	Selection<EditPoint> m_pointSelection;
	Selection<EditLine> m_lineSelection;

	// Dragging.
	bool m_bStylusDragging;
	WPoint m_stylusDownWorldPosition;
	WPoint m_stylusDragWorldPosition;
	WPoint m_stylusUpWorldPosition;
	EditPoint* m_pStylusDownPoint;
	EditLine* m_pStylusDownLine;


};


#endif // GUARD
