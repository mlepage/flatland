#ifndef __MYAPP_INCLUDED__
#define __MYAPP_INCLUDED__

#pragma once

#include "GapiApplication.h"

//-----------------------------------------------------------------------------
// Name: class CMyApplication
// Desc: Main class to run this application. Most functionality is inherited
//       from the CGapiApplication base class.
//-----------------------------------------------------------------------------
class CMyApplication : public CGapiApplication
{
// Construction
public:
	CMyApplication(const GDAPPCONFIG& config);

// Attributes
protected:

// Operations
protected:

// Implementation
public:
	virtual HRESULT InitInstance();
	virtual HRESULT ExitInstance();
	virtual HRESULT CreateSurfaces(CGapiDisplay& display, HINSTANCE hInstance);
	virtual HRESULT ProcessNextFrame(CGapiSurface& backbuffer, DWORD dwFlags);

	virtual HRESULT KeyDown(DWORD dwKey, GDKEYLIST& keylist);
	virtual HRESULT KeyUp(DWORD dwKey, GDKEYLIST& keylist);
	virtual HRESULT StylusDown(POINT p);
	virtual HRESULT StylusUp(POINT p);
	virtual HRESULT StylusDblClk(POINT p);
	virtual HRESULT StylusMove(POINT p);

	virtual ~CMyApplication();
};

#endif // __MYAPP_INCLUDED__
