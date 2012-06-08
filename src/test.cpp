// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#pragma warning(disable: 4786)


#include "test.h"


#ifdef FLATLAND_ENABLE_TEST


#include <windows.h>
#include <tchar.h>
#include "exception.h"
#include "matrix.h"
#include "world.h"


// This macro tests a condition and displays an informative message box if it
// fails.
#define CHECK(x) \
	if (!(x)) \
	{ \
		MessageBox( \
			NULL, \
			_T("Failed unit test:\n" _T(#x)), \
			_T("Flatland Error"), \
			MB_OK | MB_SETFOREGROUND | MB_ICONSTOP); \
	}


/*******************************************************************************
*******************************************************************************/
int
testExceptionHelper(
	const int knThrow,
	const int knCode)
{
	int nResult = 0;

	TRY
	{
		nResult |= 1<<0;
		if (knThrow == 1<<0) THROW(knCode);
		TRY
		{
			nResult |= 1<<1;
			if (knThrow == 1<<1) THROW(knCode);
		}
		CATCH (1)
		{
			nResult |= 1<<2;
		}
		CATCH (3)
		{
			nResult |= 1<<3;
			RETHROW;
		}
		END_TRY_CATCH
		nResult |= 1<<4;
		if (knThrow == 1<<4) THROW(knCode);
	}
	CATCH (1)
	{
		nResult |= 1<<5;
	}
	CATCH (2)
	{
		nResult |= 1<<6;
	}
	CATCHALL
	{
		nResult |= 1<<7;
	}
	END_TRY_CATCHALL

	return nResult;
}


/*******************************************************************************
*******************************************************************************/
void
Test::test()
{
	testException();
	testVec2();
	testVec3();
	testMat3();
	testGeometry();
	testWorld();
}


/*******************************************************************************
*******************************************************************************/
void
Test::testException()
{
	CHECK(testExceptionHelper(0, 1) == (1<<0|1<<1|1<<4));

	CHECK(testExceptionHelper(1<<0, 1) == (1<<0|1<<5));
	CHECK(testExceptionHelper(1<<0, 2) == (1<<0|1<<6));
	CHECK(testExceptionHelper(1<<0, 3) == (1<<0|1<<7));
	CHECK(testExceptionHelper(1<<0, 4) == (1<<0|1<<7));

	CHECK(testExceptionHelper(1<<1, 1) == (1<<0|1<<1|1<<2|1<<4));
	CHECK(testExceptionHelper(1<<1, 2) == (1<<0|1<<1|1<<6));
	CHECK(testExceptionHelper(1<<1, 3) == (1<<0|1<<1|1<<3|1<<7));
	CHECK(testExceptionHelper(1<<1, 4) == (1<<0|1<<1|1<<7));

	CHECK(testExceptionHelper(1<<4, 1) == (1<<0|1<<1|1<<4|1<<5));
	CHECK(testExceptionHelper(1<<4, 2) == (1<<0|1<<1|1<<4|1<<6));
	CHECK(testExceptionHelper(1<<4, 3) == (1<<0|1<<1|1<<4|1<<7));
	CHECK(testExceptionHelper(1<<4, 4) == (1<<0|1<<1|1<<4|1<<7));
}


/*******************************************************************************
*******************************************************************************/
void
Test::testGeometry()
{
	Brush brush;
	brush.addVertex(Vec2(0, 0));
	brush.addVertex(Vec2(10, 0));
	brush.addVertex(Vec2(10, 10));
	brush.addVertex(Vec2(0, 10));

	const scalar kfDiagonal = sqrt(10 * 10 + 10 * 10);

	{
		const Vec2 kvNormal =
			Geometry::transformDirection(
				Geometry::makeTransform(
					Geometry::convertDegreesToRadians(45),
					Vec2(0, 0)),
				Vec2(1, 0));
		const Geometry::EffectiveRadius kEffectiveRadius =
			Geometry::makeEffectiveRadius(brush, kvNormal);
		CHECK(kEffectiveRadius.m_vNormal == kvNormal);
		CHECK(kEffectiveRadius.m_fMinDistance == 0);
		CHECK(kEffectiveRadius.m_bMinEdge == false);
		CHECK(kEffectiveRadius.m_vMin1 == brush.getVertex(0));
#ifdef FLATLAND_ENABLE_EFFECTIVE_RADIUS_MAX
		CHECK(kEffectiveRadius.m_fMaxDistance == kfDiagonal);
		CHECK(kEffectiveRadius.m_bMaxEdge == false);
		CHECK(kEffectiveRadius.m_vMax1 == brush.getVertex(2));
#endif
	}
	{
		const Vec2 kvNormal(0, 1);
		const Geometry::EffectiveRadius kEffectiveRadius =
			Geometry::makeEffectiveRadius(brush, kvNormal);
		CHECK(kEffectiveRadius.m_vNormal == kvNormal);
		CHECK(kEffectiveRadius.m_fMinDistance == 0);
		CHECK(kEffectiveRadius.m_bMinEdge == true);
		CHECK(kEffectiveRadius.m_vMin1 == brush.getVertex(0));
		CHECK(kEffectiveRadius.m_vMin2 == brush.getVertex(1));
#ifdef FLATLAND_ENABLE_EFFECTIVE_RADIUS_MAX
		CHECK(kEffectiveRadius.m_fMaxDistance == 10);
		CHECK(kEffectiveRadius.m_bMaxEdge == true);
		CHECK(kEffectiveRadius.m_vMax1 == brush.getVertex(2));
		CHECK(kEffectiveRadius.m_vMax2 == brush.getVertex(3));
#endif
	}
	{
		const Vec2 kvNormal(-1, 0);
		const Geometry::EffectiveRadius kEffectiveRadius =
			Geometry::makeEffectiveRadius(brush, kvNormal);
		CHECK(kEffectiveRadius.m_vNormal == kvNormal);
		CHECK(kEffectiveRadius.m_fMinDistance == -10);
		CHECK(kEffectiveRadius.m_bMinEdge == true);
		CHECK(kEffectiveRadius.m_vMin1 == brush.getVertex(1));
		CHECK(kEffectiveRadius.m_vMin2 == brush.getVertex(2));
#ifdef FLATLAND_ENABLE_EFFECTIVE_RADIUS_MAX
		CHECK(kEffectiveRadius.m_fMaxDistance == 0);
		CHECK(kEffectiveRadius.m_bMaxEdge == true);
		CHECK(kEffectiveRadius.m_vMax1 == brush.getVertex(3));
		CHECK(kEffectiveRadius.m_vMax2 == brush.getVertex(0));
#endif
	}
	{
		const Vec2 kvNormal(0, -1);
		const Geometry::EffectiveRadius kEffectiveRadius =
			Geometry::makeEffectiveRadius(brush, kvNormal);
		CHECK(kEffectiveRadius.m_vNormal == kvNormal);
		CHECK(kEffectiveRadius.m_fMinDistance == -10);
		CHECK(kEffectiveRadius.m_bMinEdge == true);
		CHECK(kEffectiveRadius.m_vMin1 == brush.getVertex(2));
		CHECK(kEffectiveRadius.m_vMin2 == brush.getVertex(3));
#ifdef FLATLAND_ENABLE_EFFECTIVE_RADIUS_MAX
		CHECK(kEffectiveRadius.m_fMaxDistance == 0);
		CHECK(kEffectiveRadius.m_bMaxEdge == true);
		CHECK(kEffectiveRadius.m_vMax1 == brush.getVertex(0));
		CHECK(kEffectiveRadius.m_vMax2 == brush.getVertex(1));
#endif
	}
	{
		const Vec2 kvNormal(1, 0);
		const Geometry::EffectiveRadius kEffectiveRadius =
			Geometry::makeEffectiveRadius(brush, kvNormal);
		CHECK(kEffectiveRadius.m_vNormal == kvNormal);
		CHECK(kEffectiveRadius.m_fMinDistance == 0);
		CHECK(kEffectiveRadius.m_bMinEdge == true);
		CHECK(kEffectiveRadius.m_vMin1 == brush.getVertex(3));
		CHECK(kEffectiveRadius.m_vMin2 == brush.getVertex(0));
#ifdef FLATLAND_ENABLE_EFFECTIVE_RADIUS_MAX
		CHECK(kEffectiveRadius.m_fMaxDistance == 10);
		CHECK(kEffectiveRadius.m_bMaxEdge == true);
		CHECK(kEffectiveRadius.m_vMax1 == brush.getVertex(1));
		CHECK(kEffectiveRadius.m_vMax2 == brush.getVertex(2));
#endif
	}
	{
		const Mat3 kmTransform =
			Geometry::makeTransform(
				Geometry::convertDegreesToRadians(45),
				Vec2(0, 0));

		Brush brush;
		brush.addVertex(Geometry::transformPoint(kmTransform, Vec2(0, 0)));
		brush.addVertex(Geometry::transformPoint(kmTransform, Vec2(10, 0)));
		brush.addVertex(Geometry::transformPoint(kmTransform, Vec2(10, 10)));
		brush.addVertex(Geometry::transformPoint(kmTransform, Vec2(0, 10)));

		const Vec2 kvNormal =
			Geometry::transformDirection(
				kmTransform,
				Vec2(0, 1));
		const Geometry::EffectiveRadius kEffectiveRadius =
			Geometry::makeEffectiveRadius(brush, kvNormal);
		CHECK(kEffectiveRadius.m_vNormal == kvNormal);
		CHECK(kEffectiveRadius.m_fMinDistance == 0);
		CHECK(kEffectiveRadius.m_bMinEdge == true);
		CHECK(kEffectiveRadius.m_vMin1 == brush.getVertex(0));
		CHECK(kEffectiveRadius.m_vMin2 == brush.getVertex(1));
#ifdef FLATLAND_ENABLE_EFFECTIVE_RADIUS_MAX
		CHECK(kEffectiveRadius.m_fMaxDistance == 10);
		CHECK(kEffectiveRadius.m_bMaxEdge == true);
		CHECK(kEffectiveRadius.m_vMax1 == brush.getVertex(2));
		CHECK(kEffectiveRadius.m_vMax2 == brush.getVertex(3));
#endif
	}
}


/*******************************************************************************
*******************************************************************************/
void
Test::testMat3()
{
	Mat3 m1(
		2, 3, 5,
		7, 11, 13,
		17, 19, 23);
	Mat3 m2(
		29, 31, 37,
		41, 43, 47,
		53, 59, 61);

	CHECK(
		m1[0][0] == 2 && m1[0][1] == 3 && m1[0][2] == 5 &&
		m1[1][0] == 7 && m1[1][1] == 11 && m1[1][2] == 13 &&
		m1[2][0] == 17 && m1[2][1] == 19 && m1[2][2] == 23);
	CHECK(
		m2[0][0] == 29 && m2[0][1] == 31 && m2[0][2] == 37 &&
		m2[1][0] == 41 && m2[1][1] == 43 && m2[1][2] == 47 &&
		m2[2][0] == 53 && m2[2][1] == 59 && m2[2][2] == 61);

	{
		Mat3 m;
		CHECK((
			m[0][0] = -1,
			m[0][1] = -2,
			m[0][2] = -3,
			m[1][0] = -4,
			m[1][1] = -5,
			m[1][2] = -6,
			m[2][0] = -7,
			m[2][1] = -8,
			m[2][2] = -9,
			m ==
				Mat3(
					-1, -2, -3,
					-4, -5, -6,
					-7, -8, -9)));
	}

	// Matrix multiplication.
	CHECK(m1 * m2 ==
		Mat3(
			446, 486, 520,
			1343, 1457, 1569,
			2491, 2701, 2925));

	// Scalar multiplication.
	CHECK(m1 * 2 ==
		Mat3(
			4, 6, 10,
			14, 22, 26,
			34, 38, 46));
	CHECK(m1 * 2 == 2 * m1);

	// Scalar division.
	CHECK(m1 / 2 ==
		Mat3(
			1, 1.5, 2.5,
			3.5, 5.5, 6.5,
			8.5, 9.5, 11.5));

	// Addition.
	CHECK(m1 + m2 ==
		Mat3(
			31, 34, 42,
			48, 54, 60,
			70, 78, 84));
	CHECK(m1 + m2 == m2 + m1);

	// Subtraction.
	CHECK(m1 - m2 ==
		Mat3(
			-27, -28, -32,
			-34, -32, -34,
			-36, -40, -38));
	CHECK(m2 - m1 ==
		Mat3(
			27, 28, 32,
			34, 32, 34,
			36, 40, 38));
}


/*******************************************************************************
*******************************************************************************/
void
Test::testVec2()
{
}


/*******************************************************************************
*******************************************************************************/
void
Test::testVec3()
{
	Vec3 v1(2, 3, 5);
	Vec3 v2(7, 11, 13);
	Vec3 v3(17, 19, 23);

	CHECK(v1[0] == 2 && v1[1] == 3 && v1[2] == 5);
	CHECK(v2[0] == 7 && v2[1] == 11 && v2[2] == 13);
	CHECK(v3[0] == 17 && v3[1] == 19 && v3[2] == 23);

	{
		Vec3 v;
		CHECK((
			v[0] = -1,
			v[1] = -2,
			v[2] = -3,
			v == Vec3(-1, -2, -3)));
	}

	// Cross product.
	CHECK(v1 * v2 == Vec3(-16, 9, 1));
	CHECK(v2 * v1 == Vec3(16, -9, -1));

	// Scalar multiplication.
	CHECK(v1 * 2 == Vec3(4, 6, 10));
	CHECK(v1 * 2 == 2 * v1);

	// Scalar division.
	CHECK(v1 / 2 == Vec3(1, 1.5, 2.5));
	CHECK(v2 / 2 == Vec3(3.5, 5.5, 6.5));
	CHECK(v3 / 2 == Vec3(8.5, 9.5, 11.5));

	// Dot product.
	CHECK(v1 % v2 == 112);
	CHECK(v1 % v2 == v2 % v1);
	CHECK(v1 % v3 == 206);
	CHECK(v1 % v3 == v3 % v1);

	// Addition.
	CHECK(v1 + v2 == Vec3(9, 14, 18));
	CHECK(v1 + v2 == v2 + v1);

	// Subtraction.
	CHECK(v1 - v2 == Vec3(-5, -8, -8));
	CHECK(v2 - v1 == Vec3(5, 8, 8));
}


/*******************************************************************************
*******************************************************************************/
void
Test::testWorld()
{
	Brush brush1;
	brush1.addVertex(Vec2(5, -10));
	brush1.addVertex(Vec2(15, 0));
	brush1.addVertex(Vec2(5, 20));

	Brush brush2;
	brush1.addVertex(Vec2(-5, 20));
	brush1.addVertex(Vec2(-15, 0));
	brush2.addVertex(Vec2(-5, -10));

	{
		Brush brush;
		brush.addVertex(Vec2(0, 10));
		brush.addVertex(Vec2(-11, 0));
		brush.addVertex(Vec2(0, -12));
		brush.addVertex(Vec2(13, 0));

		// Center.
		CHECK(World::isPointInBrush(Vec2(0, 0), brush));

		// Vertices.
		CHECK(World::isPointInBrush(Vec2(0, 10), brush));
		CHECK(World::isPointInBrush(Vec2(-11, 0), brush));
		CHECK(World::isPointInBrush(Vec2(0, -12), brush));
		CHECK(World::isPointInBrush(Vec2(13, 0), brush));

		// Vertices.
		CHECK(!World::isPointInBrush(Vec2(0, 10.1f), brush));
		CHECK(!World::isPointInBrush(Vec2(-11.1f, 0), brush));
		CHECK(!World::isPointInBrush(Vec2(0, -12.1f), brush));
		CHECK(!World::isPointInBrush(Vec2(13.1f, 0), brush));
	}
}


#endif
