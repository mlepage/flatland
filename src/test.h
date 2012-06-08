// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__TEST
#define GUARD__TEST


#include "config.h"


#ifdef FLATLAND_ENABLE_TEST


/*******************************************************************************
	Unit testing.
*******************************************************************************/
class Test
{


public:

	static
	void
	test();

	static
	void
	testGeometry();

	static
	void
	testMat3();

	static
	void
	testVec2();

	static
	void
	testVec3();

	static
	void
	testWorld();


private:


};


#endif


#endif // GUARD
