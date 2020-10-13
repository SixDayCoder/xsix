#pragma once

namespace xsix
{
	class INonCopyable
	{
	public:

		INonCopyable(const INonCopyable&) = delete;
		void operator=(const INonCopyable&) = delete;

	protected:

		INonCopyable() = default;
		~INonCopyable() = default;
	};
}