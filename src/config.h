// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__CONFIG
#define GUARD__CONFIG


#include <limits>


#define FLATLAND_PARSE_MAX 128

#define FLATLAND_VARIABLE_NAME_MAX 64
#define FLATLAND_VARIABLE_VALUE_MAX 256

#define FLATLAND_FILENAME_MAX 1024


/*******************************************************************************
	The maximum length of a name.
*******************************************************************************/
#define FLATLAND_MAX_NAME 32


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
	Define this macro to turn on custom line clipping.
*******************************************************************************/
#define FLATLAND_ENABLE_CUSTOM_LINE_CLIPPING


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
#if 1
#define FLATLAND_TOL_EDGE 1e-6f
#else
// For fixed point 16.16, 1e-4f is enough to pass unit tests on PC.
#define FLATLAND_TOL_EDGE 1e-3f
#endif


/*******************************************************************************
	Console configuration.

	Print buffer size is in characters.
	Text line size is in characters.
	Text buffer size is in text lines.
*******************************************************************************/
#define FLATLAND_CONSOLE_PRINT_BUFFER_SIZE 1024
#define FLATLAND_CONSOLE_TEXT_LINE_SIZE 48
#define FLATLAND_CONSOLE_TEXT_BUFFER_SIZE 100


#define FLATLAND_PARSER_LINE_SIZE 512

#define FLATLAND_EXCEPTION_MAX 32


#endif // GUARD
