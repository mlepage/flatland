// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__TSTRING
#define GUARD__TSTRING


#include <string>
#include <tchar.h>


typedef std::basic_string<_TCHAR> tstring;


/*******************************************************************************
*******************************************************************************/
inline
int
toInt(
	const tstring& ks)
{
	int n;
	_stscanf(ks.c_str(), _T("%d"), &n);
	return n;
}


/*******************************************************************************
*******************************************************************************/
inline
tstring
toString(
	const int kn)
{
	_TCHAR buffer[16];
	_stprintf(buffer, _T("%d"), kn);
	return tstring(buffer);
}


#endif // GUARD
