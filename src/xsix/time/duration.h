#pragma once

#include "xsix/common_define.h"
#include "xsix/time/gettimeofday.h"
#include <time.h>

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
}