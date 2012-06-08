// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "game.h"

#include <algorithm>
#include <cmath>
#include "console.h"
#include "entity_type.h"
#include "menu.h"
#include "resourcex.h"
#include "screen.h"
#include "state_game.h"
#include "state_title.h"
#include "variable.h"
#include "world.h"


bool
Game::m_bFast = false;

bool
Game::m_bAutoscrollEnabled = true;

bool
Game::m_bPaused = false;

int
Game::m_nFrame = 0;

std::vector<Entity*>
Game::m_cpEntity;

Entity*
Game::m_pActiveEntity = 0;

Entity*
Game::m_pAutoscrollEntity = 0;


/*******************************************************************************
	Do a rough check of each mobile entity's position against the world bounds.
*******************************************************************************/
void
Game::checkForEscapedEntities()
{
	const Rect& krBounds = Game::getEntity(0).getBounds();

	for (int n = 1; n != Game::getNumberOfEntities(); ++n)
	{
		const Entity& kEntity = Game::getEntity(n);

		if (!kEntity.isMobile())
		{
			continue;
		}

		const Vec2& kvOrigin = kEntity.getOrigin();

		if (kvOrigin[0] < krBounds.getMin()[0] ||
			krBounds.getMax()[0] < kvOrigin[0] ||
			kvOrigin[1] < krBounds.getMin()[1] ||
			krBounds.getMax()[1] < kvOrigin[1])
		{
			// Entity has escaped the world.
			Console::print(
				_T("%d ESC %d"),
				Game::getFrame(),
				n);
			Screen::print(
				_T("%d ESC %d"),
				Game::getFrame(),
				n);
			Game::setPaused(true);
		}
	}
}


/*******************************************************************************
*******************************************************************************/
void
Game::checkIntegrity()
{
	checkForEscapedEntities();
	//checkInterpenetrationSimple();
}


/*******************************************************************************
	Brute force check. Every entity's vertex is checked to see if it is in
	any other entity's brushes.
*******************************************************************************/
void
Game::checkInterpenetration()
{
	for (int nThisEntityIndex = 0;
		nThisEntityIndex != Game::getNumberOfEntities() - 1;
		++nThisEntityIndex)
	{
		const Entity& kThisEntity = Game::getEntity(nThisEntityIndex);
		const Model& kThisModel = kThisEntity.getModel();

		for (int nOtherEntityIndex = nThisEntityIndex + 1;
			nOtherEntityIndex != Game::getNumberOfEntities();
			++nOtherEntityIndex)
		{
			const Entity& kOtherEntity = Game::getEntity(nOtherEntityIndex);
			const Model& kOtherModel = kOtherEntity.getModel();

			for (int nThisBrushIndex = 0;
				nThisBrushIndex != kThisModel.getNumberOfBrushes();
				++nThisBrushIndex)
			{
				const Brush& kThisBrush = kThisModel.getBrush(nThisBrushIndex);

				for (int nThisVertexIndex = 0;
					nThisVertexIndex != kThisBrush.getNumberOfVertices();
					++nThisVertexIndex)
				{
					if (World::isPointInEntity(
						Geometry::transformPoint(
							kThisEntity.getTransformM2W(),
							kThisBrush.getVertex(nThisVertexIndex)),
						kOtherEntity))
					{
						Console::print(
							_T("%d INTER %d %d"),
							Game::getFrame(),
							nThisEntityIndex,
							nOtherEntityIndex);
						Screen::print(
							_T("%d INTER %d %d"),
							Game::getFrame(),
							nThisEntityIndex,
							nOtherEntityIndex);
						Game::setPaused(true);
					}
				}
			}
		}
	}
}


