#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"
#include <mutex>
#include <condition_variable>
#include <deque>

namespace xsix
{

	template<typename ElemT>
	class BlockQueue : public INonCopyable
	{
	public:

		BlockQueue() : m_mutex(), m_cond_not_empty(), m_queue() {}

	public:

		void put(const ElemT& e)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_queue.push_back(e);
			m_cond_not_empty.notify_one();
		}

		void put(ElemT&& e)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_queue.emplace_back(std::move(e));
			m_cond_not_empty.notify_one();
		}

		ElemT take()
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			while (m_queue.empty())
			{
				m_cond_not_empty.wait(lock);
			}

			ElemT e(std::move(m_queue.front()));
			m_queue.pop_front();
			return e;
		}

		bool empty() const
		{
			return m_queue.empty();
		}

		std::size_t size() const
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			return m_queue.size();
		}
		
	private:

		mutable std::mutex		m_mutex;
		std::condition_variable m_cond_not_empty;
		std::deque<ElemT>		m_queue;

	};

}