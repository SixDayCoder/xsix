#include "xsix/buffer.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>


namespace xsix
{
	static int32_t __refix_new_size(int32_t oldsize, int32_t newsize)
	{
		if (newsize <= buffer::BUFFER_SIZE_SMALL)
		{
			return buffer::BUFFER_SIZE_SMALL;
		}
		int32_t tmp = oldsize;
		while (tmp < newsize)
		{
			if (tmp < buffer::BUFFER_SIZE_RAW)
			{
				tmp *= 2;
			}
			else if (tmp < buffer::BUFFER_SIZE_MAX)
			{
				tmp += buffer::BUFFER_SIZE_RAW;
			}
		}
		return tmp;
	}

	static bool __is_valid_size(int32_t size)
	{
		return (size > 0 && size <= buffer::BUFFER_SIZE_MAX);
	}

	buffer::buffer(int32_t init_size /*= BUFFER_SIZE_SMALL*/) : m_data(nullptr)
	{
		cleanup();
		init_size = init_size > BUFFER_SIZE_SMALL ? init_size : BUFFER_SIZE_SMALL;
		init_size = init_size > BUFFER_SIZE_MAX ? BUFFER_SIZE_MAX : init_size;
		XASSERT(resize(init_size));
	}

	buffer::buffer(const std::string& s) : m_data(nullptr)
	{
		cleanup();
		copy_from(s.c_str(), s.length(), s.length() + s.length() / 2);
	}

	buffer::~buffer()
	{
		cleanup();
	}

	void buffer::clear()
	{
		m_tail = m_head = 0;
		m_full = false;
	}

	std::string buffer::retrieve_all_as_string()
	{
		if (empty())
		{
			return std::string("");
		}

		char tmp[4096] = { 0 };
		if (m_head < m_tail)
		{
			memcpy(tmp, &m_data[m_head], length());
		}
		else
		{
			int32_t right_free = m_size - m_head;
			memcpy(tmp, &m_data[m_head], right_free);
			memcpy(&tmp[right_free], m_data, m_tail);
		}
		tmp[length()] = '\0';

		return std::string(tmp);
	}

	int32_t buffer::append(const std::string& s)
	{
		return append(s.c_str(), (int32_t)s.length());
	}

	int32_t buffer::append(const void* src, int32_t size)
	{
		XASSERT(src);

		if (size <= 0)
		{
			return 0;
		}

		if (get_free_size() < size)
		{
			if (!resize(length() + size))
			{
				return 0;
			}
		}

		if (m_head <= m_tail)
		{
			// condition(1)
			// |---------------------|---------------------------|
			// 0(head)				 tail                        size
			// <-------good----------><------------free------------>

			// condition(2)
			// |-----------|-----------|---------------------------|
			// 0           head		   tail                        size
			// <--no data--><-- good----><------------free--------->

			int32_t right_free = m_size - m_tail;
			if (size <= right_free)
			{
				memcpy(&m_data[m_tail], src, size);
			}
			else
			{
				memcpy(&m_data[m_tail], src, right_free);
				memcpy(m_data, (char*)src + right_free, size - right_free);
			}
		}
		else
		{
			// condition(3)
			// |---------|------------|---------------------------|
			// 0         tail         head                        size
			// <---good---><---free---><-------------good---------->
			memcpy(&m_data[m_tail], src, size);
		}
		int32_t oldtail = m_tail;
		m_tail = (m_tail + size) % m_size;
		check_full_after_append(oldtail, m_tail);
		return size;
	}

	int32_t buffer::write_to(void* dst, int32_t size)
	{
		if (!dst || size == 0)
		{
			return 0;
		}

		if (empty())
		{
			return 0;
		}

		if (length() < size)
		{
			size = length();
		}

		if (m_head < m_tail)
		{
			memcpy(dst, &m_data[m_head], size);
		}
		else
		{
			int32_t right_free = m_size - m_head;
			if (size <= right_free)
			{
				memcpy(dst, &m_data[m_head], size);
			}
			else
			{
				memcpy(dst, &m_data[m_head], right_free);
				memcpy((char*)dst + right_free, m_data, size - right_free);
			}
		}

		int32_t oldhead = m_head;
		m_head = (m_head + size) % m_size;
		check_full_after_writeto(oldhead, m_head);
		return size;
	}

