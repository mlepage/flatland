// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#include "world.h"

#include <cmath>
#include <fstream> // for debugging
#include "game.h"


#define TOLERANCE 1.0e-5


/*******************************************************************************
*******************************************************************************/
std::vector<Entity*>
World::getEntitiesAtPoint(
	const Vec2& kvPoint)
{
	std::vector<Entity*> cpEntity;

	for (int nEntityIndex(0); nEntityIndex != Game::getNumberOfEntities();
		++nEntityIndex)
	{
		Entity& entity = Game::getEntity(nEntityIndex);

		if (isPointInEntity(kvPoint, entity))
		{
			cpEntity.push_back(&entity);
		}
	}

	return cpEntity;
}


/*******************************************************************************
	Test the edges of the convex polygon against the point. If the point is
	outside any edge, the point is outside the polygon. This is the exterior
	algorithm.

	The definitive reference is "Point in Polygon Strategies" by Eric Haines,
	appearing in Graphics Gems IV.

	This implementation is slightly optimized because we can assume all
	polygons are in counterclockwise order (i.e. normals face outwards).
*******************************************************************************/
bool
World::isPointInBrush(
	const Vec2& kvPoint,
	const Brush& kBrush)
{
	// Check against bounding box.
	if (!Geometry::isPointInRect(
		kvPoint,
		kBrush.getBounds()))
	{
		return false;
	}

	int n1;
	int n2;

	// Check against each edge.
	for (
		n1 = kBrush.getNumberOfVertices() - 1, n2 = 0;
		n2 != kBrush.getNumberOfVertices();
		n1 = n2, ++n2)
	{
		// PERF is it faster if these are hoisted out of the loop?
		// Also it may be faster to randomize the sequence of edges tested.

		const Vec2 kvNormal(
			kBrush.getVertex(n1)[1] - kBrush.getVertex(n2)[1],
			kBrush.getVertex(n2)[0] - kBrush.getVertex(n1)[0]);
		const scalar kfOffset(
			kvNormal[0] * kBrush.getVertex(n1)[0] +
			kvNormal[1] * kBrush.getVertex(n1)[1]);

		if (kvPoint % kvNormal < kfOffset)
		{
			return false;
		}
	}

	return true;
}


/*******************************************************************************
*******************************************************************************/
bool
World::isPointInEntity(
	const Vec2& kvPoint,
	const Entity& kEntity)
{
	// Check against bounding box.
	if (!Geometry::isPointInRect(
		kvPoint,
		kEntity.getBounds()))
	{
		return false;
	}

	// PERF for an immobile entity, it's probably quicker to just check the
	// model, which already has a bounding box.

	// TODO clean this conversion mess up

	Vec2 vModelPoint(kvPoint);

	if (kEntity.isMobile())
	{
		Vec3 vPoint3 =
			kEntity.getTransformW2M() * Vec3(kvPoint[0], kvPoint[1], 1);
		vModelPoint = Vec2(vPoint3[0], vPoint3[1]);
	}

	// Check against model.
	if (isPointInModel(
		vModelPoint,
		kEntity.getModel()))
	{
		return true;
	}

	return false;
}


/*******************************************************************************
*******************************************************************************/
bool
World::isPointInModel(
	const Vec2& kvPoint,
	const Model& kModel)
{
	// Check against bounding box.
	if (!Geometry::isPointInRect(
		kvPoint,
		kModel.getBounds()))
	{
		return false;
	}

	// Check against each brush.
	for (int n(0); n != kModel.getNumberOfBrushes(); ++n)
	{
		if (isPointInBrush(
			kvPoint,
			kModel.getBrush(n)))
		{
			return true;
		}
	}

	return false;
}


