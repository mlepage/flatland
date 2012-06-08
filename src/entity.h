// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__ENTITY
#define GUARD__ENTITY


#include "matrix.h"
#include "model.h"
#include "rect.h"


class EntityType;


/*******************************************************************************
	Entities may or may not have models.

	They occupy area in the world.

	A mobile entity may move about in the world, changing its origin and angle.
	It therefore has transformation matrices to and from model coordinates. An
	immobile entity remains at origin (0, 0) and angle 0, and does not have a
	transformation to model coordinates.

	Changes to an entity should happen only when unlinked. Once changes are
	made, it should be linked. Then, the transformation matrices will be
	updated, the bounding box will be updated, etc.
*******************************************************************************/
class Entity
{


public:

	Entity();

	scalar
	getAngle() const;

	const Rect&
	getBounds() const;

	Vec3&
	getColour();

	const Vec3&
	getColour() const;

	int
	getIdentifier() const;

	scalar
	getMass() const;

	Model&
	getModel();

	const Model&
	getModel() const;

	scalar
	getOldAngle() const;

	const Vec2&
	getOldOrigin() const;

	Vec2&
	getOrigin();

	const Vec2&
	getOrigin() const;

	const Mat3&
	getTransformM2W() const;

	const Mat3&
	getTransformW2M() const;

	const EntityType&
	getType() const;

	Vec2&
	getVelocity();

	const Vec2&
	getVelocity() const;

	bool
	isMobile() const;

	void
	setAngle(
		const scalar kfAngle);

	void
	setBounds(
		const Rect& krBounds);

	void
	setIdentifier(
		const int knIdentifier);

	void
	setMobile(
		const bool kbMobile);

	void
	setModel(
		Model* const kpModel);

	void
	setOldAngle(
		const scalar kfOldAngle);

	void
	setOldOrigin(
		const Vec2& kvOldOrigin);

	void
	setTransformM2W(
		const Mat3& m);

	void
	setTransformW2M(
		const Mat3& m);

	void
	setType(
		const EntityType& kEntityType);


public:

	// TEMP path stuff
	int m_nPathStuckCount;
	scalar m_fPathSpeed;
	int m_nPathCurrent;
	std::vector<Vec2> m_cvPathPoint;

	int m_nInfiniteStuckCount;


private:

	// The identifier.
	int m_nIdentifier;

	// The type.
	const EntityType* m_pkEntityType;

	// The entity's model, if it has one.
	Model* m_pModel;

	// The colour.
	Vec3 m_vColour;

	// The mass.
	scalar m_fMass;

	// The origin.
	Vec2 m_vOrigin;

	// The old origin.
	Vec2 m_vOldOrigin;

	// The velocity.
	Vec2 m_vVelocity;

	// The angle.
	scalar m_fAngle;

	// The old angle.
	scalar m_fOldAngle;

	// TODO angular velocity?

	// The bounding box. WCS
	Rect m_rBounds;

	// Whether the entity is mobile or not.
	bool m_bMobile;

	// Transformation from model to world coordinates.
	Mat3 m_mTransformModelToWorld;

	// Transformation from world to model coordinates.
	Mat3 m_mTransformWorldToModel;


};


/*******************************************************************************
*******************************************************************************/
inline
Entity::Entity() :
	m_nInfiniteStuckCount(0),
	m_nIdentifier(0),
	m_pModel(0),
	m_vColour(255, 255, 255),
	m_fMass(1),
	m_fAngle(0),
	m_fOldAngle(0),
	m_bMobile(true),
	m_mTransformModelToWorld(1, 0, 0, 0, 1, 0, 0, 0, 1),
	m_mTransformWorldToModel(1, 0, 0, 0, 1, 0, 0, 0, 1)
{
}


/*******************************************************************************
*******************************************************************************/
inline
scalar
Entity::getAngle() const
{
	return m_fAngle;
}


/*******************************************************************************
*******************************************************************************/
inline
const Rect&
Entity::getBounds() const
{
	return m_rBounds;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec3&
Entity::getColour()
{
	return m_vColour;
}


/*******************************************************************************
*******************************************************************************/
inline
const Vec3&
Entity::getColour() const
{
	return m_vColour;
}


/*******************************************************************************
*******************************************************************************/
inline
int
Entity::getIdentifier() const
{
	return m_nIdentifier;
}


/*******************************************************************************
*******************************************************************************/
inline
scalar
Entity::getMass() const
{
	return m_fMass;
}


/*******************************************************************************
*******************************************************************************/
inline
Model&
Entity::getModel()
{
	return *m_pModel;
}


/*******************************************************************************
*******************************************************************************/
inline
const Model&
Entity::getModel() const
{
	return *m_pModel;
}


/*******************************************************************************
*******************************************************************************/
inline
scalar
Entity::getOldAngle() const
{
	return m_fOldAngle;
}


/*******************************************************************************
*******************************************************************************/
inline
const Vec2&
Entity::getOldOrigin() const
{
	return m_vOldOrigin;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec2&
Entity::getOrigin()
{
	return m_vOrigin;
}


/*******************************************************************************
*******************************************************************************/
inline
const Vec2&
Entity::getOrigin() const
{
	return m_vOrigin;
}


/*******************************************************************************
*******************************************************************************/
inline
const Mat3&
Entity::getTransformM2W() const
{
	return m_mTransformModelToWorld;
}


/*******************************************************************************
*******************************************************************************/
inline
const Mat3&
Entity::getTransformW2M() const
{
	return m_mTransformWorldToModel;
}


/*******************************************************************************
*******************************************************************************/
inline
const EntityType&
Entity::getType() const
{
	return *m_pkEntityType;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec2&
Entity::getVelocity()
{
	return m_vVelocity;
}


/*******************************************************************************
*******************************************************************************/
inline
const Vec2&
Entity::getVelocity() const
{
	return m_vVelocity;
}


/*******************************************************************************
*******************************************************************************/
inline
bool
Entity::isMobile() const
{
	return m_bMobile;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Entity::setAngle(
	const scalar kfAngle)
{
	m_fAngle = kfAngle;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Entity::setBounds(
	const Rect& krBounds)
{
	m_rBounds = krBounds;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Entity::setIdentifier(
	const int knIdentifier)
{
	m_nIdentifier = knIdentifier;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Entity::setMobile(
	const bool kbMobile)
{
	m_bMobile = kbMobile;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Entity::setModel(
	Model* const kpModel)
{
	m_pModel = kpModel;

	// TODO Lots of stuff to initialize here. For now, just set bounds.
	m_rBounds = kpModel->getBounds();
}


/*******************************************************************************
*******************************************************************************/
inline
void
Entity::setOldAngle(
	const scalar kfOldAngle)
{
	m_fOldAngle = kfOldAngle;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Entity::setOldOrigin(
	const Vec2& kvOldOrigin)
{
	m_vOldOrigin = kvOldOrigin;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Entity::setTransformM2W(
	const Mat3& km)
{
	m_mTransformModelToWorld = km;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Entity::setTransformW2M(
	const Mat3& km)
{
	m_mTransformWorldToModel = km;
}


/*******************************************************************************
*******************************************************************************/
inline
void
Entity::setType(
	const EntityType& kEntityType)
{
	m_pkEntityType = &kEntityType;
}


#endif // GUARD
