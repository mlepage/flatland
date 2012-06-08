// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__MODEL
#define GUARD__MODEL


#include <vector>
#include "brush.h"
#include "config.h"
#include "geometry.h"
#include "rect.h"
#include "tstring.h"


class CGapiSurface;


/*******************************************************************************
	The model has its own coordinate system, with (0, 0) as the origin.
*******************************************************************************/
class Model
{


public:

	Model();

	void
	addBrush(
		const Brush& kBrush);

	const Rect&
	getBounds() const;

	const Brush&
	getBrush(
		const int knIndex) const;

	const CGapiSurface&
	getImage() const;

	const tstring&
	getName() const;

	int
	getNumberOfBrushes() const;

	void
	setImage(
		const CGapiSurface& kImage);

	void
	setName(
		const tstring& ksName);


private:

	// The name.
	tstring m_sName;

	// The image.
	const CGapiSurface* m_pkImage;

	// The bounding box.
	Rect m_rBounds;

	// The brushes.
	std::vector<Brush> m_cBrush;


};


/*******************************************************************************
*******************************************************************************/
inline
Model::Model() :
	m_pkImage(0)
{
}


/*******************************************************************************
*******************************************************************************/
inline
void
Model::addBrush(
	const Brush& kBrush)
{
	// Update the bounding box.
	if (m_cBrush.empty())
	{
		m_rBounds = kBrush.getBounds();
	}
	else
	{
		Geometry::addRectToBounds(
			kBrush.getBounds(),
			m_rBounds);
	}

	m_cBrush.push_back(kBrush);
}
	
	
/*******************************************************************************
*******************************************************************************/
inline
const Rect&
Model::getBounds() const
{
	return m_rBounds;
}


/*******************************************************************************
*******************************************************************************/
inline
const Brush&
Model::getBrush(
	const int knIndex) const
{
	return m_cBrush[knIndex];
}


/*******************************************************************************
*******************************************************************************/
inline
const CGapiSurface&
Model::getImage() const
{
	return *m_pkImage;
}


/*******************************************************************************
*******************************************************************************/
inline
const tstring&
Model::getName() const
{
	return m_sName;
}


/*******************************************************************************
*******************************************************************************/
inline
int
Model::getNumberOfBrushes() const
{
	return m_cBrush.size();
}


/*******************************************************************************
*******************************************************************************/
inline
void
Model::setImage(
	const CGapiSurface& kImage)
{
	m_pkImage = &kImage;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Model::setName(
	const tstring& ksName)
{
	m_sName = ksName;
}


#endif // GUARD
