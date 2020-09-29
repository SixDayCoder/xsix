#pragma once

#include <stdint.h>
#include <assert.h>
#include <string>
#include <vector>

//platform define
#if ( defined(_WIN32) || defined(_WIN64) )
	#ifndef _XSIX_WINDOWS
		#define _XSIX_WINDOWS
	#endif
#endif

#if defined _XSIX_WINDOWS
	#pragma warning(disable:4996)
#endif

namespace xsix
{
	//assert define
	#define XASSERT(expression) assert(expression)

	//memory define
	#define XMALLOC  malloc
	#define XFREE    free
	#define XREALLOC realloc

	using string_vec = std::vector<std::string>;
}