#pragma once

#include "xsix/network/poller.h"
#include "xsix/network/network.h"

namespace xsix
{
	class PollPoller : public Poller
	{
	public:

		PollPoller(EventLoop* eventloop);

	public:

		virtual Timestamp	poll(int32_t timeout_ms, std::vector<Channel*>* active_channel_list) override;

		virtual void		update_channel(Channel* channel) override;

		virtual void		remove_channel(Channel* channel) override;

	private:

		std::vector<struct pollfd> m_pollfd_list;
	};
}