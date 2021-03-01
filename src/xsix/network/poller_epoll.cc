#include "xsix/network/poller_epoll.h"

namespace xsix
{

	EpollPoller::EpollPoller(EventLoop* eventloop) : Poller(eventloop)
	{

	}

	xsix::Timestamp EpollPoller::poll(int32_t timeout_ms, std::vector<Channel*>* active_channel_list)
	{
		return Timestamp::now();
	}

	void EpollPoller::update_channel(Channel* channel)
	{

	}

	void EpollPoller::remove_channel(Channel* channel)
	{

	}

}