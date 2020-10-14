#pragma once

#include "xsix/noncopyable.h"
#include "xsix/time/timestamp.h"
#include <vector>

namespace xsix
{

	class Channel;

	class Poller;

	class EventLoop : INonCopyable
	{
	public:

		EventLoop();

		~EventLoop();

	public:

		void run();

		void quit();

	public:

		void update_channel(Channel* channel);

		void remove_channel(Channel* channel);

		bool has_channel(Channel* channel);

	private:

		void run_tick_after_handle_event();

	private:

		bool				  m_running;
		bool				  m_quit;
		bool				  m_handling;

		std::vector<Channel*> m_total_channel_list;
		std::vector<Channel*> m_active_channel_list;
		Channel*			  m_curr_active_channel;

		Poller*				  m_poller;

	};

}
