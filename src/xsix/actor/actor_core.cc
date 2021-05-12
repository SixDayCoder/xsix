#include "actor_core.h"
#include <assert.h>

namespace xsix
{

	bool _should_tick(ActorBasePtr ptr, int64_t ts)
	{
		if (ptr->get_state() != ActorBase::STATE_READY_TICK)
		{
			return false;
		}
		auto interval = ts - ptr->get_last_tick_timestamp();
		if (interval < ptr->get_tick_interval())
		{
			return false;
		}
		return true;
	}

	void ActorCore::register_actor(ActorBasePtr actor_ptr)
	{
		if (actor_ptr)
		{
			auto it = m_actor_map.find(actor_ptr->get_id());
			if (it == m_actor_map.end())
			{
				m_actor_map.insert(std::make_pair(actor_ptr->get_id(), actor_ptr));
			}
		}
	}

	void ActorCore::run()
	{
		while (true)
		{

			auto now = std::chrono::system_clock::now();
			auto ts = std::chrono::duration_cast<std::chrono::milliseconds>(
				now.time_since_epoch()
			).count();
			for (auto it = m_actor_map.begin(); it != m_actor_map.end(); ++it)
			{
				ActorBasePtr actor_ptr = it->second;
				assert(actor_ptr);
				if (_should_tick(actor_ptr, ts))
				{
					actor_ptr->set_state(ActorBase::STATE_TICKING);
					actor_ptr->set_last_tick_timestamp(ts);
					asio::post(m_thread_pool, std::bind(&ActorBase::tick, actor_ptr));
				}
			}
		}
	}

}