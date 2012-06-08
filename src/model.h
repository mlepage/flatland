// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__MODEL
#define GUARD__MODEL


#include <vector>
#include "brush.h"
#include "geometry.h"
#include "rect.h"


/*******************************************************************************
	The model has its own coordinate system, with (0, 0) as the origin.
*******************************************************************************/
class Model
{


public:

	void
	addBrush(
		const Brush& kBrush);

	const Rect&
	getBounds() const;

	const Brush&
	getBrush(
		const int knIndex) const;

	int
	getNumberOfBrushes() const;


private:

	// The bounding box.
	Rect m_rBounds;

	// The brushes.
	std::vector<Brush> m_cBrush;


};


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
int
Model::getNumberOfBrushes() const
{
	return m_cBrush.size();
}


#endif // GUARD