/*******************************************************************************
	Situates the entity in the world. Sets the bounding box, etc.
*******************************************************************************/
void
World::linkEntity(
	Entity& entity)
{
	if (entity.isMobile())
	{
		const scalar kfAngleRad =
			Geometry::convertDegreesToRadians(entity.getAngle());

		// Set transformation matrices.
		entity.setTransformM2W(
			Geometry::makeTransform(
				kfAngleRad,
				entity.getOrigin()));
		entity.setTransformW2M(
			Geometry::makeTransformInverse(
				-kfAngleRad,
				-entity.getOrigin()));

		if (entity.getAngle() == 0)
		{
			// The entity's angle is 0. Therefore, we can simply translate the
			// model's bounding box to the new origin.
			Rect rBounds = entity.getModel().getBounds();
			rBounds.getMin() += entity.getOrigin();
			rBounds.getMax() += entity.getOrigin();
			entity.setBounds(rBounds);
		}
		else if (entity.getAngle() == entity.getOldAngle())
		{
			// The entity's angle has not changed. Therefore, we can simply
			// translate the old bounding box to the new origin.
			const Vec2 kvTranslate = entity.getOrigin() - entity.getOldOrigin();

			// TODO There may be a problem with "drift" here, over time.

			Rect rBounds = entity.getBounds();
			rBounds.getMin() += kvTranslate;
			rBounds.getMax() += kvTranslate;
			entity.setBounds(rBounds);
		}
		else
		{
			const Mat3& kmTransformM2W = entity.getTransformM2W();

			// PERF
			// Would it be faster to use Quake 2's method of setting rotated
			// bounds?

			// All four of the model's bounding box points.
			std::vector<Vec2> cvPoint(4);
			cvPoint[0] = entity.getModel().getBounds().getMin();
			cvPoint[1] = entity.getModel().getBounds().getMax();
			cvPoint[2] = Vec2(cvPoint[0][0], cvPoint[1][1]);
			cvPoint[3] = Vec2(cvPoint[1][0], cvPoint[0][1]);

			Rect rBounds(
				Vec2(99999, 99999),
				Vec2(-99999, -99999));

			for (int n(0); n != 4; ++n)
			{
				// Homogeneous coordinates.
				Vec3 v(cvPoint[n][0], cvPoint[n][1], 1);
				Vec3 vResult(kmTransformM2W * v);
				cvPoint[n] = Vec2(vResult[0], vResult[1]);
				Geometry::addPointToBounds(cvPoint[n], rBounds);
			}

			entity.setBounds(rBounds);
		}

		// Set old origin and angle.
		entity.setOldOrigin(entity.getOrigin());
		entity.setOldAngle(entity.getAngle());
	}
	else
	{
		entity.setBounds(entity.getModel().getBounds());
	}
}


