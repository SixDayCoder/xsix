#pragma once

#include "xsix/xsix_common_define.h"
#include "xsix/xsix_duration.h"


namespace xsix
{
	class Timestamp 
	{
	public:

		static Timestamp Now();

	public:

		Timestamp() : m_ns(0) {}

		explicit Timestamp(int64_t nanoseconds) : m_ns(nanoseconds) {}

		explicit Timestamp(const struct timeval& t) : m_ns(t.tv_sec * Duration::kSecond + t.tv_usec * Duration::kMicrosecond) {}

		struct timeval TimeVal() const;

		void To(struct timeval* t) const;

		int64_t Unix() const;

		int64_t UnixNano() const;

		int64_t UnixMills() const;

		void Add(Duration d);

		bool IsEpoch() const { return m_ns == 0; }

	public:

		bool operator < (const Timestamp& rhs) const  { return m_ns < rhs.m_ns; }

		bool operator == (const Timestamp& rhs) const { return m_ns == rhs.m_ns; }

		Timestamp operator += (const Duration& rhs);

		Timestamp operator + (const Duration& rhs) const;

		Timestamp operator -= (const Duration& rhs);

		Timestamp operator - (const Duration& rhs) const;

		Duration  operator - (const Timestamp& rhs) const;

	private:

		int64_t m_ns;
	};

	xsix::Timestamp Timestamp::Now()
	{
		return Timestamp(int64_t(utcmicrosecond() * Duration::kMicrosecond));
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

	inline int64_t Timestamp::Unix() const
	{
		return m_ns / Duration::kSecond;
	}

	inline int64_t Timestamp::UnixNano() const
	{
		return m_ns;
	}

	inline int64_t Timestamp::UnixMills() const
	{
		return m_ns / Duration::kMilliseoncd;
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