// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__IMAGE
#define GUARD__IMAGE


#include "GapiDraw.h"
#include "matrix.h"
#include "tstring.h"


/*******************************************************************************
	Image.
*******************************************************************************/
class Image
{


public:

	const tstring&
	getName() const;

	CGapiSurface&
	getSurface() const;

	void
	setName(
		const tstring& ksName);


private:

	tstring m_sName;

	CGapiSurface m_surface;


};


/*******************************************************************************
*******************************************************************************/
inline
const tstring&
Image::getName() const
{
	return m_sName;
}


/*******************************************************************************
*******************************************************************************/
inline
CGapiSurface&
Image::getSurface() const
{
	// GapiDraw isn't const correct, so provide non-const access to surface.
	return const_cast<CGapiSurface&>(m_surface);
}


/*******************************************************************************
*******************************************************************************/
inline
void
Image::setName(
	const tstring& ksName)
{
	m_sName = ksName;
}


#endif // GUARD
