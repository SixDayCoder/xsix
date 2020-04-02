#include "xsix/xsix_timestamp.h"
#include <stdio.h>

namespace xsix
{
	xsix::Timestamp Timestamp::Now()
	{
		return Timestamp(int64_t(utcmicrosecond() * Duration::kMicrosecond));
	}

	void Timestamp::PrintNow()
	{
		Timestamp t = Timestamp::Now();
		printf("now : %lld\n", t.UnixMills());
	}

	struct timeval Timestamp::TimeVal() const
	{
		struct timeval t;
		To(&t);
		return t;
	}

	void Timestamp::To(struct timeval* t) const
	{
		t->tv_sec = (long)(m_ns / Duration::kSecond);
		t->tv_usec = (long)(m_ns % Duration::kSecond) / (long)Duration::kMicrosecond;
	}

	inline void Timestamp::Add(Duration d)
	{
		m_ns += d.Nanoseconds();
	}

	xsix::Timestamp Timestamp::operator+(const Duration& rhs) const
	{
		Timestamp temp(*this);
		temp += rhs;
		return temp;
	}

	xsix::Duration Timestamp::operator-(const Timestamp& rhs) const
	{
		return Duration(m_ns - rhs.m_ns);
	}

	xsix::Timestamp Timestamp::operator-(const Duration& rhs) const
	{
		Timestamp temp(*this);
		temp -= rhs;
		return temp;
	}

	xsix::Timestamp Timestamp::operator-=(const Duration& rhs)
	{
		m_ns -= rhs.Nanoseconds();
		return *this;
	}

	xsix::Timestamp Timestamp::operator+=(const Duration& rhs)
	{
		m_ns += rhs.Nanoseconds();
		return *this;
	}
}