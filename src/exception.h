// Flatland.
// Copyright (C) 2003 Marc A. Lepage.


#ifndef GUARD__EXCEPTION
#define GUARD__EXCEPTION


#include <csetjmp>
#include "config.h"


#define TRY \
	{ \
		++Exception::m_nDepth; \
		switch (setjmp(Exception::m_aEnv[Exception::m_nDepth])) \
		{ \
			case 0: \
			{


#define CATCH(nCode) \
				break; \
			} \
			case nCode: \
			{


#define CATCHALL \
				break; \
			} \
			default: \
			{ \


#define END_TRY_CATCHALL \
				break; \
			} \
		} \
		--Exception::m_nDepth; \
	}


#define END_TRY_CATCH \
	CATCHALL \
	{ \
		RETHROW; \
	} \
	END_TRY_CATCHALL


#define THROW(nCode) \
	longjmp(Exception::m_aEnv[Exception::m_nDepth], Exception.m_nCode = nCode)


#define RETHROW \
	longjmp(Exception::m_aEnv[--Exception::m_nDepth], Exception.m_nCode)


/*******************************************************************************
	Exception handling. Usage:

	TRY
	{
		TRY
		{
			THROW(Exception::CodeB);
		}
		CATCH (Exception::CodeB)
		{
			RETHROW;
		}
		END_TRY_CATCH
	}
	CATCH (Exception::CodeA)
	{
	}
	CATCHALL
	{
	}
	END_TRY_CATCHALL
*******************************************************************************/
class Exception
{


public:

	enum
	{
		Parse = 1,
		OpenFile
	};


public:

	static
	void
	test();

	static
	jmp_buf m_aEnv[FLATLAND_EXCEPTION_MAX];

	static
	int m_nDepth;

	static
	int m_nCode;


};


#endif // GUARD
