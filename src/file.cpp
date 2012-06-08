// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#include "file.h"

#include "exception.h"
#include "variable.h"


/*******************************************************************************
*******************************************************************************/
void
File::closeFile(
	FILE* const kpFile)
{
	fclose(kpFile);
}


/*******************************************************************************
	Returns the original file name if it was absolute. If it was relative, then
	the base directory is prepended.
*******************************************************************************/
tstring
File::makeFileName(
	const tstring& ksFileName)
{
	if (ksFileName[0] == _T('\\'))
	{
		return ksFileName;
	}

	tstring sResult = Variable::file_base_dir.getValue();
	sResult += ksFileName;
	return sResult;
}


/*******************************************************************************
*******************************************************************************/
FILE*
File::openFile(
	const tstring& ksName,
	const tstring& ksMode)
{
	FILE* const kpFile =
		_tfopen(makeFileName(ksName).c_str(), ksMode.c_str());

	if (!kpFile)
	{
		THROW(Exception::OpenFile);
	}

	return kpFile;
}
