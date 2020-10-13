#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"
#include "xsix/time/timestamp.h"
#include <vector>
#include <map>

namespace xsix
{
	class Channel;

	class EventLoop;

	class Poller : INonCopyable
	{

	public:

		Poller(EventLoop* eventloop);

		virtual ~Poller();

	public:

		virtual Timestamp poll(int32_t timeout_ms, std::vector<Channel*> *active_channel_list) = 0;

		virtual void update_channel(Channel* channel) = 0;

		virtual void remove_channel(Channel* channel) = 0;

		virtual bool has_channel(Channel* channel) const;

	protected:

		std::map<int32_t, Channel*> m_channel_map;

	private:

		EventLoop* m_eventloop;
	};
}