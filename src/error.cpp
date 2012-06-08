// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "error.h"

#include <cstdlib>


/*******************************************************************************
*******************************************************************************/
void
Error::fatalError(
	const _TCHAR* kpFormat,
	...)
{
	// TODO Do more than just exit. Logging, error recovery, etc.
	exit(1);
}
