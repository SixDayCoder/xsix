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

		enum EventOp
		{
			Enable = 0,
			Disable = 1,
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

		void	enable_read()   { modify_event(Channel::Event::Read, Channel::EventOp::Enable); }

		void	disable_read()  { modify_event(Channel::Event::Read, Channel::EventOp::Disable); }

		void	enable_write()  { modify_event(Channel::Event::Write, Channel::EventOp::Enable); }

		void	disable_write() { modify_event(Channel::Event::Write, Channel::EventOp::Disable); }

		bool	is_enable_read() const { return m_events & Event::Read; }

		bool	is_enable_write() const { return m_events & Event::Write; }

	public:

		int     get_index() const { return m_index; }

		void	set_index(int index) { m_index = index; }

	public:

		EventLoop*	get_owner_eventloop() { return m_eventloop; }

		void		update_to_eventloop();

		void		remove_from_eventloop();

		const int	get_fd() const { return m_fd; }

	private:

		void		modify_event(int event_type, int op);

	private:

		EventLoop*				m_eventloop;

		const int				m_fd;

		int						m_events;

		ChannelEventCallBack	m_read_cb;

		ChannelEventCallBack	m_write_cb;

		ChannelEventCallBack	m_close_cb;

		ChannelEventCallBack	m_error_cb;

	private:

		int						m_index;
	};

}