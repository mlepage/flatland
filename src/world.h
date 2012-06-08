// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__WORLD
#define GUARD__WORLD


#include "brush.h"
#include "entity.h"
#include "matrix.h"
#include "model.h"
#include "rect.h"
#include "test.h"


/*******************************************************************************
*******************************************************************************/
class World
{


public:


// TODO clean this up, move it somewhere
struct Collision
{
	scalar m_fTime;
	Vec2 m_vEndPosition;
	Vec2 m_vNormal;
	Entity* m_pEntity;
};

	static
	std::vector<Entity*>
	getEntitiesAtPoint(
		const Vec2& kvPoint); // A point in world coordinate system.

	static
	bool
	isPointInBrush(
		const Vec2& kvPoint,  // A point in model coordinate system.
		const Brush& kBrush); // The brush.

	static
	bool
	isPointInEntity(
		const Vec2& kvPoint,    // A point in world coordinate system.
		const Entity& kEntity); // The entity.

	static
	bool
	isPointInModel(
		const Vec2& kvPoint,  // A point in model coordinate system.
		const Model& kModel); // The model.

	static
	void
	linkEntity(
		Entity& entity);

	static
	Collision
	traceEntity(
		Entity& entity,
		const Vec2& kvEnd);

	static
	void
	unlinkEntity(
		Entity& entity);


private:

#ifdef FLATLAND_ENABLE_TEST
	friend
	void
	Test::testWorld();
#endif


};


#endif // GUARD
