#include "actor_core.h"
#include "xsix/time/timestamp.h"
#include <assert.h>

namespace xsix
{
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
			int64_t now = Timestamp::now().unixmills();
			for (auto it = m_actor_map.begin(); it != m_actor_map.end(); ++it)
			{
				ActorBasePtr actor_ptr = it->second;
				assert(actor_ptr);	
				if (actor_ptr->ready_tick(now))
				{
					actor_ptr->set_state(ActorBase::STATE_TICKING);
					actor_ptr->set_last_tick_timestamp(now);
					asio::post(m_thread_pool, 
						[actor_ptr]() {
							if (actor_ptr)
							{
								actor_ptr->tick();
								actor_ptr->set_state(ActorBase::STATE_READY_TICK);
							}
						}
					);
				}
			}
		}
	}
}