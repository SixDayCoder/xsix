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

	public:

		int64_t seconds() const { return m_ns / kSecond; }

		int64_t millseconds() const { return m_ns / kMilliseoncd;}

		int64_t microseconds() const { return m_ns / kMicrosecond; }

		int64_t nanoseconds() const { return m_ns; }

		int64_t minutes() const { return m_ns / kMinute; }

		int64_t hours() const { return m_ns / kHour; }

	public:

		struct timeval get_time_val() const
		{
			struct timeval t;
			make_time_val(&t);
			return t;
		}

		void make_time_val(struct timeval* t) const
		{
			t->tv_sec = static_cast<long>(m_ns / kSecond);
			t->tv_usec = static_cast<long> (m_ns % kSecond) / static_cast<long>(kMicrosecond);
		}

	public:

		bool operator <  (const Duration& rhs) const { return m_ns < rhs.m_ns; }

		bool operator <= (const Duration& rhs) const { return m_ns <= rhs.m_ns; }

		bool operator >  (const Duration& rhs) const { return m_ns > rhs.m_ns; }

		bool operator >= (const Duration& rhs) const { return m_ns >= rhs.m_ns; }

		bool operator == (const Duration& rhs) const { return m_ns == rhs.m_ns; }

	public:

		Duration operator += (const Duration& rhs)
		{
			m_ns += rhs.m_ns;
			return *this;
		}

		Duration operator -= (const Duration& rhs)
		{
			m_ns -= rhs.m_ns;
			return *this;
		}

	private:

		int64_t m_ns; // nanoseconds

	};
}