/*******************************************************************************
	Checks only non-world entities to see if their position is too close. So
	this depends on the test world entities.
*******************************************************************************/
void
Game::checkInterpenetrationSimple()
{
	const scalar kfRadius = 8.5;
	const scalar kfDiameter = kfRadius + kfRadius;
	const scalar kfSquaredDistance = kfDiameter * kfDiameter;

	for (int nThisEntityIndex = 1;
		nThisEntityIndex != Game::getNumberOfEntities() - 1;
		++nThisEntityIndex)
	{
		const Entity& kThisEntity = Game::getEntity(nThisEntityIndex);

		for (int nOtherEntityIndex = nThisEntityIndex + 1;
			nOtherEntityIndex != Game::getNumberOfEntities();
			++nOtherEntityIndex)
		{
			const Entity& kOtherEntity = Game::getEntity(nOtherEntityIndex);

			const Vec2 kvRelativePosition =
				kOtherEntity.getOrigin() - kThisEntity.getOrigin();

			if (kvRelativePosition % kvRelativePosition < kfSquaredDistance)
			{
				Screen::print(
					_T("%d INTER %d %d"),
					Game::getFrame(),
					nThisEntityIndex,
					nOtherEntityIndex);
				Game::setPaused(true);
			}
		}
	}
}


/*******************************************************************************
*******************************************************************************/
void
Game::debugSetEntityState(
	Entity& entity,
	const int knX,
	const int knY,
	const int knDeltaX,
	const int knDeltaY)
{
	const scalar kfX = *reinterpret_cast<const scalar*>(&knX);
	const scalar kfY = *reinterpret_cast<const scalar*>(&knY);
	const scalar kfDeltaX = *reinterpret_cast<const scalar*>(&knDeltaX);
	const scalar kfDeltaY = *reinterpret_cast<const scalar*>(&knDeltaY);

	World::unlinkEntity(entity);
	entity.getOrigin() = Vec2(kfX, kfY);
	entity.getVelocity() = Vec2(kfDeltaX, kfDeltaY);
	World::linkEntity(entity);
}


/*******************************************************************************
	TODO Destroy entities. Right now this leaks. Also, I need to destroy
	anonymous models when I destroy their entity.
*******************************************************************************/
void
Game::destroyEntity(
	Entity& entity)
{
	World::unlinkEntity(entity);

	// TODO It is dangerous to change the vector while iterating through it
	// (e.g. destroying entities while processing them). Fix this.

	m_cpEntity.erase(std::find(m_cpEntity.begin(), m_cpEntity.end(), &entity));
}


/*******************************************************************************
*******************************************************************************/
void
Game::endGame()
{
	// TODO some of this should be elsewhere. Maybe startGame?
	// Destroy all remaining entities (reverse order).
	for (int n = Game::getNumberOfEntities() - 1; n != -1; --n)
	{
		Entity& entity = Game::getEntity(n);
		Game::destroyEntity(entity);
	}
	// Menu.
	Menu::setCurrentMenu(Menu::title_main);
	Menu::setCurrentItem(0);
	Game::setPaused(false);
	// Go back to title screen.
	StateTitle::changeState();
}


/*******************************************************************************
*******************************************************************************/
int
Game::getChecksum()
{
	int nChecksum = 0;

	// TODO consider shifting bits to avoid using only (mostly) low bits?

	for (int n = 0; n != Game::getNumberOfEntities(); ++n)
	{
		Entity& entity = Game::getEntity(n);

		nChecksum ^= *reinterpret_cast<const int*>(&entity.getOrigin()[0]);
		nChecksum ^= *reinterpret_cast<const int*>(&entity.getOrigin()[1]);
		nChecksum ^= *reinterpret_cast<const int*>(&entity.getVelocity()[0]);
		nChecksum ^= *reinterpret_cast<const int*>(&entity.getVelocity()[1]);
		const scalar kfAngle = entity.getAngle();
		nChecksum ^= *reinterpret_cast<const int*>(&kfAngle);
	}

	return nChecksum;
}


/*******************************************************************************
*******************************************************************************/
void
Game::newGame()
{
	StateGame::changeState();

	// Menu.
	Menu::clearCurrentMenu();
}


