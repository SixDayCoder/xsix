#include "xsix/network/channel.h"
#include "xsix/network/eventloop.h"

namespace xsix
{

	Channel::Channel(EventLoop* eventloop, int fd) : 
					m_eventloop(eventloop), 
					m_fd(fd),
					m_read_cb(nullptr),
					m_write_cb(nullptr),
					m_close_cb(nullptr),
					m_error_cb(nullptr)
	{

	}

	Channel::~Channel()
	{

	}

}


