// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__RESOURCEX
#define GUARD__RESOURCEX


#include "directory.h"
#include "tstring.h"


class CGapiSurface;
class Model;


/*******************************************************************************
*******************************************************************************/
class Resourcex
{

public:

	static
	Model&
	getModel(
		const tstring& ksName);

	static
	void
	init();

	static
	CGapiSurface&
	loadImage(
		const _TCHAR* const kpkName);

	static
	void
	loadLevel(
		const tstring& ksName);

	static
	Model&
	loadModel(
		const tstring& ksName);

	// Takes a file name like "file.ext" and looks in application dir.
	// TODO remove this in favour of file module
	static
	FILE*
	openFile(
		const _TCHAR* const kpkName,
		const _TCHAR* const kpkMode);

private:


private:

	static
	Directory<Model> m_modelDirectory;


};


#endif // GUARD