/*******************************************************************************
*******************************************************************************/
void
Game::physics(
	Entity& entity)
{
	// For debugging
	bool bDumpEntity = false;
	
	if (!entity.isMobile() ||
		entity.getVelocity() == Vec2(0, 0))
	{
		return;
	}

	scalar fTimeCompletedOld = -1;
	scalar fTimeCompleted = 0;
	scalar fTimeRemaining = 1;

	int nCollisionCount = 0;

	while ((int)fTimeRemaining != 0)
	{
		World::unlinkEntity(entity);

		// Calculate new end position.
		const Vec2 kvEnd =
			entity.getOrigin() + entity.getVelocity() * fTimeRemaining;

		// Trace entity to end position.
		World::Collision collision = World::traceEntity(entity, kvEnd);

		// Update time.
		const scalar kfCurrentTime = collision.m_fTime * fTimeRemaining;
		fTimeCompletedOld = fTimeCompleted;
		fTimeCompleted += kfCurrentTime;
		fTimeRemaining = 1 - fTimeCompleted;

		if (fTimeCompleted == fTimeCompletedOld)
		{
			// We aren't making any progress.
			Console::print(
				_T("Infinite: f=%d tc=%.3f t=%.3f ct=%.3f e1=%d\n"),
				Game::getFrame(),
				fTimeCompleted,
				collision.m_fTime,
				kfCurrentTime,
				entity.getIdentifier());
			++entity.m_nInfiniteStuckCount;
			if (1 < entity.m_nInfiniteStuckCount)
			{
				Console::print(
					_T("Too many infinite stuck: f=%d e1=%d stuck=%d\n"),
					Game::getFrame(),
					entity.getIdentifier(),
					entity.m_nInfiniteStuckCount);
				exit(1);
			}
			else
			{
				fTimeRemaining = 0;
			}
		}
		else
		{
			entity.m_nInfiniteStuckCount = 0;
		}

		// Move entity.
		entity.getOrigin() +=
			entity.getVelocity() * (kfCurrentTime * 0.9999); // FUDGE

		// Resolve collision.
		if (collision.m_fTime != 1)
		{
			Console::print(
				_T("Collision: f=%d tc=%.3f t=%.3f e1=%d e2=%d\n"),
				Game::getFrame(),
				fTimeCompletedOld,
				collision.m_fTime,
				entity.getIdentifier(),
				collision.m_pEntity->getIdentifier());

			++nCollisionCount;
			if (10 < nCollisionCount)
			{
				// Too many collisions. Just bail.
				fTimeRemaining = 0;
				continue;
			}

			// TEMP Treat path entities specially until I do them properly.
			if (entity.getType().getName() == tstring(_T("path")))
			{
				// Just stop moving.
				++entity.m_nPathStuckCount;
				fTimeRemaining = 0;
				// Bump the other entity by your velocity.
				if (collision.m_pEntity->getType().getName() ==
					tstring(_T("player")))
				{
					scalar fOtherEntitySpeed =
						sqrt(collision.m_pEntity->getVelocity() %
							collision.m_pEntity->getVelocity());
					collision.m_pEntity->getVelocity() +=
						entity.getVelocity();
				}
			}
			else if (collision.m_pEntity->isMobile() &&
				collision.m_pEntity->getType().getName() == tstring(_T("monster")))
			{
				const bool bStuck =
					resolveCollision(collision, entity, *collision.m_pEntity);
				if (bStuck)
				{
					Console::print(
						_T("Response: e1=%d stuck!\n"),
						entity.getIdentifier());
					entity.getOrigin() -=
						entity.getVelocity() * kfCurrentTime;
					fTimeRemaining = 0;
				}
				// Path stuff.
				entity.m_nPathStuckCount = 0;
			}
			else
			{
				// Set new velocity.
				// PERF lots to optimize here.
				// This isn't really velocity, it's negated.
				const Vec2 kvNormalizedVelocity =
					-entity.getVelocity() /
						sqrt(entity.getVelocity() % entity.getVelocity());
				entity.getVelocity() =
					(2 * collision.m_vNormal *
					(collision.m_vNormal % kvNormalizedVelocity) -
					kvNormalizedVelocity) *
					sqrt(entity.getVelocity() % entity.getVelocity());
			}

			Console::print(
				_T("Response: e1=%d v=<%.3f,%.3f>\n"),
				entity.getIdentifier(),
				entity.getVelocity()[0],
				entity.getVelocity()[1]);

			if (collision.m_pEntity->getIdentifier() != 0)
			{
				Console::print(
					_T("Response: e2=%d v=<%.3f,%.3f>\n"),
					collision.m_pEntity->getIdentifier(),
					collision.m_pEntity->getVelocity()[0],
					collision.m_pEntity->getVelocity()[1]);
			}

			// If this entity was active, deactivate it.
			if (getActiveEntity() == &entity)
			{
				setActiveEntity(0);
			}

			// If the other entity's type was block, destroy it.
			if (collision.m_pEntity->getType().getName() ==
				tstring(_T("block")) &&
				entity.getType().getName() == tstring(_T("player")))
			{
				Console::print(
					_T("Response: e2=%d destroyed\n"),
					entity.getIdentifier());
				Game::destroyEntity(*collision.m_pEntity);
			}
		}

		World::linkEntity(entity);
	}

	World::linkEntity(entity);
}


