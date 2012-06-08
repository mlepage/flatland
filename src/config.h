// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__CONFIG
#define GUARD__CONFIG


#include <limits>


/*******************************************************************************
	Define this macro to turn on logging.
*******************************************************************************/
#define FLATLAND_ENABLE_LOG


/*******************************************************************************
	Define this macro to turn on unit testing.
*******************************************************************************/
#define FLATLAND_ENABLE_TEST


/*******************************************************************************
	Define this macro to turn on the max distance in effective radius.
*******************************************************************************/
//#define FLATLAND_ENABLE_EFFECTIVE_RADIUS_MAX


/*******************************************************************************
	Controls whether geometry's transform functions are optimized.
*******************************************************************************/
#define FLATLAND_OPTIMIZE_TRANSFORMS


/*******************************************************************************
	Controls the floating point tolerance in makeEffectiveRadius for deciding
	whether radius is to an edge (instead of just to a vertex).

	FLT_EPSILON is enough for PC, but not for Pocket PC.
*******************************************************************************/
//#define FLATLAND_TOL_EDGE std::numeric_limits<scalar>::epsilon()
#define FLATLAND_TOL_EDGE 1e-6f


/*******************************************************************************
	Console configuration.

	Print buffer size is in characters.
	Text line size is in characters.
	Text buffer size is in text lines.
*******************************************************************************/
#define FLATLAND_CONSOLE_PRINT_BUFFER_SIZE 1024
#define FLATLAND_CONSOLE_TEXT_LINE_SIZE 48
#define FLATLAND_CONSOLE_TEXT_BUFFER_SIZE 100


#endif // GUARD
