#include "actor_base.h"
#include <cstdio>

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

		printf("total msg count : %d, handle count : %d\n", msglist.size(), handle_count);
	}

}