#pragma once

#include "xsix/network/poller.h"
#include "xsix/network/network.h"

namespace xsix
{
	class SelectPoller : public Poller
	{
	public:

		SelectPoller(EventLoop* eventloop);

	public:

		virtual Timestamp	poll(int32_t timeout_ms, std::vector<Channel*> *active_channel_list) override;

		virtual void		update_channel(Channel* channel) override;

		virtual void		remove_channel(Channel* channel) override;;

	private:

		fd_set m_read_set;

		fd_set m_write_set;

		fd_set m_except_set;
	};
}