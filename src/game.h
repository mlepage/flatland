// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__GAME
#define GUARD__GAME


#include <vector>
#include "entity.h"
#include "world.h"


/*******************************************************************************
*******************************************************************************/
class Game
{


public:

	static
	void
	destroyEntity(
		Entity& entity);

	static
	Entity*
	getActiveEntity();

	static
	int
	getChecksum();

	static
	Entity&
	getEntity(
		const int knIndex);

	static
	int
	getFrame();

	static
	int
	getNumberOfEntities();

	static
	bool
	isFast();

	static
	bool
	isPaused();

	static
	void
	makeTestLevel();

	static
	bool
	resolveCollision(
		const World::Collision& kCollision,
		Entity& entity1,                    // The entity that moved.
		Entity& entity2);                   // The entity it hit.

	static
	void
	runEntity(
		Entity& entity);

	static
	void
	runFrame();

	static
	void
	setActiveEntity(
		Entity* pEntity);

	static
	void
	setFast(
		const bool kbFast);

	static
	void
	setPaused(
		const bool kbPaused);

	static
	Entity&
	spawnEntity();


private:

	static
	void
	checkForEscapedEntities();

	static
	void
	checkIntegrity();

	static
	void
	checkInterpenetration();

	static
	void
	checkInterpenetrationSimple();

	static
	void
	debugSetEntityState(
		Entity& entity,
		const int knX,
		const int knY,
		const int knDeltaX,
		const int knDeltaY);

	static
	void
	makeTestLevel1();

	static
	void
	makeTestLevel2();

	static
	void
	makeTestLevel3();

	static
	void
	physics(
		Entity& entity);

	static
	bool m_bFast;

	static
	bool m_bPaused;

	static
	int m_nFrame;

	// The entities.
	static
	std::vector<Entity*> m_cpEntity;

	static
	Entity* m_pActiveEntity;


};


/*******************************************************************************
*******************************************************************************/
inline
Entity*
Game::getActiveEntity()
{
	return m_pActiveEntity;
}


/*******************************************************************************
*******************************************************************************/
inline
Entity&
Game::getEntity(
	const int knIndex)
{
	return *m_cpEntity[knIndex];
}


/*******************************************************************************
*******************************************************************************/
inline
int
Game::getFrame()
{
	return m_nFrame;
}


/*******************************************************************************
*******************************************************************************/
inline
int
Game::getNumberOfEntities()
{
	return m_cpEntity.size();
}


/*******************************************************************************
*******************************************************************************/
inline
bool
Game::isFast()
{
	return m_bFast;
}


/*******************************************************************************
*******************************************************************************/
inline
bool
Game::isPaused()
{
	return m_bPaused;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Game::setFast(
	const bool kbFast)
{
	m_bFast = kbFast;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Game::setPaused(
	const bool kbPaused)
{
	m_bPaused = kbPaused;
}


#endif // GUARD
