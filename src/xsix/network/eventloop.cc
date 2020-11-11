#include "xsix/network/eventloop.h"
#include "xsix/network/poller.h"
#include "xsix/network/poller_select.h"
#include "xsix/network/poller_poll.h"
#include "xsix/network/channel.h"
#include "xsix/log/log_define.h"

namespace xsix
{
	
	static Poller* __default_poller(EventLoop* eventloop)
	{
#if   defined (_XSIX_WINDOWS)
		return new SelectPoller(eventloop);
#elif defined (_XSIX_LINUX)
		return new PollPoller(eventloop);
#endif
		return new SelectPoller(eventloop);
	}

	EventLoop::EventLoop() : 
				m_eventloop_thread_id(std::this_thread::get_id()),
				m_running(false),
				m_quit(false),
				m_curr_active_channel(nullptr),
				m_poller(__default_poller(this))
	{

	}

	EventLoop::~EventLoop()
	{

	}

	void EventLoop::run()
	{
		m_running = true;
		m_quit = false;

		xsix::log(LOGGER(EventLoop), "eventloop run");

		while (!m_quit)
		{
			m_active_channel_list.clear();
			m_poller->poll(5000, &m_active_channel_list);
			for (Channel* channel : m_active_channel_list)
			{
				m_curr_active_channel = channel;
				m_curr_active_channel->handle_event();
			}
			m_curr_active_channel = nullptr;

			run_after_handle_event();
			run_pending_func();
		}
		
		xsix::log(LOGGER(EventLoop), "eventloop quit");
	}

	void EventLoop::quit()
	{
		m_quit = true;
	}

	void EventLoop::run_in_loop(EventLoopFunc func)
	{
		if (!func)
		{
			return;
		}

		if (is_currthread_in_loopthread())
		{
			func();
		}
		else
		{
			queue_in_loop(std::move(func));
		}
	}

	void EventLoop::queue_in_loop(EventLoopFunc func)
	{
		std::lock_guard<std::mutex> lock(m_pending_func_mtx);
		m_pending_func.emplace_back(std::move(func));
	}

	int EventLoop::pending_queue_size()
	{
		std::lock_guard<std::mutex> lock(m_pending_func_mtx);
		return m_pending_func.size();
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

	void EventLoop::run_after_handle_event()
	{
		LOGGER_MANAGER_FLUSH;
	}

	void EventLoop::run_pending_func()
	{
		std::vector<EventLoopFunc> funcs;
		{
			std::lock_guard<std::mutex> lock(m_pending_func_mtx);
			funcs.swap(m_pending_func);
		}

		for (const EventLoopFunc& func : funcs)
		{
			func();
		}

		//xsix::log(LOGGER(EventLoop), "run_pending_func pending_func_size : %",
		//	xsix::Timestamp::now().unixmills(),
		//	funcs.size()
		//);
	}

}