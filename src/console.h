// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__CONSOLE
#define GUARD__CONSOLE


#include <tchar.h>


/*******************************************************************************
	Eventally this will be a Quake-style console. For now, it simply logs to a
	file.
*******************************************************************************/
class Console
{

public:

	static
	int
	getDisplayLineIndex();

	static
	const _TCHAR*
	getLine(
		const int knLineIndex);

	static
	void
	init();

	static
	int
	print(
		const _TCHAR* kpFormat,
		...);


private:


};


#endif // GUARD
