#include <cstdio>

#include "actor_base.h"
#include "xsix/time/timestamp.h"


namespace xsix
{

	void ActorBase::tick()
	{
		set_state(STATE_TICKING);
		handle_all_msg();
	}

	void ActorBase::init()
	{
		set_state(STATE_READY_TICK);
	}

	bool ActorBase::ready_tick(int64_t ts)
	{
		if (get_state() != STATE_READY_TICK) 
		{
			return false;
		}

		auto interval = ts - get_last_tick_timestamp();
		if (interval < get_tick_interval())
		{
			return false;
		}

		return true;
	}

	void ActorBase::addmsg(ActorMsgPtr msgptr)
	{
		std::lock_guard<std::mutex> lock(m_msg_vec_mutex);
		m_msg_vec.push_back(msgptr);
	}

	void ActorBase::handle_all_msg()
	{
		std::vector<ActorMsgPtr> msglist;

		m_msg_vec_mutex.lock();
		msglist.assign(m_msg_vec.begin(), m_msg_vec.end());
		m_msg_vec_mutex.unlock();

		int32_t handle_count = 0;
		for (auto msgptr : msglist)
		{
			if (msgptr)
			{	
				msgptr->handle_msg(*this);
				handle_count++;
			}
		}
	}

}