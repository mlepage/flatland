// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "level.h"

#include "entity.h"


/*******************************************************************************
*******************************************************************************/
Level::~Level()
{
	for (int n = m_cpEntity.size(); n != -1; --n)
	{
		delete m_cpEntity[n];
	}
}
