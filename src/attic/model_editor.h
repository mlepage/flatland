// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__MODEL_EDITOR
#define GUARD__MODEL_EDITOR


#include "sigslot.h"

#include "selection.h"
#include "tstring.h"


class EditLine;
class EditModel;
class EditPoint;
class EditPolygon;
class WidgetMessageDialog;


/*******************************************************************************
	Undo/redo functionality is deferred. When implemented, just store a stack
	of mementos with models and selections and whatever. But still always work
	on the local copies. Just be sure to checkpoint a new memento whenever
	something changes.
*******************************************************************************/
class ModelEditor : public sigslot::has_slots<>
{


public:

	static
	EditModel&
	getModel()
	{
		return *m_pModel;
	}

	static
	ModelEditor&
	getSingleton()
	{
		return m_modelEditor;
	}

	static
	bool
	isModelDirty()
	{
		return m_bModelDirty;
	}

	static
	bool
	isModelNew()
	{
		return m_bModelNew;
	}

	static
	void
	setModel(
		EditModel& model)
	{
		m_pModel = &model;
	}


public:
	
	void
	doLoadModel();

	void
	doNewModel();

	void
	doSaveModel();

	void
	doSaveModelAs();


private:

	static
	void
	loadModel(
		const tstring& ksModelName);


private:

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


private:

	// Singleton.
	static ModelEditor m_modelEditor;

	// Owned.
	static
	WidgetMessageDialog* m_pSaveChangesMessageDialog;

	// Owned.
	static
	EditModel* m_pModel;

	static
	bool m_bModelDirty;

	static
	bool m_bModelNew;

	// Selection.
	static Selection<EditPoint> m_pointSelection;
	static Selection<EditLine> m_lineSelection;
	static Selection<EditPolygon> m_polygonSelection;

	static
	tstring m_sLoadModelName;


};


#endif // GUARD
