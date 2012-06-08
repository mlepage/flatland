// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "resourcex.h"

#include <cstdio>
#include "GapiDraw.h"
#include "console.h"
#include "error.h"
#include "exception.h"
#include "file.h"
#include "game.h"
#include "model.h"
#include "parser.h"
#include "text_buffer.h"
#include "variable.h"


Directory<Model>
Resourcex::m_modelDirectory;


/*******************************************************************************
	Returns the model. Loads it if necessary. Fatal error if not found.
*******************************************************************************/
Model&
Resourcex::getModel(
	const tstring& ksName)
{
	Model* pModel;

	// Find the model.
	if (pModel = m_modelDirectory.find(ksName))
	{
		return *pModel;
	}

	// Load the model.
	return loadModel(ksName);
}


/*******************************************************************************
*******************************************************************************/
void
Resourcex::init()
{
	// TODO this should all be in File::init
	_TCHAR atModuleFileName[FLATLAND_FILENAME_MAX];
	GetModuleFileName(NULL, atModuleFileName, FLATLAND_FILENAME_MAX);
	_TCHAR* const kptLastDirSeparator = _tcsrchr(atModuleFileName, _T('\\'));
	if (kptLastDirSeparator != NULL)
	{
		kptLastDirSeparator[1] = _T('\0');
	}
	else
	{
		// Error?
	}
	Variable::file_base_dir.setValue(atModuleFileName);
}


/*******************************************************************************
	The image name is a simple name, e.g. "world".
*******************************************************************************/
CGapiSurface&
Resourcex::loadImage(
	const _TCHAR* const kpkName)
{
	// TODO fix this mess
	TextBuffer<256> fileName;
	if (kpkName[0] == _T('\\'))
	{
		// Already absolute name.
		_sntprintf(
			fileName.getBuffer(),
			fileName.getSize(),
			_T("%s.png"),
			kpkName);
	}
	else
	{
		_sntprintf(
			fileName.getBuffer(),
			fileName.getSize(),
			_T("%s\\data\\images\\%s.png"),
			Variable::file_base_dir.getValue().c_str(),
			kpkName);
	}

	// TODO manage this resource
	CGapiSurface* pImage = new CGapiSurface();

	HRESULT result = pImage->CreateSurface(0, fileName.getBuffer());
	if (result != GD_OK)
	{
		Console::print(_T("Error loading resource: %s\n"),
			fileName.getBuffer());
		Console::print(_T("error code %d\n"), result);
		Console::print(_T("codes: %d %d %d %d %d %d %d %d\n"),
			GDERR_BITMAPNOTFOUND,
			GDERR_INCOMPATIBLEPRIMARY,
			GDERR_INVALIDBITMAP,
			GDERR_INVALIDPARAMS,
			GDERR_INVALIDSURFACETYPE,
			GDERR_LOCKEDSURFACES,
			GDERR_OUTOFMEMORY, 
			GDERR_SURFACELOST);
	}
	pImage->SetColorKey(RGB(0, 0, 0));

	Console::print(_T("Resource loaded: %s\n"), fileName.getBuffer());

	return *pImage;
}


/*******************************************************************************
	The level name is a simple name, e.g. "demo1".
*******************************************************************************/
void
Resourcex::loadLevel(
	const tstring& ksName)
{
	const tstring ksFileName =
		tstring(_T("data\\levels\\")) +
		ksName +
		tstring(_T(".txt"));

	FILE* pFile = File::openFile(ksFileName, _T("r"));

	Parser parser(pFile);

	while (parser.queryLine(_T("entity")))
	{
		Entity& entity = Game::spawnEntity();
		parser.parseEntity(entity);
		World::linkEntity(entity);
	}
}


/*******************************************************************************
	The model name is a simple name, e.g. "world".
*******************************************************************************/
Model&
Resourcex::loadModel(
	const tstring& ksName)
{
	const tstring ksFileName =
		tstring(_T("data\\models\\")) +
		ksName +
		tstring(_T(".txt"));

	FILE* pFile = File::openFile(ksFileName, _T("r"));

	Model* pModel = new Model();
	m_modelDirectory.insert(ksName, *pModel);

	pModel->setName(ksName);

	TRY
	{
		Parser parser(pFile);
		parser.parseModel(*pModel);
	}
	CATCHALL
	{
		m_modelDirectory.erase(ksName);
		RETHROW;
	}
	END_TRY_CATCHALL

	File::closeFile(pFile);

	// Load the image.
	// Can't do this until we start loading models after app is running.
	pModel->setImage(loadImage(ksName.c_str()));
	// TODO check that model image is same size as model bounds

	return *pModel;
}


/*******************************************************************************
*******************************************************************************/
FILE*
Resourcex::openFile(
	const _TCHAR* const kpktName,
	const _TCHAR* const kpktMode)
{
	_TCHAR atAbsoluteName[FLATLAND_FILENAME_MAX + 1];
	atAbsoluteName[FLATLAND_FILENAME_MAX] = _T('\0');

	FILE* pFile;

	if (kpktName[0] == _T('\\'))
	{
		// Absolute name.
		pFile = _tfopen(kpktName, kpktMode);
		Console::print(_T("openFile: %s\n"), kpktName);
	}
	else
	{
		// Relative name, so use base dir.
		_sntprintf(
			atAbsoluteName,
			FLATLAND_FILENAME_MAX,
			_T("%s%s"),
			Variable::file_base_dir.getValue().c_str(),
			kpktName);
		pFile = _tfopen(atAbsoluteName, kpktMode);
		Console::print(_T("openFile: %s\n"), atAbsoluteName);
	}

	// TODO error logging, error handling

	return pFile;
}
