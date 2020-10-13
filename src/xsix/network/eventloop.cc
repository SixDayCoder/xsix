#include "xsix/network/eventloop.h"
#include "xsix/network/poller.h"
#include "xsix/network/poller_select.h"

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
				//handle event
			}
			m_curr_active_channel = nullptr;
			m_handling = false;

		}
	}

	void EventLoop::quit()
	{
		m_quit = true;
	}

	void EventLoop::update_channel(Channel* channel)
	{

	}

	void EventLoop::remove_channel(Channel* channel)
	{

	}

	bool EventLoop::has_channel(Channel* channel)
	{
		return false;
	}

}