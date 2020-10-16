#include "xsix/network/channel.h"
#include "xsix/network/eventloop.h"

namespace xsix
{

	Channel::Channel(EventLoop* eventloop, int fd) : 
					m_eventloop(eventloop), 
					m_fd(fd),
					m_events(Channel::Event::None),
					m_read_cb(nullptr),
					m_write_cb(nullptr),
					m_close_cb(nullptr),
					m_error_cb(nullptr)
	{

	}

	Channel::~Channel()
	{

	}

	void Channel::handle_event()
	{
		if (m_events == Channel::Event::Read && m_read_cb)
		{
			m_read_cb();
		}

		if (m_events == Channel::Event::Write && m_write_cb)
		{
			m_write_cb();
		}
	}

	void Channel::enable_read()
	{
		//TODO:
		update_to_eventloop();
	}

	void Channel::disable_read()
	{
		//TODO:
		update_to_eventloop();
	}

	void Channel::enable_write()
	{
		//TODO:
		update_to_eventloop();
	}

	void Channel::disable_write()
	{
		//TODO:
		update_to_eventloop();
	}

	void Channel::update_to_eventloop()
	{
		m_eventloop->update_channel(this);
	}

	void Channel::remove_from_eventloop()
	{
		m_eventloop->remove_channel(this);
	}

}


