// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__ERROR
#define GUARD__ERROR


#include <tchar.h>


/*******************************************************************************
	Error handling.
*******************************************************************************/
class Error
{


public:

	static
	void
	fatalError(
		const _TCHAR* kpFormat,
		...);


};


#endif // GUARD
