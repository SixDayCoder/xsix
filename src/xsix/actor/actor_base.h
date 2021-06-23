#pragma once

#include "actor_message.h"
#include <vector>
#include <mutex>
#include <atomic>

namespace xsix
{
	class ActorCore;

	class ActorBase;
	using ActorBasePtr = std::shared_ptr<ActorBase>;

	class ActorBase
	{
	public:

		enum
		{
			STATE_READY_TICK = 0,
			STATE_TICKING = 1,
		};

	public:

		friend class ActorCore;

	public:

		ActorBase(int32_t id = 0) : m_id(id), m_state(STATE_READY_TICK) {}
		virtual ~ActorBase() {}
		
	public:

		void	set_id(int32_t id) { m_id = id; }
		int32_t get_id() const { return m_id; }

		void	set_state(int32_t state) { m_state.store(state); }
		int32_t	get_state() const { return (int32_t)m_state.load(); }

	public:

		virtual int32_t get_tick_interval() const { return 100; }

		virtual void tick();

		virtual void init();

	public:

		bool ready_tick(int64_t ts);

	public:

		void addmsg(ActorMsgPtr msgptr);

	private:

		void handle_all_msg();

	public:

		int64_t get_last_tick_timestamp() const { return m_last_tick_timestamp; }
		void	set_last_tick_timestamp(int64_t ts) { m_last_tick_timestamp = ts; }

	private:

		int32_t					 m_id = -1;
		std::atomic_int			 m_state;

		std::mutex				 m_msg_vec_mutex;
		std::vector<ActorMsgPtr> m_msg_vec;

		int64_t					 m_last_tick_timestamp;
	};
}