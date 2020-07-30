#pragma once

#include "xsix/xsix_common_define.h"

namespace xsix
{
	class MathUtils
	{
	public:

		static bool			IsPrimeNumebr(uint64_t n);

		static bool			IsPowerOf2(uint64_t n);
			
		static uint64_t		MinPowerOf2(uint64_t n);

	};
}