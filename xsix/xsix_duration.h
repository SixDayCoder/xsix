#pragma once

#include "xsix/xsix_common_define.h"
#include <time.h>

#if defined(_XSIX_WINDOWS)
	#include <windows.h>
	#include <WinSock2.h>
	
	namespace xsix
	{
		int gettimeofday(struct timeval * tp, struct timezone * tzp)
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
	}

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


namespace xsix
{

	class Duration
	{
	public:

		static const int64_t kNanosecond = 1;
		static const int64_t kMicrosecond = 1000;
		static const int64_t kMilliseoncd = 1000 * kMicrosecond;
		static const int64_t kSecond = 1000 * kMilliseoncd;
		static const int64_t kMinute = 60 * kSecond;
		static const int64_t kHour = 60 * kMinute;

	public:

		Duration() : m_ns(0) {}

		explicit Duration(const struct timeval& t) : m_ns(t.tv_sec * kSecond + t.tv_usec * kMicrosecond) {}

		explicit Duration(int64_t nanoseconds) : m_ns(nanoseconds) {}

		explicit Duration(int32_t nanoseconds) : m_ns(nanoseconds) {}

		int64_t Nanoseconds() const { return m_ns;}

		int64_t Seconds() const { return m_ns * kSecond; }

		int64_t Milliseconds() const { return m_ns * kMilliseoncd;}

		int64_t Microseconds() const { return m_ns * kMicrosecond; }

		int64_t Minutes() const { return m_ns * kMinute; }

		int64_t Hours() const { return m_ns * kHour; }

		struct timeval TimeVal() const;

		void To(struct timeval* t) const;

		bool IsZero() const { return m_ns == 0; }

		bool operator <  (const Duration& rhs) const { return m_ns < rhs.m_ns; }

		bool operator <= (const Duration& rhs) const { return m_ns <= rhs.m_ns; }

		bool operator >  (const Duration& rhs) const { return m_ns > rhs.m_ns; }

		bool operator >= (const Duration& rhs) const { return m_ns >= rhs.m_ns; }

		bool operator == (const Duration& rhs) const { return m_ns == rhs.m_ns; }

		Duration operator += (const Duration& rhs);

		Duration operator -= (const Duration& rhs);

	private:
		int64_t m_ns; // nanoseconds
	};

	struct timeval Duration::TimeVal() const
	{
		struct timeval t;
		To(&t);
		return t;
	}

	void Duration::To(struct timeval* t) const
	{
		t->tv_sec = static_cast<long>(m_ns / kSecond);
		t->tv_usec = static_cast<long> (m_ns % kSecond) / static_cast<long>(kMicrosecond);
	}

	xsix::Duration Duration::operator-=(const Duration& rhs)
	{
		m_ns -= rhs.m_ns;
		return *this;
	}

	xsix::Duration Duration::operator+=(const Duration& rhs)
	{
		m_ns += rhs.m_ns;
		return *this;
	}

}