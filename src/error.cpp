// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


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
