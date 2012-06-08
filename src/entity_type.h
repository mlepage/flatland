// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__ENTITY_TYPE
#define GUARD__ENTITY_TYPE


#include "directory.h"
#include "exception.h"
#include "tstring.h"


/*******************************************************************************
	Let's say we start out with four entity types:

	world - The world.
	block - Blocks to be hit and disappear.
	player - Moveable player objects.
	control - Nonsolid areas where player can control objects.
*******************************************************************************/
class EntityType
{

public:

	static
	EntityType&
	find(
		const tstring& ksName);

	static
	void
	init();


public:

	const tstring&
	getName() const;

	bool
	isSolid() const;


private:

	static
	Directory<EntityType> m_entityTypeDirectory;


private:

	// The name.
	tstring m_sName;

	bool m_bSolid;

};


/*******************************************************************************
	Throws EntityTypeNotFound if not found.
*******************************************************************************/
inline
EntityType&
EntityType::find(
	const tstring& ksName)
{
	EntityType* const kp = m_entityTypeDirectory.find(ksName);

	if (!kp)
	{
		THROW(Exception::EntityTypeNotFound);
	}

	return *kp;
}


/*******************************************************************************
*******************************************************************************/
inline
const tstring&
EntityType::getName() const
{
	return m_sName;
}


/*******************************************************************************
*******************************************************************************/
inline
bool
EntityType::isSolid() const
{
	return m_bSolid;
}


#endif // GUARD
