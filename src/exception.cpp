// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#include "exception.h"


jmp_buf
Exception::m_aEnv[FLATLAND_EXCEPTION_MAX];

int
Exception::m_nDepth = -1;

int
Exception::m_nCode;
