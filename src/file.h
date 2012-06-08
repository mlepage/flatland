// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__FILE
#define GUARD__FILE


#include <cstdio>
#include "tstring.h"


/*******************************************************************************
	File system layer.
*******************************************************************************/
class File
{

public:

	static
	void
	closeFile(
		FILE* const kpFile);

	static
	tstring
	makeFileName(
		const tstring& ksFileName);

	static
	FILE*
	openFile(
		const tstring& ksName,
		const tstring& ksMode);

	static
	FILE*
	openFileNoException(
		const tstring& ksName,
		const tstring& ksMode);


};


#endif // GUARD