/*******************************************************************************
	Traces an entity through the world, against other entities.

	Assumes the entity's start position and bounding box.

	There are two entities involved: "this" entity and the "other" entity.

	We know that this entity is mobile, because it is being traced. The other
	entity may or may not be mobile.
*******************************************************************************/
World::Collision
World::traceEntity(
	Entity& entity,
	const Vec2& kvEnd)
{
	Collision collision;
	collision.m_fTime = 1;
	collision.m_vEndPosition = kvEnd;

	// For debugging.
	int nThisEntityIndex = 0;
	for (; nThisEntityIndex != Game::getNumberOfEntities();
		++nThisEntityIndex)
	{
		const Entity& kOtherEntity = Game::getEntity(nThisEntityIndex);

		if (&entity == &kOtherEntity)
		{
			break;
		}
	}
	if (nThisEntityIndex == 13)
	{
		int n = 0;
	}

	// The bounds of the entity at its end position (t=1), in world coordinates.
	// TODO There is a bug here! We may not be moving for a whole timeslice
	// (and therefore for the whole velocity). On the other hand, we don't want
	// to miss any potential collisions, maybe we should pad this a bit.
	// 1.0001 appears to be not enough.
	// 1.01 appears to be not enough.
	Rect rEndBounds = entity.getBounds();
	rEndBounds.getMin() += entity.getVelocity() * 1.1f;
	rEndBounds.getMax() += entity.getVelocity() * 1.1f;

	// The bounds of the move, in world coordinates.
	Rect rMoveBounds = entity.getBounds();
	Geometry::addRectToBounds(rEndBounds, rMoveBounds);

	for (int nOtherEntityIndex = 0;
		nOtherEntityIndex != Game::getNumberOfEntities();
		++nOtherEntityIndex)
	{
		const Entity& kOtherEntity = Game::getEntity(nOtherEntityIndex);

		if (&entity == &kOtherEntity)
		{
			continue;
		}

		// ADDTOLERANCE?
		if (!Geometry::isRectTouchingRect(
			kOtherEntity.getBounds(), rMoveBounds))
		{
			continue;
		}

		const Model& kOtherModel = kOtherEntity.getModel();

		// Transform origin and velocity into other model's coordinate system.
		const Vec2 kvTransformedOrigin =
			kOtherEntity.isMobile() ?
				Geometry::transformPoint(
					kOtherEntity.getTransformW2M(),
					entity.getOrigin()) :
				entity.getOrigin();
		const Vec2 kvTransformedVelocity =
			kOtherEntity.isMobile() ?
				Geometry::transformDirection(
					kOtherEntity.getTransformW2M(),
					kvEnd - entity.getOrigin()) :
				kvEnd - entity.getOrigin();

		const Mat3 kmTransformThisModelToOtherModel =
			kOtherEntity.isMobile() ?
				kOtherEntity.getTransformW2M() *
					entity.getTransformM2W() :
				entity.getTransformM2W();
		const Mat3 kmTransformOtherModelToThisModel =
			kOtherEntity.isMobile() ?
				entity.getTransformW2M() *
					kOtherEntity.getTransformM2W() :
				entity.getTransformW2M();

		for (int nOtherBrushIndex = 0;
			nOtherBrushIndex != kOtherModel.getNumberOfBrushes();
			++nOtherBrushIndex)
		{
			const Brush& kOtherBrush = kOtherModel.getBrush(nOtherBrushIndex);

			// ADDTOLERANCE?
			// For immobile entities, check against bounds of brush.
			if (!kOtherEntity.isMobile() &&
				!Geometry::isRectTouchingRect(
					kOtherBrush.getBounds(), rMoveBounds))
			{
				continue;
			}

			// Whether to reverse roles and check again.
			bool bReverseCheck = true;

			int n1;
			int n2;

			// Check against each edge.
			for (
				n1 = kOtherBrush.getNumberOfVertices() - 1, n2 = 0;
				n2 != kOtherBrush.getNumberOfVertices();
				n1 = n2, ++n2)
			{
				const Vec3 kvLine = kOtherBrush.getLine(n1);
				const Vec2 kvEdgeNormal = Vec2(kvLine[0], kvLine[1]);

				// ADDTOLERANCE?
				if (0 < kvTransformedVelocity % kvEdgeNormal)
				{
					continue;
				}

				const scalar kfEdgeDistance = kvLine[2];

				const Geometry::EffectiveRadius kEffectiveRadius =
					Geometry::makeEffectiveRadius(
						entity.getModel(),
						Geometry::transformDirection(
							kmTransformOtherModelToThisModel,
							kvEdgeNormal));
				const scalar kfEffectiveRadius =
					kEffectiveRadius.m_fMinDistance;

				const scalar kfDistanceFromPlane =
					kvLine %
					Vec3(kvTransformedOrigin[0], kvTransformedOrigin[1], 1);

#ifdef FLATLAND_ENABLE_EFFECTIVE_RADIUS_MAX
				// ADDTOLERANCE?
				// Skip the edge if entity is fully behind it.
				if (kfDistanceFromPlane <= -kEffectiveRadius.m_fMaxDistance)
				{
					continue;
				}
#endif

				// TODO need to check entity does not intersect edge plane.

				const scalar kfDenominator =
					kvEdgeNormal % kvTransformedVelocity;

				// ADDTOLERANCE?
				if (kfDenominator == 0)
				{
					continue;
				}

				const scalar kfCollisionTime =
					-(Vec3(
						kvEdgeNormal[0],
						kvEdgeNormal[1],
						kfEdgeDistance + kfEffectiveRadius) %
					Vec3(
						kvTransformedOrigin[0],
						kvTransformedOrigin[1],
						1)) /
					kfDenominator;

				if (kfCollisionTime < 0 - TOLERANCE ||
					1 + TOLERANCE <= kfCollisionTime)
				{
					// No collision with line has occurred.

					if (TOLERANCE <= kfDistanceFromPlane + kfEffectiveRadius)
					{
						// Entity was initially completely on positive side of
						// line. No other edge can collide. Continue with next
						// brush.
						bReverseCheck = false;
						break;
					}
					else
					{
						// Continue with next edge.
						continue;
					}
				}

				const Vec2 kvEdge =
					kOtherBrush.getVertex(n2) - kOtherBrush.getVertex(n1);
				
				// Determine whether line collision occurs on edge.
				const Vec2 kvCollisionPoint1 =
					Geometry::transformPoint(
						kmTransformThisModelToOtherModel,
						kEffectiveRadius.m_vMin1) +
						kfCollisionTime * kvTransformedVelocity;
				const scalar kfCollisionDotProd1 =
					(kvCollisionPoint1 - kOtherBrush.getVertex(n1)) % kvEdge;
				const scalar kfEdgeDotProd =
					kvEdge % kvEdge;
				if (kEffectiveRadius.m_bMinEdge)
				{
					const Vec2 kvCollisionPoint2 =
						Geometry::transformPoint(
							kmTransformThisModelToOtherModel,
							kEffectiveRadius.m_vMin2) +
							kfCollisionTime * kvTransformedVelocity;
					const scalar kfCollisionDotProd2 =
						(kvCollisionPoint2 - kOtherBrush.getVertex(n1)) % kvEdge;
					const scalar kfFirstCollisionDotProd =
						kfCollisionDotProd1 <= kfCollisionDotProd2 ?
						kfCollisionDotProd1 :
						kfCollisionDotProd2;
					const scalar kfSecondCollisionDotProd =
						kfCollisionDotProd1 <= kfCollisionDotProd2 ?
						kfCollisionDotProd2 :
						kfCollisionDotProd1;
					if (kfSecondCollisionDotProd < 0 - TOLERANCE ||
						kfEdgeDotProd + TOLERANCE < kfFirstCollisionDotProd)
					{
						// Continue with next edge.
						continue;
					}
				}
				else
				{
					if (kfCollisionDotProd1 < 0 - TOLERANCE ||
						kfEdgeDotProd + TOLERANCE < kfCollisionDotProd1)
					{
						// Continue with next edge.
						continue;
					}
				}

				// TODO If collision time is zero, we can exit this routine.

				// TODO end position and collision point are different things,
				// resolve this.

				// ADDTOLERANCE?
				if (kfCollisionTime < collision.m_fTime)
				{
					// Collision time is earliest yet, so record it.
					collision.m_fTime = kfCollisionTime;
					if (kOtherEntity.isMobile())
					{
						collision.m_vEndPosition =
							Geometry::transformPoint(
								kOtherEntity.getTransformM2W(),
								kvCollisionPoint1);
						collision.m_vNormal =
							Geometry::transformDirection(
								kOtherEntity.getTransformM2W(),
								kvEdgeNormal);
						collision.m_pEntity = &const_cast<Entity&>(kOtherEntity);
					}
					else
					{
						collision.m_vEndPosition = kvCollisionPoint1;
						collision.m_vNormal = kvEdgeNormal;
						collision.m_pEntity = &const_cast<Entity&>(kOtherEntity);
					}
				}

				// No other edge can collide earlier.
				// Continue with next brush.
				bReverseCheck = false;
				break;
			}

			if (!bReverseCheck)
			{
				continue;
			}

			// Reverse check. Now, assume the brush is moving and check against
			// this entity's model. So we are checking other brush against this
			// brush. We do not need to check edge collisions.

			// Transform other origin and velocity into this model's
			// coordinate system.
			const Vec2 kvOtherTransformedOrigin =
				Geometry::transformPoint(
					entity.getTransformW2M(),
					kOtherEntity.getOrigin());
			const Vec2 kvOtherTransformedVelocity =
				Geometry::transformDirection(
					entity.getTransformW2M(),
					-(kvEnd - entity.getOrigin()));

			for (int nThisBrushIndex = 0;
				nThisBrushIndex != entity.getModel().getNumberOfBrushes();
				++nThisBrushIndex)
			{
				const Brush& kThisBrush =
					entity.getModel().getBrush(nThisBrushIndex);

				// Check against each edge.
				for (
					n1 = kThisBrush.getNumberOfVertices() - 1, n2 = 0;
					n2 != kThisBrush.getNumberOfVertices();
					n1 = n2, ++n2)
				{
					const Vec3 kvLine = kThisBrush.getLine(n1);
					const Vec2 kvEdgeNormal = Vec2(kvLine[0], kvLine[1]);

					// ADDTOLERANCE?
					if (0 < kvOtherTransformedVelocity % kvEdgeNormal)
					{
						continue;
					}

					const scalar kfEdgeDistance = kvLine[2];

					const Geometry::EffectiveRadius kEffectiveRadius =
						Geometry::makeEffectiveRadius(
							kOtherBrush,
							Geometry::transformDirection(
								kmTransformThisModelToOtherModel,
								kvEdgeNormal));
					const scalar kfEffectiveRadius =
						kEffectiveRadius.m_fMinDistance;

					const scalar kfDistanceFromPlane =
						kvLine %
						Vec3(kvOtherTransformedOrigin[0], kvOtherTransformedOrigin[1], 1);

					const scalar kfDenominator =
						kvEdgeNormal % kvOtherTransformedVelocity;

					// ADDTOLERANCE?
					if (kfDenominator == 0)
					{
						continue;
					}

					const scalar kfCollisionTime =
						-(Vec3(
							kvEdgeNormal[0],
							kvEdgeNormal[1],
							kfEdgeDistance + kfEffectiveRadius) %
						Vec3(
							kvOtherTransformedOrigin[0],
							kvOtherTransformedOrigin[1],
							1)) /
						kfDenominator;

					if (kfCollisionTime < 0 - TOLERANCE ||
						1 + TOLERANCE <= kfCollisionTime)
					{
						// No collision with line has occurred.

						if (TOLERANCE <= kfDistanceFromPlane + kfEffectiveRadius)
						{
							// Entity was initially completely on positive side of
							// line. No other edge can collide. Continue with next
							// brush.
							break;
						}
						else
						{
							// Continue with next edge.
							continue;
						}
					}

					const Vec2 kvEdge =
						kThisBrush.getVertex(n2) - kThisBrush.getVertex(n1);

					const Vec2 kvCollisionPoint1 =
						Geometry::transformPoint(
							kmTransformOtherModelToThisModel,
							kEffectiveRadius.m_vMin1) +
							kfCollisionTime * kvOtherTransformedVelocity;
					const scalar kfCollisionDotProd1 =
						(kvCollisionPoint1 - kThisBrush.getVertex(n1)) % kvEdge;
					const scalar kfEdgeDotProd =
						kvEdge % kvEdge;
					if (kEffectiveRadius.m_bMinEdge)
					{
						const Vec2 kvCollisionPoint2 =
							Geometry::transformPoint(
								kmTransformOtherModelToThisModel,
								kEffectiveRadius.m_vMin2) +
								kfCollisionTime * kvOtherTransformedVelocity;
						const scalar kfCollisionDotProd2 =
							(kvCollisionPoint2 - kThisBrush.getVertex(n1)) % kvEdge;
						const scalar kfFirstCollisionDotProd =
							kfCollisionDotProd1 <= kfCollisionDotProd2 ?
							kfCollisionDotProd1 :
							kfCollisionDotProd2;
						const scalar kfSecondCollisionDotProd =
							kfCollisionDotProd1 <= kfCollisionDotProd2 ?
							kfCollisionDotProd2 :
							kfCollisionDotProd1;
						if (kfSecondCollisionDotProd < 0 - TOLERANCE ||
							kfEdgeDotProd + TOLERANCE < kfFirstCollisionDotProd)
						{
							// Continue with next edge.
							continue;
						}
					}
					else
					{
						if (kfCollisionDotProd1 < 0 - TOLERANCE ||
							kfEdgeDotProd + TOLERANCE < kfCollisionDotProd1)
						{
							// Continue with next edge.
							continue;
						}
					}

					// ADDTOLERANCE?
					if (kfCollisionTime < collision.m_fTime)
					{
						// Collision time is earliest yet, so record it.
						collision.m_fTime = kfCollisionTime;
						collision.m_vNormal =
							-Geometry::transformDirection(
								entity.getTransformM2W(),
								kvEdgeNormal);
						collision.m_pEntity = &const_cast<Entity&>(kOtherEntity);
					}
				}
			}
		}
	}

	return collision;
}


/*******************************************************************************
*******************************************************************************/
void
World::unlinkEntity(
	Entity& entity)
{
}
