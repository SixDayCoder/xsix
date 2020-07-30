#pragma once

#include "xsix/xsix_duration.h"

namespace xsix
{
	class Timestamp
	{
	public:

		static Timestamp Now();

		static void PrintNow();

	public:

		Timestamp() : m_ns(0) {}

		explicit Timestamp(int64_t nanoseconds) : m_ns(nanoseconds) {}

		explicit Timestamp(const struct timeval& t) : m_ns(t.tv_sec* Duration::kSecond + t.tv_usec * Duration::kMicrosecond) {}

		struct timeval TimeVal() const;

		void To(struct timeval* t) const;

		int64_t Unix() const { return m_ns / Duration::kSecond; }

		int64_t UnixNano() const { return m_ns; }

		int64_t UnixMills() const { return m_ns / Duration::kMilliseoncd; }

		void Add(Duration d);

		bool IsEpoch() const { return m_ns == 0; }

	public:

		bool operator < (const Timestamp& rhs) const { return m_ns < rhs.m_ns; }

		bool operator == (const Timestamp& rhs) const { return m_ns == rhs.m_ns; }

		Timestamp operator += (const Duration& rhs);

		Timestamp operator + (const Duration& rhs) const;

		Timestamp operator -= (const Duration& rhs);

		Timestamp operator - (const Duration& rhs) const;

		Duration  operator - (const Timestamp& rhs) const;

	private:

		int64_t m_ns;
	};
}