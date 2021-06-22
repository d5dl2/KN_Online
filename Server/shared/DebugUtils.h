#pragma once

void FormattedDebugString(const char * fmt, ...);

// TODO: Shift this logic into a separate header and generally clean this shared logic up
#ifndef ASSERT
#if defined(_DEBUG)
#define ASSERT assert
#include <assert.h>
#else
#define ASSERT
#endif
#endif


#include "CrtDbg.h"

#ifndef _DEBUG
//#define __ASSERT(expr, expMessage) SDL_assert(expr)
#define __ASSERT(expr, expMessage) \
if(!(expr)) {\
	printf("ERROR-> %s\n%s: %d\n\n", expMessage, __FILE__, __LINE__);\
}
//system("pause");\

#else

#define __ASSERT(expr, expMessage) \
if(!(expr)) {\
	printf("ERROR-> %s\n%s: %d\n\n", expMessage, __FILE__, __LINE__);\
}
//system("pause");\

/*
#define __ASSERT(expr, expMessage) \
if(!(expr)) \
{ \
	_CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, "N3 Custom Assert Functon", expMessage); \
	char __szErr[512]; \
	sprintf(__szErr, "  ---- N3 Assert Warning (File:%s, Line:%d) ---- \n", __FILE__, __LINE__); \
	OutputDebugString(__szErr); \
	sprintf(__szErr, "    : %s\n", expMessage); \
	OutputDebugString(__szErr); \
	_CrtDbgBreak(); \
}
*/
#endif