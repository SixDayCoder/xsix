#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"
#include "xsix/time/timestamp.h"

#include <functional>

namespace xsix
{

	class EventLoop;


	class Channel : public INonCopyable
	{
	public:

		using EventCallBack = std::function<void()>;

	public:

		Channel(EventLoop* eventloop, int fd);

		~Channel();

	public:

		void	set_read_cb(EventCallBack cb) { m_read_cb = std::move(cb); }

		void	set_write_cb(EventCallBack cb) { m_write_cb = std::move(cb); }

		void	set_close_cb(EventCallBack cb) { m_close_cb = std::move(cb); }

		void	set_error_cb(EventCallBack cb) { m_error_cb = std::move(cb); }

	public:

		int		get_fd() const { return m_fd; }

	public:

		EventLoop*		m_eventloop;

		const int		m_fd;

		EventCallBack	m_read_cb;

		EventCallBack	m_write_cb;

		EventCallBack   m_close_cb;

		EventCallBack	m_error_cb;	
	};

}