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
					m_error_cb(nullptr),
					m_index(-1)
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

	void Channel::update_to_eventloop()
	{
		m_eventloop->update_channel(this);
	}

	void Channel::remove_from_eventloop()
	{
		m_eventloop->remove_channel(this);
	}

	void Channel::modify_event(int event_type, int op)
	{
		if (op == EventOp::Enable)
		{
			m_events |= event_type;
		}
		if (op == EventOp::Disable)
		{
			m_events &= event_type;
		}
		update_to_eventloop();
	}

}


