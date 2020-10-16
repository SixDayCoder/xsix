#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"
#include "xsix/time/timestamp.h"
#include "xsix/network/callback_define.h"

namespace xsix
{

	class EventLoop;


	class Channel : public INonCopyable
	{
	public:

		enum Event
		{
			None  = 0,
			Read  = 1,
			Write = 2,
		};

	public:

		Channel(EventLoop* eventloop, int fd);

		~Channel();

	public:

		void	set_read_cb(ChannelEventCallBack cb) { m_read_cb = std::move(cb); }

		void	set_write_cb(ChannelEventCallBack cb) { m_write_cb = std::move(cb); }

		void	set_close_cb(ChannelEventCallBack cb) { m_close_cb = std::move(cb); }

		void	set_error_cb(ChannelEventCallBack cb) { m_error_cb = std::move(cb); }

		void	set_event(int e) { m_events = e; }

		void    handle_event();

	public:

		void	enable_read();

		void	disable_read();

		void	enable_write();

		void	disable_write();

		bool	is_reading() const { return m_events & Event::Read; }

		bool	is_writing() const { return m_events & Event::Write; }

	public:

		EventLoop*	get_owner_eventloop() { return m_eventloop; }

		void		update_to_eventloop();

		void		remove_from_eventloop();

	public:

		const int	get_fd() const { return m_fd; }

	public:

		EventLoop*				m_eventloop;

		const int				m_fd;

		int						m_events;

		ChannelEventCallBack	m_read_cb;

		ChannelEventCallBack	m_write_cb;

		ChannelEventCallBack	m_close_cb;

		ChannelEventCallBack	m_error_cb;
	};

}