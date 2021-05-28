#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"

namespace xsix
{	
	template<typename ElementT>
	class ThreadSafeList : public xsix::INonCopyable
	{
	public:

		int32_t size() const
		{
			std::lock_guard<std::mutex> lck(m_mutex);
			return m_list.size();
		}

		bool empty() const
		{
			std::lock_guard<std::mutex> lck(m_mutex);
			return m_list.empty();
		}

		void push_back(const ElementT& e)
		{
			std::lock_guard<std::mutex> lck(m_mutex);
			m_list.push_back(std::move(e));
		}

		void push_front(const ElementT& e)
		{
			std::lock_guard<std::mutex> lck(m_mutex);
			m_list.push_front(std::move(e));
		}

		const ElementT& back() const
		{
			std::lock_guard<std::mutex> lck(m_mutex);
			return m_list.back();
		}

		const ElementT& front() const
		{
			std::lock_guard<std::mutex> lck(m_mutex);
			return m_list.front();
		}

		void pop_back()
		{
			std::lock_guard<std::mutex> lck(m_mutex);
			m_list.pop_back();
		}

		void pop_front()
		{
			std::lock_guard<std::mutex> lck(m_mutex);
			m_list.pop_front();
		}

	private:
		mutable std::mutex	m_mutex;
		std::list<ElementT> m_list;
	};
}