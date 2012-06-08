// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__SHELL
#define GUARD__SHELL


#include <tchar.h>


/*******************************************************************************
	Shell interpreter.
*******************************************************************************/
class Shell
{

public:

	// A single command, no semicolons, no newlines.
	static
	void
	doCommand(
		const _TCHAR* const kpkt);


private:


};


#endif // GUARD
