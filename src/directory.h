// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__DIRECTORY
#define GUARD__DIRECTORY


#include <map>
#include "tstring.h"


/*******************************************************************************
	A simple name to whatever map.

	Not intended to hold null values. Returns null if not found. Stores values
	by pointer.
*******************************************************************************/
template <typename VALUE>
class Directory
{


public:

	void
	erase(
		const tstring& ksName);

	VALUE*
	find(
		const tstring& ksName);

	void
	insert(
		const tstring& ksName,
		VALUE& value);


private:

	typedef std::map<tstring, VALUE*> MapNameToValue;


private:

	MapNameToValue m_mapNameToValue;


};


/*******************************************************************************
*******************************************************************************/
template <typename VALUE>
inline
void
Directory<VALUE>::erase(
	const tstring& ksName)
{
	m_mapNameToValue.erase(ksName);
}


/*******************************************************************************
*******************************************************************************/
template <typename VALUE>
inline
VALUE*
Directory<VALUE>::find(
	const tstring& ksName)
{
	MapNameToValue::iterator i =
		m_mapNameToValue.find(ksName);

	if (i != m_mapNameToValue.end())
	{
		// Found.
		return i->second;
	}

	// Not found.
	return NULL;
}


/*******************************************************************************
*******************************************************************************/
template <typename VALUE>
inline
void
Directory<VALUE>::insert(
	const tstring& ksName,
	VALUE& value)
{
	m_mapNameToValue[ksName] = &value;
}


#endif // GUARD
