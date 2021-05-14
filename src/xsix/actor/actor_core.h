#pragma once

#include <vector>
#include <mutex>
#include <atomic>
#include <map>
#include <chrono>
#include <functional>

#include "actor_base.h"
#include "asio.hpp"
#include "asio/thread_pool.hpp"

namespace xsix
{
	class ActorCore : public std::enable_shared_from_this<ActorCore>
	{
	public:

		ActorCore() : m_thread_pool(std::thread::hardware_concurrency()) {}

	public:

		void register_actor(ActorBasePtr actor_ptr);
		void run();

	private:
	
		std::map<int32_t, ActorBasePtr> m_actor_map;
		asio::thread_pool				m_thread_pool;
	};
}
