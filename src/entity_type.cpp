// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "entity_type.h"


Directory<EntityType>
EntityType::m_entityTypeDirectory;


/*******************************************************************************
*******************************************************************************/
void
EntityType::init()
{
	{
		tstring sName(_T("block"));
		EntityType& entityType = *new EntityType();
		m_entityTypeDirectory.insert(sName, entityType);
		entityType.m_sName = sName;
		entityType.m_bSolid = true;
	}
	{
		tstring sName(_T("control"));
		EntityType& entityType = *new EntityType();
		m_entityTypeDirectory.insert(sName, entityType);
		entityType.m_sName = sName;
		entityType.m_bSolid = false;
	}
	{
		tstring sName(_T("path"));
		EntityType& entityType = *new EntityType();
		m_entityTypeDirectory.insert(sName, entityType);
		entityType.m_sName = sName;
		entityType.m_bSolid = true;
	}
	{
		tstring sName(_T("player"));
		EntityType& entityType = *new EntityType();
		m_entityTypeDirectory.insert(sName, entityType);
		entityType.m_sName = sName;
		entityType.m_bSolid = true;
	}
	{
		tstring sName(_T("monster"));
		EntityType& entityType = *new EntityType();
		m_entityTypeDirectory.insert(sName, entityType);
		entityType.m_sName = sName;
		entityType.m_bSolid = true;
	}
	{
		tstring sName(_T("world"));
		EntityType& entityType = *new EntityType();
		m_entityTypeDirectory.insert(sName, entityType);
		entityType.m_sName = sName;
		entityType.m_bSolid = true;
	}
}
