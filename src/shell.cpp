// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#include "shell.h"

#include <cstdio>
#include "console.h" // testing
#include "variable.h"


/*******************************************************************************
*******************************************************************************/
void
Shell::doCommand(
	const _TCHAR* const kpkt)
{
	// TODO Braindead implementation, for now only support set command.

	_TCHAR anName[128];
	_TCHAR anValue[128];

	if (_stscanf(
		kpkt,
		_T(" # %s "),
		anName) == 1)
	{
		Console::print(_T("shell: comment: %s\n"), anName);
	}
	else if (_stscanf(
		kpkt,
		_T(" set %128s = %128s "),
		anName,
		anValue) == 2)
	{
		Console::print(_T("shell: setting variable: %s %s\n"), anName, anValue);
		Variable* pVariable = Variable::find(anName);
		if (pVariable)
		{
			pVariable->setValue(anValue);
		}
	}
}
