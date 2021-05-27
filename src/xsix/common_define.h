#pragma once

#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <atomic>
#include <map>
#include <array>
#include <list>
#include <mutex>

//platform define
#if ( defined(_WIN32) || defined(_WIN64) )
	#ifndef _XSIX_WINDOWS
		#define _XSIX_WINDOWS
	#endif
#endif

#if defined _XSIX_WINDOWS
#pragma warning(disable:4996)
#include <winsock2.h>
#endif

#if defined _XSIX_LINUX
#include<netinet/in.h>
#endif

namespace xsix
{
	#define XASSERT(expression) assert(expression)

	#define XMALLOC  malloc
	#define XFREE    free
	#define XREALLOC realloc

	using string_vec = std::vector<std::string>;

	template<typename T>
	inline T* get_pointer(const std::shared_ptr<T>& ptr)
	{
		return ptr.get();
	}

	template<typename T>
	inline T* get_pointer(const std::unique_ptr<T>& ptr)
	{
		return ptr.get();
	}
}