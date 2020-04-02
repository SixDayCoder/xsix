#pragma once

#include "xsix/xsix_common_define.h"

#if defined(_XSIX_WINDOWS)

#define _WINSOCKAPI_
#include <windows.h>
#include <WinSock2.h>


	#ifndef H_XSIX_WIN32_GETTIMEOFDAY
	#define H_XSIX_WIN32_GETTIMEOFDAY
	inline int gettimeofday(struct timeval * tp, void* tzp)
	{
		uint64_t  intervals;
		FILETIME  ft;

		GetSystemTimeAsFileTime(&ft);

		intervals = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
		intervals -= 116444736000000000;

		tp->tv_sec = (long)(intervals / 10000000);
		tp->tv_usec = (long)((intervals % 10000000) / 10);

		return 0;
	}
	#endif //XSIX_WIN32_GETTIMEOFDAY

#endif

#if defined(_XSIX_LINUX)
#include <sys/time.h>
#endif

namespace xsix
{
	inline double utcsecond()
	{
		struct timeval tv;
		gettimeofday(&tv, nullptr);
		return (double)(tv.tv_sec) + ((double)(tv.tv_usec)) / 1000000.0f;
	}

	inline uint64_t utcmicrosecond()
	{
		struct timeval tv;
		gettimeofday(&tv, nullptr);
		return (uint64_t)(((uint64_t)(tv.tv_sec)) * 1000000 + tv.tv_usec);
	}

	inline struct timeval timevalconv(uint64_t time_us)
	{
		struct timeval tv;
		tv.tv_sec = (long)time_us / 1000000;
		tv.tv_usec = (long)time_us % 1000000;
		return tv;
	}
}