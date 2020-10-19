#pragma once

#include "xsix/noncopyable.h"
#include "xsix/time/timestamp.h"
#include "xsix/network/callback_define.h"
#include <thread>
#include <vector>
#include <mutex>

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

		//当前线程是否在事件循环loop
		bool is_currthread_in_loopthread() { return m_eventloop_thread_id == std::this_thread::get_id(); }

		//在事件循环loop的线程执行func
		void run_in_loop(EventLoopFunc func);

		//加入等待队列,等待事件循环执行func
		void queue_in_loop(EventLoopFunc func);

		//等待执行队列的func个数
		int  pending_queue_size();

	public:

		void update_channel(Channel* channel);

		void remove_channel(Channel* channel);

		bool has_channel(Channel* channel);

	private:

		void run_after_handle_event();

		void run_pending_func();

	private:

		std::thread::id		  m_eventloop_thread_id;

		bool				  m_running;
		bool				  m_quit;

		std::vector<Channel*> m_total_channel_list;
		std::vector<Channel*> m_active_channel_list;
		Channel*			  m_curr_active_channel;

		Poller*				  m_poller;

		std::vector<EventLoopFunc>	m_pending_func;
		std::mutex					m_pending_func_mtx;
	};

}
