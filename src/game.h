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
	void
	endGame();

	static
	Entity*
	getActiveEntity();

	static
	Entity*
	getAutoscrollEntity();

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
	isAutoscrollEnabled();

	static
	bool
	isFast();

	static
	bool
	isPaused();

	static
	void
	newGame();

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
	setAutoscrollEnabled(
		const bool kbAutoscrollEnabled);

	static
	void
	setAutoscrollEntity(
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
	physics(
		Entity& entity);

	static
	bool m_bFast;

	static
	bool m_bAutoscrollEnabled;

	static
	bool m_bPaused;

	static
	int m_nFrame;

	// The entities.
	static
	std::vector<Entity*> m_cpEntity;

	static
	Entity* m_pActiveEntity;

	static
	Entity* m_pAutoscrollEntity;


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
Entity*
Game::getAutoscrollEntity()
{
	return m_pAutoscrollEntity;
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
Game::isAutoscrollEnabled()
{
	return m_bAutoscrollEnabled;
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
Game::setActiveEntity(
	Entity* pEntity)
{
	m_pActiveEntity = pEntity;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Game::setAutoscrollEnabled(
	const bool kbAutoscrollEnabled)
{
	m_bAutoscrollEnabled = kbAutoscrollEnabled;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Game::setAutoscrollEntity(
	Entity* pEntity)
{
	m_pAutoscrollEntity = pEntity;
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
