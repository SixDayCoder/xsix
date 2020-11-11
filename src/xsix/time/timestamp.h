#pragma once

#include "xsix/time/duration.h"
#include "xsix/fmt.hpp"

namespace xsix
{
	class Timestamp
	{
	public:

		static Timestamp now()
		{
			return Timestamp(int64_t(utcmicrosecond() * Duration::kMicrosecond));
		}

	public:

		Timestamp() : m_ns(0) {}

		explicit Timestamp(int64_t nanoseconds) : m_ns(nanoseconds) {}

		explicit Timestamp(const struct timeval& t) : m_ns(t.tv_sec* Duration::kSecond + t.tv_usec * Duration::kMicrosecond) {}

	public:

		struct timeval get_timeval() const
		{
			struct timeval t;
			make_time_val(&t);
			return t;
		}

		void make_time_val(struct timeval* t) const
		{
			t->tv_sec = (long)(m_ns / Duration::kSecond);
			t->tv_usec = (long)(m_ns % Duration::kSecond) / (long)Duration::kMicrosecond;
		}

		std::string yymmhhmmss() const
		{
			time_t ts = unix();
			struct tm *t = localtime(&ts);

			char buffer[128] = { 0 };
			strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", t);

			return std::string(buffer);
		}

	public:

		void	add(Duration d) { m_ns += d.nanoseconds(); }

		int64_t	unix() const { return m_ns / Duration::kSecond; }

		int64_t	unixnano() const { return m_ns; }

		int64_t	unixmills() const { return m_ns / Duration::kMilliseoncd; }

	public:

		bool operator < (const Timestamp& rhs) const { return m_ns < rhs.m_ns; }

		bool operator == (const Timestamp& rhs) const { return m_ns == rhs.m_ns; }

	public:

		Timestamp operator += (const Duration& rhs)
		{
			m_ns += rhs.nanoseconds();
			return *this;
		}

		Timestamp operator + (const Duration& rhs) const
		{
			Timestamp tmp(*this);
			tmp += rhs;
			return tmp;
		}

		Timestamp operator -= (const Duration& rhs)
		{
			m_ns -= rhs.nanoseconds();
			return *this;
		}

		Timestamp operator - (const Duration& rhs) const
		{
			Timestamp tmp(*this);
			tmp -= rhs;
			return tmp;
		}

		Duration  operator - (const Timestamp& rhs) const
		{
			return Duration(m_ns - rhs.m_ns);
		}

	private:

		int64_t m_ns;
	};
}