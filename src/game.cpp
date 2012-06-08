// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#include "game.h"

#include <cmath>
#include "console.h"
#include "screen.h"
#include "world.h"


bool
Game::m_bFast = false;

bool
Game::m_bPaused = true;

int
Game::m_nFrame = 0;

std::vector<Entity*>
Game::m_cpEntity;

Entity*
Game::m_pActiveEntity = 0;


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
	checkInterpenetrationSimple();
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
Game::makeTestWorld()
{
	//makeTestWorld1();
	makeTestWorld2();
}


/*******************************************************************************
	The test world is 500x600, which is slightly wider and shorter than 4x4
	screens.

	The test world consists of 5 brushes: 1 in the center and 4 on the edges.
*******************************************************************************/
void
Game::makeTestWorld1()
{
	Model* pWorldModel = new Model();
	{
		// Center.
		Brush brush;
		brush.addVertex(
			Vec2(0, 50));
		brush.addVertex(
			Vec2(-50, 0));
		brush.addVertex(
			Vec2(0, -50));
		brush.addVertex(
			Vec2(50, 0));
		pWorldModel->addBrush(brush);
	}
	{
		// Northeast.
		Brush brush;
		brush.addVertex(
			Vec2(250, 300));
		brush.addVertex(
			Vec2(50, 300));
		brush.addVertex(
			Vec2(250, 50));
		pWorldModel->addBrush(brush);
	}
	{
		// Northwest.
		Brush brush;
		brush.addVertex(
			Vec2(-250, 300));
		brush.addVertex(
			Vec2(-250, 50));
		brush.addVertex(
			Vec2(50, 300));
		pWorldModel->addBrush(brush);
	}
	{
		// Southwest.
		Brush brush;
		brush.addVertex(
			Vec2(-250, -300));
		brush.addVertex(
			Vec2(50, -300));
		brush.addVertex(
			Vec2(-250, 50));
		pWorldModel->addBrush(brush);
	}
	{
		// Southeast.
		Brush brush;
		brush.addVertex(
			Vec2(250, -300));
		brush.addVertex(
			Vec2(250, 50));
		brush.addVertex(
			Vec2(50, -300));
		pWorldModel->addBrush(brush);
	}

	Entity* pWorldEntity = &Game::spawnEntity();
	pWorldEntity->setModel(pWorldModel);
	pWorldEntity->getColour() = Vec3(127, 127, 127);
	// don't link world
	// but still need to set its bounds?
	pWorldEntity->setBounds(pWorldModel->getBounds());

	Model* pShipModel = new Model();
	{
		// Right.
		Brush brush;
		brush.addVertex(Vec2(5, -10));
		brush.addVertex(Vec2(15, 0));
		brush.addVertex(Vec2(5, 20));
		pShipModel->addBrush(brush);
	}
	{
		// Left.
		Brush brush;
		brush.addVertex(Vec2(-5, 20));
		brush.addVertex(Vec2(-15, 0));
		brush.addVertex(Vec2(-5, -10));
		pShipModel->addBrush(brush);
	}

	// This ship moves.
	Entity* pShipEntity = &Game::spawnEntity();
	pShipEntity->setModel(pShipModel);
	pShipEntity->setMobile(true);
	pShipEntity->getColour() = Vec3(0, 255, 255);
	pShipEntity->getOrigin() = Vec2(0, 225);
	//pShipEntity->getVelocity() = Vec2(0, 20);
	pShipEntity->setAngle(0);
	World::linkEntity(*pShipEntity);

	pShipEntity = &Game::spawnEntity();
	pShipEntity->setMobile(true);
	pShipEntity->setModel(pShipModel);
	pShipEntity->getColour() = Vec3(0, 255, 0);
	pShipEntity->getOrigin() = Vec2(50, 50) + Vec2(30, 30);
	pShipEntity->getVelocity() = Vec2(5, 5);
	pShipEntity->setAngle(-45);
	World::linkEntity(*pShipEntity);

	pShipEntity = &Game::spawnEntity();
	pShipEntity->setModel(pShipModel);
	pShipEntity->setMobile(true);
	pShipEntity->getColour() = Vec3(255, 0, 0);
	pShipEntity->getOrigin() = Vec2(-50, 50);
	pShipEntity->getVelocity() = Vec2(0, 0);
	pShipEntity->setAngle(45);
	World::linkEntity(*pShipEntity);

	pShipEntity = &Game::spawnEntity();
	pShipEntity->setModel(pShipModel);
	pShipEntity->setMobile(true);
	pShipEntity->getColour() = Vec3(0, 0, 255);
	pShipEntity->getOrigin() = Vec2(-50, -50);
	pShipEntity->getVelocity() = Vec2(0, 0);
	pShipEntity->setAngle(45 + 90);
	World::linkEntity(*pShipEntity);

	pShipEntity = &Game::spawnEntity();
	pShipEntity->setModel(pShipModel);
	pShipEntity->setMobile(true);
	pShipEntity->getColour() = Vec3(255, 255, 0);
	pShipEntity->getOrigin() = Vec2(50, -50);
	pShipEntity->getVelocity() = Vec2(0, 0);
	pShipEntity->setAngle(-45 - 90);
	World::linkEntity(*pShipEntity);

	Model* pCircleModel = new Model();
	{
		Brush brush;
		brush.addVertex(Vec2(0, 10));
		brush.addVertex(Vec2(-7, 7));
		brush.addVertex(Vec2(-10, 0));
		brush.addVertex(Vec2(-7, -7));
		brush.addVertex(Vec2(0, -10));
		brush.addVertex(Vec2(7, -7));
		brush.addVertex(Vec2(10, 0));
		brush.addVertex(Vec2(7, 7));
		pCircleModel->addBrush(brush);
	}

	Entity* pCircleEntity = &Game::spawnEntity();
	pCircleEntity->setModel(pCircleModel);
	pCircleEntity->setMobile(true);
	pCircleEntity->getColour() = Vec3(255, 0, 255);
	pCircleEntity->getOrigin() = Vec2(100, 100);
	World::linkEntity(*pCircleEntity);

	pCircleEntity = &Game::spawnEntity();
	pCircleEntity->setModel(pCircleModel);
	pCircleEntity->setMobile(true);
	pCircleEntity->getColour() = Vec3(255, 0, 255);
	pCircleEntity->getOrigin() = Vec2(-100, 100);
	World::linkEntity(*pCircleEntity);

	pCircleEntity = &Game::spawnEntity();
	pCircleEntity->setModel(pCircleModel);
	pCircleEntity->setMobile(true);
	pCircleEntity->getColour() = Vec3(255, 0, 255);
	pCircleEntity->getOrigin() = Vec2(-100, -100);
	World::linkEntity(*pCircleEntity);

	pCircleEntity = &Game::spawnEntity();
	pCircleEntity->setModel(pCircleModel);
	pCircleEntity->setMobile(true);
	pCircleEntity->getColour() = Vec3(255, 0, 255);
	pCircleEntity->getOrigin() = Vec2(100, -100);
	World::linkEntity(*pCircleEntity);
}


