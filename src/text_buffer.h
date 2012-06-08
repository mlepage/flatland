// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__TEXT_BUFFER
#define GUARD__TEXT_BUFFER


#include <tchar.h>


/*******************************************************************************
	A generic text buffer. Uses Microsoft's <tchar.h> facilities.

	It contains a trailing null termination character not counting in size.

	When using sprintf, use size, and you will be safely terminated whether or
	not sprintf terminates.

	When using gets, you can use size+1, since gets will terminate it safely.
*******************************************************************************/
template <int SIZE>
class TextBuffer
{


public:

	TextBuffer();

	_TCHAR*
	getBuffer();

	int
	getSize() const;

private:

	_TCHAR m_anBuffer[SIZE + 1];

};


/*******************************************************************************
*******************************************************************************/
template <int SIZE>
inline
TextBuffer<SIZE>::TextBuffer()
{
	// Terminate.
	m_anBuffer[SIZE] = _T('\0');
}


/*******************************************************************************
*******************************************************************************/
template <int SIZE>
inline
_TCHAR*
TextBuffer<SIZE>::getBuffer()
{
	return m_anBuffer;
}


/*******************************************************************************
*******************************************************************************/
template <int SIZE>
inline
int
TextBuffer<SIZE>::getSize() const
{
	return SIZE;
}


#endif // GUARD
