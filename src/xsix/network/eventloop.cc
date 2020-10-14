#include "xsix/network/eventloop.h"
#include "xsix/network/poller.h"
#include "xsix/network/poller_select.h"
#include "xsix/network/channel.h"

namespace xsix
{
	
	EventLoop::EventLoop() : 
				m_running(false),
				m_quit(false),
				m_handling(false),
				m_curr_active_channel(nullptr),
				m_poller(new SelectPoller(this))
	{

	}

	EventLoop::~EventLoop()
	{

	}

	void EventLoop::run()
	{
		m_running = true;
		m_quit = false;

		while (!m_quit)
		{
			m_active_channel_list.clear();
			m_poller->poll(1000, &m_active_channel_list);

			m_handling = true;
			for (Channel* channel : m_active_channel_list)
			{
				m_curr_active_channel = channel;
				m_curr_active_channel->handle_event();
			}
			m_curr_active_channel = nullptr;
			m_handling = false;

			run_tick_after_handle_event();
		}
	}

	void EventLoop::quit()
	{
		m_quit = true;
	}

	void EventLoop::update_channel(Channel* channel)
	{
		XASSERT(channel->get_owner_eventloop() == this);
		m_poller->update_channel(channel);
	}

	void EventLoop::remove_channel(Channel* channel)
	{
		XASSERT(channel->get_owner_eventloop() == this);
		m_poller->remove_channel(channel);
	}

	bool EventLoop::has_channel(Channel* channel)
	{
		return m_poller->has_channel(channel);
	}

	void EventLoop::run_tick_after_handle_event()
	{
		printf("timestamp : %lld, active channel cnt : %d\n", 
			xsix::Timestamp::now().unixmills(),
			m_active_channel_list.size()
		);
	}

}