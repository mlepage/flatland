// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__RESOURCEX
#define GUARD__RESOURCEX


#include "directory.h"
#include "tstring.h"


class Anim;
class CGapiSurface;
class Image;
class Model;


/*******************************************************************************
*******************************************************************************/
class Resourcex
{

public:

	static
	Anim&
	getAnim(
		const tstring& ksName);

	static
	Image&
	getImage(
		const tstring& ksName);

	static
	Model&
	getModel(
		const tstring& ksName);

	static
	void
	init();

	static
	void
	loadLevel(
		const tstring& ksName);

	// Takes a file name like "file.ext" and looks in application dir.
	// TODO remove this in favour of file module
	static
	FILE*
	openFile(
		const _TCHAR* const kpkName,
		const _TCHAR* const kpkMode);

private:

	static
	Anim&
	loadAnim(
		const tstring& ksName);

	static
	Image&
	loadImage(
		const tstring& ksName);

	static
	Model&
	loadModel(
		const tstring& ksName);


private:

	static
	Directory<Anim> m_animDirectory;

	static
	Directory<Image> m_imageDirectory;

	static
	Directory<Model> m_modelDirectory;


};


#endif // GUARD
