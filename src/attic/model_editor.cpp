// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "editor/model_editor.h"

#include "application.h"
#include "edit_model.h"
#include "resourcex.h"
#include "widget_file_dialog.h"
#include "widget_main_window.h"
#include "widget_message_dialog.h"


ModelEditor
ModelEditor::m_modelEditor;

WidgetMessageDialog*
ModelEditor::m_pSaveChangesMessageDialog = 0;

EditModel*
ModelEditor::m_pModel = 0;

bool
ModelEditor::m_bModelDirty = false;

bool
ModelEditor::m_bModelNew = false;

Selection<EditPoint>
ModelEditor::m_pointSelection;

Selection<EditLine>
ModelEditor::m_lineSelection;

Selection<EditPolygon>
ModelEditor::m_polygonSelection;

tstring
ModelEditor::m_sLoadModelName;


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doLoadModel()
{
	Application::getFileDialog().setCaption(_T("Load Model"));
	Application::getFileDialog().setAcceptButtonText(_T("Load"));
	Application::getFileDialog().setFileName(_T(""));

	// Connect signals/slots.
	Application::getFileDialog().cancelled.connect(
		&ModelEditor::getSingleton(),
		&ModelEditor::doLoadModelCancelled);
	Application::getFileDialog().accepted.connect(
		&ModelEditor::getSingleton(),
		&ModelEditor::doLoadModelAccepted);

	// TODO If menu bar knew how to hide itself this wouldn't be necessary.
	Application::getModelEditorMainWindow().getMenuBar().hide();

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

	if (isModelDirty())
	{
		doSaveChanges();
		return;
	}

	loadModel(m_sLoadModelName);
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
	// TODO Check if old model needs changes saved.

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
ModelEditor::doSaveModel()
{
	// TODO If not dirty, don't save.
	// If new, do save as.
	// Otherwise, just save.
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSaveChanges()
{
	if (!m_pSaveChangesMessageDialog)
	{
		m_pSaveChangesMessageDialog =
			new WidgetMessageDialog(_T("save_changes"));
	}

	WidgetMessageDialog& messageDialog = *m_pSaveChangesMessageDialog;
	// TODO Set proper model name.
	messageDialog.setText(_T("Save changes to modelname?"));

	// Connect signals/slots.
	messageDialog.cancelled.connect(
		&ModelEditor::getSingleton(),
		&ModelEditor::doSaveChangesCancelled);
	messageDialog.rejected.connect(
		&ModelEditor::getSingleton(),
		&ModelEditor::doSaveChangesRejected);
	messageDialog.accepted.connect(
		&ModelEditor::getSingleton(),
		&ModelEditor::doSaveChangesAccepted);

	messageDialog.show();
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSaveChangesAccepted()
{
	// Disconnect signals/slots.
	m_pSaveChangesMessageDialog->cancelled.disconnect_all();
	m_pSaveChangesMessageDialog->rejected.disconnect_all();
	m_pSaveChangesMessageDialog->accepted.disconnect_all();

	doSaveModelAs();

	m_sLoadModelName = _T("");
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSaveChangesCancelled()
{
	// Disconnect signals/slots.
	m_pSaveChangesMessageDialog->cancelled.disconnect_all();
	m_pSaveChangesMessageDialog->rejected.disconnect_all();
	m_pSaveChangesMessageDialog->accepted.disconnect_all();

	m_sLoadModelName = _T("");
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSaveChangesRejected()
{
	// Disconnect signals/slots.
	m_pSaveChangesMessageDialog->cancelled.disconnect_all();
	m_pSaveChangesMessageDialog->rejected.disconnect_all();
	m_pSaveChangesMessageDialog->accepted.disconnect_all();

	loadModel(m_sLoadModelName);

	m_sLoadModelName = _T("");
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSaveModelAs()
{
	Application::getFileDialog().setCaption(_T("Save Model As"));
	Application::getFileDialog().setAcceptButtonText(_T("Save"));
	Application::getFileDialog().setFileName(_T(""));

	// Connect signals/slots.
	Application::getFileDialog().cancelled.connect(
		&ModelEditor::getSingleton(),
		&ModelEditor::doSaveModelAsCancelled);
	Application::getFileDialog().accepted.connect(
		&ModelEditor::getSingleton(),
		&ModelEditor::doSaveModelAsAccepted);

	// TODO If menu bar knew how to hide itself this wouldn't be necessary.
	Application::getModelEditorMainWindow().getMenuBar().hide();

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

	// TODO Need to actually save model here. Check for existing file.
}


/*******************************************************************************
*******************************************************************************/
void
ModelEditor::doSaveModelAsCancelled()
{
	// Disconnect signals/slots.
	Application::getFileDialog().cancelled.disconnect_all();
	Application::getFileDialog().accepted.disconnect_all();
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
