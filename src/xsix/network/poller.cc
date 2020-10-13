#include "xsix/network/poller.h"
#include "xsix/network/channel.h"
#include "xsix/network/eventloop.h"

namespace xsix
{

	Poller::Poller(EventLoop* eventloop) :
			m_eventloop(eventloop)
	{

	}

	Poller::~Poller()
	{

	}

	bool Poller::has_channel(Channel* channel) const
	{	
		auto it = m_channel_map.find(channel->get_fd());
		return it != m_channel_map.end() && it->second == channel;
	}

}