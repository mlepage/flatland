// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__ANIM
#define GUARD__ANIM


#include <vector>
#include "matrix.h"
#include "tstring.h"


class Image;


/*******************************************************************************
	Image animation.

	TODO delete image if owned
*******************************************************************************/
class Anim
{

public:

	class Frame;


public:

	void
	addFrame(
		const Frame& kFrame);

	const Frame&
	getFrame(
		const int knIndex) const;

	const Image&
	getImage() const;

	const tstring&
	getName() const;

	int
	getNumberOfFrames() const;

	const Vec2&
	getSize() const;

	void
	setImage(
		const Image& kImage);

	void
	setName(
		const tstring& ksName);

	void
	setSize(
		const Vec2& kvSize);


private:

	tstring m_sName;

	Vec2 m_vSize;

	// Owned if anonymous.
	const Image* m_pkImage;

	std::vector<Frame> m_cFrame;


};


/*******************************************************************************
*******************************************************************************/
class Anim::Frame
{

public:

	int m_nIndex;

	int m_nCount;


};


/*******************************************************************************
*******************************************************************************/
inline
void
Anim::addFrame(
	const Frame& kFrame)
{
	m_cFrame.push_back(kFrame);
}


/*******************************************************************************
*******************************************************************************/
inline
const Anim::Frame&
Anim::getFrame(
	const int knIndex) const
{
	return m_cFrame[knIndex];
}


/*******************************************************************************
*******************************************************************************/
inline
const Image&
Anim::getImage() const
{
	// TODO throw exception if null
	return *m_pkImage;
}


/*******************************************************************************
*******************************************************************************/
inline
const tstring&
Anim::getName() const
{
	return m_sName;
}


/*******************************************************************************
*******************************************************************************/
inline
int
Anim::getNumberOfFrames() const
{
	return m_cFrame.size();
}


/*******************************************************************************
*******************************************************************************/
inline
const Vec2&
Anim::getSize() const
{
	return m_vSize;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Anim::setImage(
	const Image& kImage)
{
	m_pkImage = &kImage;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Anim::setName(
	const tstring& ksName)
{
	m_sName = ksName;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Anim::setSize(
	const Vec2& kvSize)
{
	m_vSize = kvSize;
}


#endif // GUARD
