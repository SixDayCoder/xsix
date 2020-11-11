#include "xsix/network/poller_poll.h"
#include "xsix/network/channel.h"
#include "xsix/network/eventloop.h"
#include "xsix/log/log_define.h"

namespace xsix
{

	PollPoller::PollPoller(EventLoop* eventloop) : Poller(eventloop)
	{

	}

	Timestamp PollPoller::poll(int32_t timeout_ms, std::vector<Channel*>* active_channel_list)
	{
		active_channel_list->clear();

#if defined(_XSIX_LINUX)

		//param_1 : address of start_element
		//param_2 ：max index of element + 1
		//param_3 : timeout
		//return  : ready fd count
		int rc = ::poll(&(m_pollfd_list[0]), m_pollfd_list.size(), timeout_ms);

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
		for (int32_t i = 0; i < m_pollfd_list.size(); ++i)
		{
			const pollfd& pfd = m_pollfd_list[i];
			//return_events = revents
			if (pfd.revents & POLLIN)
			{
				auto it = m_channel_map.find(pfd.fd);
				if (it != m_channel_map.end())
				{
					Channel* channel = it->second;
					channel->set_events(Channel::Event::Read);
					active_channel_list->push_back(channel);
				}
			}
		}
#endif

		return xsix::Timestamp::now();
	}

	void PollPoller::update_channel(Channel* channel)
	{
		//first add
		if (channel->get_index() < 0)
		{
			XASSERT(m_channel_map.find(channel->get_fd()) == m_channel_map.end());

			struct pollfd pfd;
			pfd.fd = channel->get_fd();
			pfd.events = static_cast<short>(channel->get_events());
			pfd.revents = 0;

			m_pollfd_list.push_back(pfd);
			int new_index = (int)(m_pollfd_list.size() - 1);
			channel->set_index(new_index);
			m_channel_map.insert(std::make_pair(channel->get_fd(), channel));

			xsix::log(LOGGER(Poller), "add_channel success channel index(%) fd(%)", channel->get_index(), channel->get_fd());
		}
		//update
		else
		{
			XASSERT(m_channel_map.find(channel->get_fd()) != m_channel_map.end());
			int index = channel->get_index();

			XASSERT(index >= 0 && index < (int)m_pollfd_list.size());
			struct pollfd& pfd = m_pollfd_list[index];
			pfd.fd = channel->get_fd();
			pfd.events = static_cast<short>(channel->get_events());
			pfd.revents = 0;

			xsix::log(LOGGER(Poller), "update_channel success channel index(%) fd(%)", channel->get_index(), channel->get_fd());
		}
	}

	void PollPoller::remove_channel(Channel* channel)
	{
		XASSERT(m_channel_map.find(channel->get_fd()) != m_channel_map.end());
		int index = channel->get_index();
		XASSERT(index >= 0 && index < (int)m_pollfd_list.size());

		m_channel_map.erase(channel->get_fd());
		
		m_pollfd_list.erase(m_pollfd_list.begin() + index);

		//删除channel后调整剩余channel的index
		for (int i = 0; i < (int)m_pollfd_list.size(); ++i)
		{
			auto it = m_channel_map.find(m_pollfd_list[i].fd);
			XASSERT(it != m_channel_map.end());
			Channel* tmp_channel = it->second;
			XASSERT(tmp_channel);
			tmp_channel->set_index(i);
		}

		xsix::log(LOGGER(Poller), "remove_channel success channel index(%) fd(%)", channel->get_index(), channel->get_fd());
	}

}