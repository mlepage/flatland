// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__MATRIX
#define GUARD__MATRIX


#include "test.h"


// The scalar type.
typedef float scalar;


/*******************************************************************************
	Vector with 2 elements, indexed 0, 1.

	Same as Vec3.
*******************************************************************************/
class Vec2
{


public:

	Vec2();

	Vec2(
		const scalar kf0,
		const scalar kf1);

	scalar&
	operator [](
		const int kn);

	const scalar&
	operator [](
		const int kn) const;

	Vec2&
	operator *=(
		const Vec2& kv);

	Vec2&
	operator *=(
		const scalar kf);

	Vec2&
	operator /=(
		const scalar kf);

	Vec2&
	operator +=(
		const Vec2& kv);

	Vec2&
	operator -=(
		const Vec2& kv);

private:

	scalar m_af[2];

#ifdef FLATLAND_ENABLE_TEST
	friend
	void
	Test::testVec2();
#endif


};


/*******************************************************************************
*******************************************************************************/
inline
Vec2::Vec2()
{
	m_af[0] = 0;
	m_af[1] = 0;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec2::Vec2(
	const scalar kf0,
	const scalar kf1)
{
	m_af[0] = kf0;
	m_af[1] = kf1;
}


/*******************************************************************************
*******************************************************************************/
inline
scalar&
Vec2::operator [](
	const int kn)
{
	return m_af[kn];
}


/*******************************************************************************
*******************************************************************************/
inline
const scalar&
Vec2::operator [](
	const int kn) const
{
	return m_af[kn];
}


/*******************************************************************************
	Moved below to allow inlining.
  
	Vec2&
	Vec2::operator *=(
		const Vec2&)
*******************************************************************************/


/*******************************************************************************
*******************************************************************************/
inline
Vec2&
Vec2::operator *=(
	const scalar kf)
{
	m_af[0] *= kf;
	m_af[1] *= kf;
	return *this;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec2&
Vec2::operator /=(
	const scalar kf)
{
	m_af[0] /= kf;
	m_af[1] /= kf;
	return *this;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec2&
Vec2::operator +=(
	const Vec2& kv)
{
	m_af[0] += kv.m_af[0];
	m_af[1] += kv.m_af[1];
	return *this;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec2&
Vec2::operator -=(
	const Vec2& kv)
{
	m_af[0] -= kv.m_af[0];
	m_af[1] -= kv.m_af[1];
	return *this;
}


/*******************************************************************************
	Unary minus.
*******************************************************************************/
inline
Vec2
operator -(
	const Vec2& kv)
{
	return Vec2(-kv[0], -kv[1]);
}


/*******************************************************************************
	Cross product.
*******************************************************************************/
inline
Vec2
operator *(
	const Vec2& kv1,
	const Vec2& kv2)
{
#if 0
	return
		Vec2(
			kv1[1]*kv2[2] - kv1[2]*kv2[1],
			kv1[2]*kv2[0] - kv1[0]*kv2[2],
			kv1[0]*kv2[1] - kv1[1]*kv2[0]);
#else
	// Need to look up definition.
	return Vec2();
#endif
}


/*******************************************************************************
*******************************************************************************/
inline
Vec2
operator *(
	const Vec2& kv,
	const scalar kf)
{
	return Vec2(kv) *= kf;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec2
operator *(
	const scalar kf,
	const Vec2& kv)
{
	// Commutative.
	return Vec2(kv) *= kf;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec2
operator /(
	const Vec2& kv,
	const scalar kf)
{
	return Vec2(kv) /= kf;
}


/*******************************************************************************
	Dot product.
*******************************************************************************/
inline
scalar
operator %(
	const Vec2& kv1,
	const Vec2& kv2)
{
	return
		kv1[0]*kv2[0] + kv1[1]*kv2[1];
}


/*******************************************************************************
*******************************************************************************/
inline
Vec2
operator +(
	const Vec2& kv1,
	const Vec2& kv2)
{
	return Vec2(kv1) += kv2;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec2
operator -(
	const Vec2& kv1,
	const Vec2& kv2)
{
	return Vec2(kv1) -= kv2;
}


/*******************************************************************************
*******************************************************************************/
inline
bool
operator ==(
	const Vec2& kv1,
	const Vec2& kv2)
{
	return
		kv1[0] == kv2[0] &&
		kv1[1] == kv2[1];
}


/*******************************************************************************
*******************************************************************************/
inline
bool
operator !=(
	const Vec2& kv1,
	const Vec2& kv2)
{
	return
		kv1[0] != kv2[0] ||
		kv1[1] != kv2[1];
}


/*******************************************************************************
	Moved from above to allow inlining.
*******************************************************************************/
inline
Vec2&
Vec2::operator *=(
	const Vec2& kv)
{
	return *this = *this * kv;
}


/*******************************************************************************
	Vector with 3 elements, indexed 0, 1, 2.

	TODO
	- Vec3 *= Mat3

	PERFORMANCE NOTES

    It might be possible to speed up the nonmember operators by making them
	friends and directly accessing members.
*******************************************************************************/
class Vec3
{


public:

	Vec3();

	Vec3(
		const scalar kf0,
		const scalar kf1,
		const scalar kf2);

	scalar&
	operator [](
		const int kn);

	const scalar&
	operator [](
		const int kn) const;

	Vec3&
	operator *=(
		const Vec3& kv);

	Vec3&
	operator *=(
		const scalar kf);

	Vec3&
	operator /=(
		const scalar kf);

	Vec3&
	operator +=(
		const Vec3& kv);

	Vec3&
	operator -=(
		const Vec3& kv);

private:

	scalar m_af[3];

#ifdef FLATLAND_ENABLE_TEST
	friend
	void
	Test::testVec3();
#endif


};


/*******************************************************************************
*******************************************************************************/
inline
Vec3::Vec3()
{
	m_af[0] = 0;
	m_af[1] = 0;
	m_af[2] = 0;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec3::Vec3(
	const scalar kf0,
	const scalar kf1,
	const scalar kf2)
{
	m_af[0] = kf0;
	m_af[1] = kf1;
	m_af[2] = kf2;
}


/*******************************************************************************
*******************************************************************************/
inline
scalar&
Vec3::operator [](
	const int kn)
{
	return m_af[kn];
}


/*******************************************************************************
*******************************************************************************/
inline
const scalar&
Vec3::operator [](
	const int kn) const
{
	return m_af[kn];
}


/*******************************************************************************
	Moved below to allow inlining.
  
	Vec3&
	Vec3::operator *=(
		const Vec3&)
*******************************************************************************/


/*******************************************************************************
*******************************************************************************/
inline
Vec3&
Vec3::operator *=(
	const scalar kf)
{
	m_af[0] *= kf;
	m_af[1] *= kf;
	m_af[2] *= kf;
	return *this;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec3&
Vec3::operator /=(
	const scalar kf)
{
	m_af[0] /= kf;
	m_af[1] /= kf;
	m_af[2] /= kf;
	return *this;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec3&
Vec3::operator +=(
	const Vec3& kv)
{
	// PERFORMANCE NOTE
	// I wondered if kv[0] would be just as fast as kv.m_af[0]. It should.
	// On the PC there is no performance difference. But on the Pocket PC
	// there was a performance degradation by factor 1.13.
	m_af[0] += kv.m_af[0];
	m_af[1] += kv.m_af[1];
	m_af[2] += kv.m_af[2];
	return *this;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec3&
Vec3::operator -=(
	const Vec3& kv)
{
	m_af[0] -= kv.m_af[0];
	m_af[1] -= kv.m_af[1];
	m_af[2] -= kv.m_af[2];
	return *this;
}


/*******************************************************************************
	Unary minus.
*******************************************************************************/
inline
Vec3
operator -(
	const Vec3& kv)
{
	return Vec3(-kv[0], -kv[1], -kv[2]);
}


/*******************************************************************************
	Cross product.
*******************************************************************************/
inline
Vec3
operator *(
	const Vec3& kv1,
	const Vec3& kv2)
{
	return
		Vec3(
			kv1[1]*kv2[2] - kv1[2]*kv2[1],
			kv1[2]*kv2[0] - kv1[0]*kv2[2],
			kv1[0]*kv2[1] - kv1[1]*kv2[0]);
}


/*******************************************************************************
*******************************************************************************/
inline
Vec3
operator *(
	const Vec3& kv,
	const scalar kf)
{
	return Vec3(kv) *= kf;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec3
operator *(
	const scalar kf,
	const Vec3& kv)
{
	// Commutative.
	return Vec3(kv) *= kf;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec3
operator /(
	const Vec3& kv,
	const scalar kf)
{
	return Vec3(kv) /= kf;
}


/*******************************************************************************
	Dot product.
*******************************************************************************/
inline
scalar
operator %(
	const Vec3& kv1,
	const Vec3& kv2)
{
	return
		kv1[0]*kv2[0] + kv1[1]*kv2[1] + kv1[2]*kv2[2];
}


/*******************************************************************************
*******************************************************************************/
inline
Vec3
operator +(
	const Vec3& kv1,
	const Vec3& kv2)
{
	// PERFORMANCE NOTE
	// Can this be made faster by code like this?
	// return Vec3(
	//     kv1[0] + kv2[0],
	//     kv1[1] + kv2[1],
	//     kv1[2] + kv2[2]);
	// It seems it can on the PC, but there is no difference on the Pocket PC.
	return Vec3(kv1) += kv2;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec3
operator -(
	const Vec3& kv1,
	const Vec3& kv2)
{
	return Vec3(kv1) -= kv2;
}


/*******************************************************************************
*******************************************************************************/
inline
bool
operator ==(
	const Vec3& kv1,
	const Vec3& kv2)
{
	return
		kv1[0] == kv2[0] &&
		kv1[1] == kv2[1] &&
		kv1[2] == kv2[2];
}


/*******************************************************************************
*******************************************************************************/
inline
bool
operator !=(
	const Vec3& kv1,
	const Vec3& kv2)
{
	return
		kv1[0] != kv2[0] ||
		kv1[1] != kv2[1] ||
		kv1[2] != kv2[2];
}


/*******************************************************************************
	Moved from above to allow inlining.
*******************************************************************************/
inline
Vec3&
Vec3::operator *=(
	const Vec3& kv)
{
	return *this = *this * kv;
}


/*******************************************************************************
	Matrix with 3 rows and 3 columns.

	The elements are specified by row then column, starting from zero:

	[ 00 01 02 ]
	[ 10 11 12 ]
	[ 20 21 22 ]
*******************************************************************************/
class Mat3
{


public:

	Mat3();

	Mat3(
		const Vec3& kvRow0,
		const Vec3& kvRow1,
		const Vec3& kvRow2);

	Mat3(
		const scalar kf00,
		const scalar kf01,
		const scalar kf02,
		const scalar kf10,
		const scalar kf11,
		const scalar kf12,
		const scalar kf20,
		const scalar kf21,
		const scalar kf22);

	Vec3&
	operator [](
		const int kn);

	const Vec3&
	operator [](
		const int kn) const;

	Mat3&
	operator *=(
		const Mat3& km);

	Mat3&
	operator *=(
		const scalar kf);

	Mat3&
	operator /=(
		const scalar kf);

	Mat3&
	operator +=(
		const Mat3& km);

	Mat3&
	operator -=(
		const Mat3& km);

private:

	// Row vectors.
	Vec3 m_av[3];

#ifdef FLATLAND_ENABLE_TEST
	friend
	void
	Test::testMat3();
#endif


};


/*******************************************************************************
*******************************************************************************/
inline
Mat3::Mat3()
{
	m_av[0][0] = 0;
	m_av[0][1] = 0;
	m_av[0][2] = 0;
	m_av[1][0] = 0;
	m_av[1][1] = 0;
	m_av[1][2] = 0;
	m_av[2][0] = 0;
	m_av[2][1] = 0;
	m_av[2][2] = 0;
}


/*******************************************************************************
*******************************************************************************/
inline
Mat3::Mat3(
	const Vec3& kvRow0,
	const Vec3& kvRow1,
	const Vec3& kvRow2)
{
	m_av[0] = kvRow0;
	m_av[1] = kvRow1;
	m_av[2] = kvRow2;
}


/*******************************************************************************
*******************************************************************************/
inline
Mat3::Mat3(
	const scalar kf00,
	const scalar kf01,
	const scalar kf02,
	const scalar kf10,
	const scalar kf11,
	const scalar kf12,
	const scalar kf20,
	const scalar kf21,
	const scalar kf22)
{
	m_av[0][0] = kf00;
	m_av[0][1] = kf01;
	m_av[0][2] = kf02;
	m_av[1][0] = kf10;
	m_av[1][1] = kf11;
	m_av[1][2] = kf12;
	m_av[2][0] = kf20;
	m_av[2][1] = kf21;
	m_av[2][2] = kf22;
}


/*******************************************************************************
*******************************************************************************/
inline
Vec3&
Mat3::operator [](
	const int kn)
{
	return m_av[kn];
}


/*******************************************************************************
*******************************************************************************/
inline
const Vec3&
Mat3::operator [](
	const int kn) const
{
	return m_av[kn];
}


/*******************************************************************************
	Moved below to allow inlining.
  
	Mat3&
	Mat3::operator *=(
		const Mat3&)
*******************************************************************************/


/*******************************************************************************
*******************************************************************************/
inline
Mat3&
Mat3::operator *=(
	const scalar kf)
{
	m_av[0] *= kf;
	m_av[1] *= kf;
	m_av[2] *= kf;
	return *this;
}


/*******************************************************************************
*******************************************************************************/
inline
Mat3&
Mat3::operator /=(
	const scalar kf)
{
	m_av[0] /= kf;
	m_av[1] /= kf;
	m_av[2] /= kf;
	return *this;
}


/*******************************************************************************
*******************************************************************************/
inline
Mat3&
Mat3::operator +=(
	const Mat3& km)
{
	m_av[0][0] += km.m_av[0][0];
	m_av[0][1] += km.m_av[0][1];
	m_av[0][2] += km.m_av[0][2];
	m_av[1][0] += km.m_av[1][0];
	m_av[1][1] += km.m_av[1][1];
	m_av[1][2] += km.m_av[1][2];
	m_av[2][0] += km.m_av[2][0];
	m_av[2][1] += km.m_av[2][1];
	m_av[2][2] += km.m_av[2][2];
	return *this;
}


/*******************************************************************************
*******************************************************************************/
inline
Mat3&
Mat3::operator -=(
	const Mat3& km)
{
	m_av[0][0] -= km.m_av[0][0];
	m_av[0][1] -= km.m_av[0][1];
	m_av[0][2] -= km.m_av[0][2];
	m_av[1][0] -= km.m_av[1][0];
	m_av[1][1] -= km.m_av[1][1];
	m_av[1][2] -= km.m_av[1][2];
	m_av[2][0] -= km.m_av[2][0];
	m_av[2][1] -= km.m_av[2][1];
	m_av[2][2] -= km.m_av[2][2];
	return *this;
}


/*******************************************************************************
	Unary minus.
*******************************************************************************/
inline
Mat3
operator -(
	const Mat3& km)
{
	return
		Mat3(
			-km[0][0], -km[0][1], -km[0][2],
			-km[1][0], -km[1][1], -km[1][2],
			-km[2][0], -km[2][1], -km[2][2]);
}


/*******************************************************************************
*******************************************************************************/
inline
Mat3
operator *(
	const Mat3& km1,
	const Mat3& km2)
{
	return
		Mat3(
			km1[0][0]*km2[0][0] + km1[0][1]*km2[1][0] + km1[0][2]*km2[2][0],
			km1[0][0]*km2[0][1] + km1[0][1]*km2[1][1] + km1[0][2]*km2[2][1],
			km1[0][0]*km2[0][2] + km1[0][1]*km2[1][2] + km1[0][2]*km2[2][2],
			km1[1][0]*km2[0][0] + km1[1][1]*km2[1][0] + km1[1][2]*km2[2][0],
			km1[1][0]*km2[0][1] + km1[1][1]*km2[1][1] + km1[1][2]*km2[2][1],
			km1[1][0]*km2[0][2] + km1[1][1]*km2[1][2] + km1[1][2]*km2[2][2],
			km1[2][0]*km2[0][0] + km1[2][1]*km2[1][0] + km1[2][2]*km2[2][0],
			km1[2][0]*km2[0][1] + km1[2][1]*km2[1][1] + km1[2][2]*km2[2][1],
			km1[2][0]*km2[0][2] + km1[2][1]*km2[1][2] + km1[2][2]*km2[2][2]);
}


/*******************************************************************************
*******************************************************************************/
inline
Vec3
operator *(
	const Mat3& km,
	const Vec3& kv)
{
	return
		Vec3(
			km[0][0]*kv[0] + km[0][1]*kv[1] + km[0][2]*kv[2],
			km[1][0]*kv[0] + km[1][1]*kv[1] + km[1][2]*kv[2],
			km[2][0]*kv[0] + km[2][1]*kv[1] + km[2][2]*kv[2]);
}


/*******************************************************************************
*******************************************************************************/
inline
Vec3
operator *(
	const Vec3& kv,
	const Mat3& km)
{
	return
		Vec3(
			kv[0]*km[0][0] + kv[1]*km[1][0] + kv[2]*km[2][0],
			kv[0]*km[0][1] + kv[1]*km[1][1] + kv[2]*km[2][1],
			kv[0]*km[0][2] + kv[1]*km[1][2] + kv[2]*km[2][2]);
}


/*******************************************************************************
*******************************************************************************/
inline
Mat3
operator *(
	const Mat3& km,
	const scalar kf)
{
	return Mat3(km) *= kf;
}


/*******************************************************************************
*******************************************************************************/
inline
Mat3
operator *(
	const scalar kf,
	const Mat3& km)
{
	// Commutative.
	return Mat3(km) *= kf;
}


/*******************************************************************************
*******************************************************************************/
inline
Mat3
operator /(
	const Mat3& km,
	const scalar kf)
{
	return Mat3(km) /= kf;
}


/*******************************************************************************
*******************************************************************************/
inline
Mat3
operator +(
	const Mat3& km1,
	const Mat3& km2)
{
	return Mat3(km1) += km2;
}


/*******************************************************************************
*******************************************************************************/
inline
Mat3
operator -(
	const Mat3& km1,
	const Mat3& km2)
{
	return Mat3(km1) -= km2;
}


/*******************************************************************************
*******************************************************************************/
inline
bool
operator ==(
	const Mat3& km1,
	const Mat3& km2)
{
	return
		km1[0][0] == km2[0][0] &&
		km1[0][1] == km2[0][1] &&
		km1[0][2] == km2[0][2] &&
		km1[1][0] == km2[1][0] &&
		km1[1][1] == km2[1][1] &&
		km1[1][2] == km2[1][2] &&
		km1[2][0] == km2[2][0] &&
		km1[2][1] == km2[2][1] &&
		km1[2][2] == km2[2][2];
}


/*******************************************************************************
*******************************************************************************/
inline
bool
operator !=(
	const Mat3& km1,
	const Mat3& km2)
{
	return
		km1[0][0] != km2[0][0] &&
		km1[0][1] != km2[0][1] &&
		km1[0][2] != km2[0][2] &&
		km1[1][0] != km2[1][0] &&
		km1[1][1] != km2[1][1] &&
		km1[1][2] != km2[1][2] &&
		km1[2][0] != km2[2][0] &&
		km1[2][1] != km2[2][1] &&
		km1[2][2] != km2[2][2];
}


/*******************************************************************************
	Moved from above to allow inlining.
*******************************************************************************/
inline
Mat3&
Mat3::operator *=(
	const Mat3& km)
{
	return *this = *this * km;
}


/*******************************************************************************
	Convert from homogeneous coordinates.
*******************************************************************************/
inline
Vec2
dehomo(
	const Vec3& kv)
{
	return Vec2(kv[0], kv[1]);
}


/*******************************************************************************
	Convert to homogeneous coordinates.
*******************************************************************************/
inline
Vec3
homo(
	const Vec2& kv)
{
	return Vec3(kv[0], kv[1], 1);
}


#endif // GUARD