/*******************************************************************************
	Returns whether the entity is stuck.
*******************************************************************************/
bool
Game::resolveCollision(
	const World::Collision& kCollision,
	Entity& entity1,
	Entity& entity2)
{
	const scalar kfCoefficientOfRestitution = 1;

	const Vec2 kvRelativeVelocity =
		entity1.getVelocity() - entity2.getVelocity();

	// PERF Since the normal is normalized, we can simplify this, by removing
	// the normal % normal from the denominator. TODO do this and measure.

	// The impulse is the normal scaled by this value.
	const scalar kfImpulseNumerator =
		-(1 + kfCoefficientOfRestitution) *
		(kvRelativeVelocity % kCollision.m_vNormal);
	const scalar kfImpulseDenominator =
		(kCollision.m_vNormal % kCollision.m_vNormal) *
		(1 / entity1.getMass() + 1 / entity2.getMass());
	const scalar kfImpulse = kfImpulseNumerator / kfImpulseDenominator;

	if (kfImpulse < 0)
	{
		return true;
	}
	else
	{
		entity1.getVelocity() +=
			(kfImpulse / entity1.getMass()) * kCollision.m_vNormal;
		entity2.getVelocity() -=
			(kfImpulse / entity2.getMass()) * kCollision.m_vNormal;
		return false;
	}
}

											
/*******************************************************************************
*******************************************************************************/
void
Game::runEntity(
	Entity& entity)
{
	// Hack here to limit velocity of thrown entity. Also hack in friction.
	{
		if (&entity != Game::getActiveEntity() &&
			entity.getType().getName() == tstring(_T("player")))
		{
			const scalar kfSpeedLimit =
				Variable::physics_speed_limit.getFloatValue();
			scalar fSpeed =
				sqrt(entity.getVelocity() % entity.getVelocity());
			if (kfSpeedLimit < fSpeed)
			{
				entity.getVelocity() *= kfSpeedLimit / fSpeed;
			}
			// Friction.
			entity.getVelocity() *= Variable::physics_friction.getFloatValue();
			scalar fNewSpeed =
				sqrt(entity.getVelocity() % entity.getVelocity());
			if (fNewSpeed < 0.1)
			{
				entity.getVelocity() = Vec2(0, 0);
			}
		}
	}

	// Hack here to add simple path support.
	if (entity.getType().getName() == tstring(_T("path")))
	{
		Vec2 vCurrentPoint =
			entity.m_cvPathPoint[entity.m_nPathCurrent];
		const scalar kfDistanceToCurrentPoint =
			sqrt((vCurrentPoint - entity.getOrigin()) %
				(vCurrentPoint - entity.getOrigin()));
		if (kfDistanceToCurrentPoint <= entity.m_fPathSpeed ||
			1 < entity.m_nPathStuckCount)
		{
			// We're there, go to next path point.
			++entity.m_nPathCurrent;
			if (entity.m_cvPathPoint.size() <= entity.m_nPathCurrent)
			{
				entity.m_nPathCurrent = 0;
			}
			vCurrentPoint =
				entity.m_cvPathPoint[entity.m_nPathCurrent];
			entity.m_nPathStuckCount = 0;
		}
		Vec2 vVectorToPoint =
			vCurrentPoint - entity.getOrigin();
		scalar fDistanceToPoint =
			sqrt(vVectorToPoint % vVectorToPoint);
		entity.getVelocity() =
			vVectorToPoint / fDistanceToPoint * entity.m_fPathSpeed;
	}

	// Hack here to add simple force support.
	if (entity.getType().getName() == tstring(_T("force")))
	{
		// Get player entity
		// See if it isn't controlled
		// See if it is on force entity
		// Add force vector to it

		// TODO need API for doing this quickly
		Entity* pPlayerEntity = 0;
		for (int n = 0; n != Game::getNumberOfEntities(); ++n)
		{
			Entity& testEntity = Game::getEntity(n);
			if (testEntity.getType().getName() == tstring(_T("player")))
			{
				pPlayerEntity = &testEntity;
				break;
			}
		}

		// Assume we found a player entity, of course, and that there is only
		// one.
		// This uses seek and arrival steering behaviour.
		// TODO check physics book for proper force application.
		// TODO this needs a poly-to-poly intersection test
		// TODO currently it uses the player's origin which may not be in the
		//      model
		if (!Game::getActiveEntity() &&
			pPlayerEntity->m_nForceFrame + 30 <
			Application::getActualFrameNumber() &&
			World::isPointInEntity(
				pPlayerEntity->getOrigin(),
				entity))
		{
			Vec2 vTargetOffset =
				entity.m_vForcePoint - pPlayerEntity->getOrigin();
			scalar fDistance =
				sqrt(vTargetOffset % vTargetOffset);
			scalar fRampedSpeed =
				entity.m_fForceSpeed *
				(fDistance / (entity.m_fForceSpeed * 10));
			scalar fClippedSpeed =
				fRampedSpeed < entity.m_fForceSpeed ?
					fRampedSpeed :
					entity.m_fForceSpeed;
			// This is the "desired velocity"
			Vec2 vDesiredVelocity =
				fDistance != 0 ?
					(fClippedSpeed / fDistance) *
					vTargetOffset :
				Vec2(0, 0);
			Vec2 vSteering =
				vDesiredVelocity - pPlayerEntity->getVelocity();
			if (fDistance < entity.m_fForceSpeed)
			{
				pPlayerEntity->getVelocity() = Vec2(0, 0);
			}
			else
			{
				pPlayerEntity->getVelocity() += vSteering;
			}
		}
	}

	physics(entity);

	// TODO runThink
}


/*******************************************************************************
*******************************************************************************/
void
Game::runFrame()
{
	for (int n = 0; n != getNumberOfEntities(); ++n)
	{
		runEntity(getEntity(n));
	}

	++m_nFrame;

	Screen::print(
		_T("%d %p"),
		Game::getFrame(),
		Game::getChecksum());
	checkIntegrity();
}


/*******************************************************************************
*******************************************************************************/
Entity&
Game::spawnEntity()
{
	Entity* pEntity = new Entity();

	pEntity->setIdentifier(m_cpEntity.size());
	m_cpEntity.push_back(pEntity);

	return *pEntity;
}
