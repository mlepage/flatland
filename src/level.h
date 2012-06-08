// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__LEVEL
#define GUARD__LEVEL


#include <vector>


class Entity;


/*******************************************************************************
*******************************************************************************/
class Level
{


public:

	~Level();

	void
	addEntity(
		Entity& entity)
	{
		m_cpEntity.push_back(&entity);
	}

	Entity&
	getEntity(
		const int knIndex) const
	{
		return *m_cpEntity[knIndex];
	}

	int
	getNumberOfEntities() const
	{
		return m_cpEntity.size();
	}

private:

	// The level owns the entities and they are deleted upon destruction.
	std::vector<Entity*> m_cpEntity;


};


#endif // GUARD