/*******************************************************************************
	This world consists of adjacent octagonal rooms with passages joining them.
	Each room contains four entities.
*******************************************************************************/
void
Game::makeTestWorld2()
{
	// World parameters.
	const int knRoomWidth = 240;
	const int knRoomHeight = 320;
	const int knHorizontalPassageWidth = 20;
	const int knHorizontalPassageHeight = 100;
	const int knVerticalPassageWidth = 100;
	const int knVerticalPassageHeight = 20;
	const scalar kfEntityRadius = 10;

	// Coordinates of interest.
	const int knX0 = 0;
	const int knX1 = knHorizontalPassageWidth;
	const int knX2 = (knRoomWidth / 2) - (knVerticalPassageWidth / 2);
	const int knX3 = (knRoomWidth / 2) + (knVerticalPassageWidth / 2);
	const int knX4 = knRoomWidth - knHorizontalPassageWidth;
	const int knX5 = knRoomWidth;
	const int knY0 = 0;
	const int knY1 = knVerticalPassageHeight;
	const int knY2 = (knRoomHeight / 2) - (knHorizontalPassageHeight / 2);
	const int knY3 = (knRoomHeight / 2) + (knHorizontalPassageHeight / 2);
	const int knY4 = knRoomHeight - knVerticalPassageHeight;
	const int knY5 = knRoomHeight;

	Model* pEntityModel = new Model();
	{
		const int kfDiagonal = kfEntityRadius * sqrt(2) / 2;
		Brush brush;
		brush.addVertex(Vec2(0, kfEntityRadius));
		brush.addVertex(Vec2(-kfDiagonal, kfDiagonal));
		brush.addVertex(Vec2(-kfEntityRadius, 0));
		brush.addVertex(Vec2(-kfDiagonal, -kfDiagonal));
		brush.addVertex(Vec2(0, -kfEntityRadius));
		brush.addVertex(Vec2(kfDiagonal, -kfDiagonal));
		brush.addVertex(Vec2(kfEntityRadius, 0));
		brush.addVertex(Vec2(kfDiagonal, kfDiagonal));
		pEntityModel->addBrush(brush);
	}

	// PERF The first edge tested is the edge BEFORE the first vertex. So we
	// can try to order the edges to present the most likely candidate for
	// collision first.
	// This seems to gain only about 4% on both PC and PPC.
#define REORDER_EDGES 1

	// Room brushes. These are copied and translated to create rooms. Some are
	// omitted to create passages between rooms.
	std::vector<Brush> cRoomBrush;
	{
		// SW corner.
		Brush brush;
#if REORDER_EDGES
		brush.addVertex(Vec2(knX1, knY2));
		brush.addVertex(Vec2(knX0, knY2));
		brush.addVertex(Vec2(knX0, knY0));
		brush.addVertex(Vec2(knX2, knY0));
		brush.addVertex(Vec2(knX2, knY1));
#else
		brush.addVertex(Vec2(knX0, knY0));
		brush.addVertex(Vec2(knX2, knY0));
		brush.addVertex(Vec2(knX2, knY1));
		brush.addVertex(Vec2(knX1, knY2));
		brush.addVertex(Vec2(knX0, knY2));
#endif
		cRoomBrush.push_back(brush);
	}
	{
		// S passage.
		Brush brush;
#if REORDER_EDGES
		brush.addVertex(Vec2(knX2, knY1));
		brush.addVertex(Vec2(knX2, knY0));
		brush.addVertex(Vec2(knX3, knY0));
		brush.addVertex(Vec2(knX3, knY1));
#else
		brush.addVertex(Vec2(knX2, knY0));
		brush.addVertex(Vec2(knX3, knY0));
		brush.addVertex(Vec2(knX3, knY1));
		brush.addVertex(Vec2(knX2, knY1));
#endif
		cRoomBrush.push_back(brush);
	}
	{
		// SE corner.
		Brush brush;
#if REORDER_EDGES
		brush.addVertex(Vec2(knX3, knY1));
		brush.addVertex(Vec2(knX3, knY0));
		brush.addVertex(Vec2(knX5, knY0));
		brush.addVertex(Vec2(knX5, knY2));
		brush.addVertex(Vec2(knX4, knY2));
#else
		brush.addVertex(Vec2(knX5, knY0));
		brush.addVertex(Vec2(knX5, knY2));
		brush.addVertex(Vec2(knX4, knY2));
		brush.addVertex(Vec2(knX3, knY1));
		brush.addVertex(Vec2(knX3, knY0));
#endif
		cRoomBrush.push_back(brush);
	}
	{
		// E passage.
		Brush brush;
#if REORDER_EDGES
		brush.addVertex(Vec2(knX4, knY2));
		brush.addVertex(Vec2(knX5, knY2));
		brush.addVertex(Vec2(knX5, knY3));
		brush.addVertex(Vec2(knX4, knY3));
#else
		brush.addVertex(Vec2(knX5, knY2));
		brush.addVertex(Vec2(knX5, knY3));
		brush.addVertex(Vec2(knX4, knY3));
		brush.addVertex(Vec2(knX4, knY2));
#endif
		cRoomBrush.push_back(brush);
	}
	{
		// NE corner.
		Brush brush;
#if REORDER_EDGES
		brush.addVertex(Vec2(knX4, knY3));
		brush.addVertex(Vec2(knX5, knY3));
		brush.addVertex(Vec2(knX5, knY5));
		brush.addVertex(Vec2(knX3, knY5));
		brush.addVertex(Vec2(knX3, knY4));
#else
		brush.addVertex(Vec2(knX5, knY5));
		brush.addVertex(Vec2(knX3, knY5));
		brush.addVertex(Vec2(knX3, knY4));
		brush.addVertex(Vec2(knX4, knY3));
		brush.addVertex(Vec2(knX5, knY3));
#endif
		cRoomBrush.push_back(brush);
	}
	{
		// N passage.
		Brush brush;
#if REORDER_EDGES
		brush.addVertex(Vec2(knX3, knY4));
		brush.addVertex(Vec2(knX3, knY5));
		brush.addVertex(Vec2(knX2, knY5));
		brush.addVertex(Vec2(knX2, knY4));
#else
		brush.addVertex(Vec2(knX3, knY5));
		brush.addVertex(Vec2(knX2, knY5));
		brush.addVertex(Vec2(knX2, knY4));
		brush.addVertex(Vec2(knX3, knY4));
#endif
		cRoomBrush.push_back(brush);
	}
	{
		// NW corner.
		Brush brush;
#if REORDER_EDGES
		brush.addVertex(Vec2(knX2, knY4));
		brush.addVertex(Vec2(knX2, knY5));
		brush.addVertex(Vec2(knX0, knY5));
		brush.addVertex(Vec2(knX0, knY3));
		brush.addVertex(Vec2(knX1, knY3));
#else
		brush.addVertex(Vec2(knX0, knY5));
		brush.addVertex(Vec2(knX0, knY3));
		brush.addVertex(Vec2(knX1, knY3));
		brush.addVertex(Vec2(knX2, knY4));
		brush.addVertex(Vec2(knX2, knY5));
#endif
		cRoomBrush.push_back(brush);
	}
	{
		// W passage.
		Brush brush;
#if REORDER_EDGES
		brush.addVertex(Vec2(knX1, knY3));
		brush.addVertex(Vec2(knX0, knY3));
		brush.addVertex(Vec2(knX0, knY2));
		brush.addVertex(Vec2(knX1, knY2));
#else
		brush.addVertex(Vec2(knX0, knY3));
		brush.addVertex(Vec2(knX0, knY2));
		brush.addVertex(Vec2(knX1, knY2));
		brush.addVertex(Vec2(knX1, knY3));
#endif
		cRoomBrush.push_back(brush);
	}


	// Room creation macros.
#define ENTITY(nX, nY) \
		pEntity = &Game::spawnEntity(); \
		pEntity->setModel(pEntityModel); \
		pEntity->setMobile(true); \
		pEntity->getColour() = Vec3(255, 0, 255); \
		pEntity->getOrigin() = Vec2(nX, nY) + kvOrigin; \
		pEntity->getVelocity() = Vec2(2, 2); \
		World::linkEntity(*pEntity);
#define BEGIN_ROOM(nX, nY) \
	{ \
		const Vec2 kvOrigin(nX, nY); \
		int n; \
		Entity* pEntity; \
		ENTITY(knX2 + 0, knY2 + 5); \
		ENTITY(knX2 + 1, knY3 + 3); \
		ENTITY(knX3 + 3, knY3 + 1); \
		ENTITY(knX3 + 5, knY2 + 0);
#define COPY_BRUSH(nBrushIndex) \
	{ \
		Brush brush; \
		for (n = 0; n != cRoomBrush[nBrushIndex].getNumberOfVertices(); ++n) \
		{ \
			brush.addVertex(cRoomBrush[nBrushIndex].getVertex(n) + kvOrigin); \
		} \
		pWorldModel->addBrush(brush); \
	}
#define END_ROOM() \
	}

	// World model and entity.
	Model* pWorldModel = new Model();
	Entity* pWorldEntity = &Game::spawnEntity();
	pWorldEntity->getColour() = Vec3(127, 127, 127);

	// NW room.
	BEGIN_ROOM(0, 0);
	COPY_BRUSH(0);
	//COPY_BRUSH(1);
	COPY_BRUSH(2);
	COPY_BRUSH(3);
	COPY_BRUSH(4);
	COPY_BRUSH(5);
	COPY_BRUSH(6);
	//COPY_BRUSH(7);
	END_ROOM();

	// NE room.
	BEGIN_ROOM(-knRoomWidth, 0);
	COPY_BRUSH(0);
	//COPY_BRUSH(1);
	COPY_BRUSH(2);
	//COPY_BRUSH(3);
	COPY_BRUSH(4);
	COPY_BRUSH(5);
	COPY_BRUSH(6);
	COPY_BRUSH(7);
	END_ROOM();

	// SW room.
	BEGIN_ROOM(-knRoomWidth, -knRoomHeight);
	COPY_BRUSH(0);
	COPY_BRUSH(1);
	COPY_BRUSH(2);
	COPY_BRUSH(3);
	COPY_BRUSH(4);
	//COPY_BRUSH(5);
	COPY_BRUSH(6);
	COPY_BRUSH(7);
	END_ROOM();

	// SE room.
	BEGIN_ROOM(0, -knRoomHeight);
	COPY_BRUSH(0);
	COPY_BRUSH(1);
	COPY_BRUSH(2);
	COPY_BRUSH(3);
	COPY_BRUSH(4);
	//COPY_BRUSH(5);
	COPY_BRUSH(6);
	COPY_BRUSH(7);
	END_ROOM();

	pWorldEntity->setModel(pWorldModel);
	pWorldEntity->setBounds(pWorldModel->getBounds());

#if 0
	{
		// Last known state from frame 39467.
		Entity* pEntity = &Game::spawnEntity();
		pEntity->setModel(pEntityModel);
		pEntity->setMobile(true);
		pEntity->getColour() = Vec3(0, 255, 255);
		pEntity->getOrigin() = Vec2(100, 100);
		pEntity->getVelocity() = Vec2(0, 0);
		World::linkEntity(*pEntity);
		Game::debugSetEntityState(*pEntity, 1116935872, 1116523338, 1058486036, 1073472374);
	}
	{
		Entity* pEntity = &Game::spawnEntity();
		pEntity->setModel(pEntityModel);
		pEntity->setMobile(true);
		pEntity->getColour() = Vec3(0, 255, 255);
		pEntity->getOrigin() = Vec2(-100, -100);
		pEntity->getVelocity() = Vec2(0, 0);
		World::linkEntity(*pEntity);
	}
	{
		Entity* pEntity = &Game::spawnEntity();
		pEntity->setModel(pEntityModel);
		pEntity->setMobile(true);
		pEntity->getColour() = Vec3(0, 255, 255);
		pEntity->getOrigin() = Vec2(-150, -150);
		pEntity->getVelocity() = Vec2(0, 0);
		World::linkEntity(*pEntity);
	}
#endif
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

	while (fTimeRemaining)
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
				_T("Infinite: f=%d e1=%d\n"),
				Game::getFrame(),
				entity.getIdentifier());
			exit(1);
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

			if (collision.m_pEntity->isMobile())
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
void
Game::setActiveEntity(
	Entity* pEntity)
{
	m_pActiveEntity = pEntity;
}


/*******************************************************************************
	TODO right now this leaks.
*******************************************************************************/
Entity&
Game::spawnEntity()
{
	Entity* pEntity = new Entity();

	pEntity->setIdentifier(m_cpEntity.size());
	m_cpEntity.push_back(pEntity);

	return *pEntity;
}