	int32_t buffer::length() const
	{
		if (empty())
		{
			return 0;
		}

		if (full())
		{
			return m_size;
		}

		if (m_head < m_tail)
		{
			return m_tail - m_head;
		}
		else
		{
			return (m_size - m_head) + (m_tail);
		}

		return 0;
	}

	int32_t buffer::peek(void* dst, int32_t size)
	{
		if (!dst || size == 0)
		{
			return 0;
		}

		if (empty())
		{
			return 0;
		}

		if (length() < size)
		{
			size = length();
		}

		if (m_head < m_tail)
		{
			memcpy(dst, &m_data[m_head], size);
		}
		else
		{
			int32_t right_free = m_size - m_head;
			if (size <= right_free)
			{
				memcpy(dst, &m_data[m_head], size);
			}
			else
			{
				memcpy(dst, &m_data[m_head], right_free);
				memcpy((char*)dst + right_free, m_data, size - right_free);
			}
		}

		return size;
	}

	bool buffer::skip(int32_t size)
	{
		if (size <= 0)
		{
			return false;
		}

		if (length() < size)
		{
			return false;
		}

		int32_t oldhead = m_head;
		m_head = (m_head + size) % m_size;
		check_full_after_writeto(oldhead, m_head);
		return true;
	}

	void buffer::cleanup()
	{
		if (m_data)
		{
			XFREE(m_data);
		}
		m_data = nullptr;
		m_size = m_head = m_tail = 0;
		m_full = false;
	}

	void buffer::realloc(int32_t newsize)
	{
		if (newsize <= 0)
		{
			return;
		}

		char* new_data = (char*)XMALLOC(newsize);
		XASSERT(new_data);

		if (m_data != nullptr)
		{
			if (m_head < m_tail)
			{
				memcpy(new_data, &m_data[m_head], m_tail - m_head);
			}
			else
			{
				int32_t right_free = m_size - m_head;
				memcpy(new_data, &m_data[m_head], right_free);
				memcpy(&new_data[right_free], m_data, m_tail);
			}
			XFREE(m_data);
		}

		m_data = new_data;
		m_head = 0;
		m_tail = length();
		m_size = newsize;
		m_full = m_tail == m_size ? true : false;
	}

	bool buffer::resize(int32_t newsize)
	{
		if (!__is_valid_size(newsize))
		{
			return false;
		}

		if (size() >= newsize)
		{
			return true;
		}

		newsize = __refix_new_size(m_size, newsize);
		if (!__is_valid_size(newsize))
		{
			return false;
		}

		realloc(newsize);
		return true;
	}

	void buffer::copy_from(const char* src, int32_t len, int32_t size)
	{
		if (src && len > 0 && len <= size)
		{
			if (m_data)
			{
				XFREE(m_data);
			}
			m_data = (char*)XMALLOC(len);
			XASSERT(m_data);	
			m_size = size;
			m_head = 0;
			m_tail = (m_tail + size) % m_size;
			memcpy(&m_data[0], src, len);
			m_full = m_tail == m_size ? true : false;
		}
	}

	void buffer::check_full_after_append(int32_t oldtail, int32_t newtail)
	{
		m_full = false;
		if (m_head == newtail)
		{
			m_full = true;
		}
	}

	void buffer::check_full_after_writeto(int32_t oldhead, int32_t newhead)
	{
		m_full = false;
	}

	std::string buffer::get_debug_text()
	{
		if (!m_data)
		{
			return std::string("");
		}

		if (empty())
		{
			return std::string("");
		}

		std::string s = retrieve_all_as_string();
		char fmt[4096 + 256] = { 0 };
		snprintf(fmt,
			4096 + 256,
			"buffer::[head(%d) tail(%d) length(%d) size(%d) free_size(%d) data(\"%s\")]",
			m_head, m_tail, length(), size(), get_free_size(), s.c_str()
		);
		return std::string(fmt);
	}
}