// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "resourcex.h"

#include <cstdio>
#include "GapiDraw.h"
#include "anim.h"
#include "console.h"
#include "error.h"
#include "exception.h"
#include "file.h"
#include "game.h"
#include "image.h"
#include "model.h"
#include "parser.h"
#include "text_buffer.h"
#include "variable.h"


Directory<Anim>
Resourcex::m_animDirectory;

Directory<Image>
Resourcex::m_imageDirectory;

Directory<Model>
Resourcex::m_modelDirectory;


/*******************************************************************************
	Returns the anim. Loads it if necessary. Fatal error if not found.
*******************************************************************************/
Anim&
Resourcex::getAnim(
	const tstring& ksName)
{
	Anim* pAnim;

	// Find the anim.
	if (pAnim = m_animDirectory.find(ksName))
	{
		return *pAnim;
	}

	// Load the anim.
	return loadAnim(ksName);
}


/*******************************************************************************
	Returns the image. Loads it if necessary. Fatal error if not found.
*******************************************************************************/
Image&
Resourcex::getImage(
	const tstring& ksName)
{
	Image* pImage;

	// Find the image.
	if (pImage = m_imageDirectory.find(ksName))
	{
		return *pImage;
	}

	// Load the image.
	return loadImage(ksName);
}


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
	The anim name is a simple name, e.g. "world".
*******************************************************************************/
Anim&
Resourcex::loadAnim(
	const tstring& ksName)
{
	const tstring ksFileName =
		tstring(_T("data\\anims\\")) +
		ksName +
		tstring(_T(".txt"));

	FILE* pFile = File::openFile(ksFileName, _T("r"));

	Anim& anim = *new Anim();
	anim.setName(ksName);
	m_animDirectory.insert(ksName, anim);

	TRY
	{
		Parser parser(pFile);
		parser.parseAnim(anim);
	}
	CATCHALL
	{
		m_animDirectory.erase(ksName);
		delete &anim;
		RETHROW;
	}
	END_TRY_CATCHALL

	File::closeFile(pFile);

	return anim;
}


/*******************************************************************************
	The image name is a simple name, e.g. "world".
*******************************************************************************/
Image&
Resourcex::loadImage(
	const tstring& ksName)
{
	const tstring ksFileName =
		tstring(_T("data\\images\\")) +
		ksName +
		tstring(_T(".png"));

	Image& image = *new Image();
	image.setName(ksName);
	m_imageDirectory.insert(ksName, image);

	HRESULT result =
		image.getSurface().CreateSurface(
			0,
			File::makeFileName(ksFileName).c_str());
	if (result != GD_OK)
	{
		m_imageDirectory.erase(ksName);
		delete &image;
		// TODO Throw exception
	}

	return image;
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

	Model& model = *new Model();
	model.setName(ksName);
	m_modelDirectory.insert(ksName, model);

	TRY
	{
		Parser parser(pFile);
		parser.parseModel(model);
	}
	CATCHALL
	{
		m_modelDirectory.erase(ksName);
		delete &model;
		RETHROW;
	}
	END_TRY_CATCHALL

	File::closeFile(pFile);

	return model;
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
