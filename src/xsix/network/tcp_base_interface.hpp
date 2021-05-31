#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"

#include "asio.hpp"

namespace xsix
{
	namespace net
	{	
		class ITCPConnectionParent : public INonCopyable
		{	
		public:

			virtual void broadcast(const char* msg, std::size_t msgsize) = 0;
		};
	}
}
