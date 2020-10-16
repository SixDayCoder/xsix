#include "xsix/network/poller_select.h"
#include "xsix/network/channel.h"
#include "xsix/network/eventloop.h"

namespace xsix
{

	SelectPoller::SelectPoller(EventLoop* eventloop) : Poller(eventloop)
	{

	}

	Timestamp SelectPoller::poll(int32_t timeout_ms, std::vector<Channel*> *active_channel_list)
	{
		int max_socket_id = -1;

		FD_ZERO(&m_read_set);
		FD_ZERO(&m_write_set);
		FD_ZERO(&m_except_set);

		for (auto it = m_channel_map.begin(); it != m_channel_map.end(); ++it)
		{
			int fd = it->second->get_fd();
			FD_SET(fd, &m_read_set);
			FD_SET(fd, &m_write_set);
			FD_SET(fd, &m_except_set);
			if (max_socket_id < it->second->get_fd())
			{
				max_socket_id = fd;
			}
		}

		struct timeval tv { 0, timeout_ms * 1000 };
		int rc = ::select(max_socket_id + 1, &m_read_set, &m_write_set, &m_except_set, &tv);

		//error
		if (rc < 0)
		{
			return Timestamp::now();
		}

		//timeout
		if (rc == 0)
		{
			return Timestamp::now();
		}

		//success
		for (auto it = m_channel_map.begin() ; it != m_channel_map.end(); ++it)
		{
			int fd = it->second->get_fd();
			if (FD_ISSET(fd, &m_read_set))
			{
				it->second->set_event(Channel::Event::Read);
				active_channel_list->push_back(it->second);
			}
		}

		return Timestamp::now();
	}

	void SelectPoller::update_channel(Channel* channel)
	{
		auto it = m_channel_map.find(channel->get_fd());
		if (it != m_channel_map.end())
		{
			return;
		}
		else
		{
			m_channel_map.insert(std::make_pair(channel->get_fd(), channel));
		}
	}

	void SelectPoller::remove_channel(Channel* channel)
	{
		auto it = m_channel_map.find(channel->get_fd());
		if (it == m_channel_map.end())
		{
			return;
		}
		else
		{
			m_channel_map.erase(channel->get_fd());
		}
	}
}