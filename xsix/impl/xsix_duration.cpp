#include "xsix/xsix_duration.h"

namespace xsix
{
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