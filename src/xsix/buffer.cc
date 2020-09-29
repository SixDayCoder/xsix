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

	buffer::buffer(int32_t init_size /*= BUFFER_SIZE_SMALL*/)
	{
		init();
		init_size = init_size > BUFFER_SIZE_SMALL ? init_size : BUFFER_SIZE_SMALL;
		init_size = init_size > BUFFER_SIZE_MAX ? BUFFER_SIZE_MAX : init_size;
		XASSERT(resize(init_size));
	}

	buffer::buffer(const std::string& s)
	{
		init();
		copy_from(s.c_str(), s.length(), s.length() + s.length() / 2);
	}

	buffer::~buffer()
	{
		cleanup();
	}

	int32_t buffer::write(const char* src, int32_t size)
	{
		XASSERT(src);

		if (src <= 0)
		{
			return 0;
		}

		if (free_size() < size)
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
			if (m_head == 0)
			{
				memcpy(&m_data[m_tail], src, size);
			}
			else
			{
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
					memcpy(m_data, &src[right_free], size - right_free);
				}
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

		m_tail = (m_tail + size) % m_size;
		return size;
	}

	int32_t buffer::read(char* dst, int32_t size)
	{
		if (size == 0)
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
			int32_t nRightlength = m_size - m_head;
			if (size <= nRightlength)
			{
				memcpy(dst, &m_data[m_head], size);
			}
			else
			{
				memcpy(dst, &m_data[m_head], nRightlength);
				memcpy(&dst[nRightlength], m_data, size - nRightlength);
			}
		}

		m_head = (m_head + size) % m_size;
		return size;
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

		char tmp[2048] = { 0 };
		if (m_head <= m_tail)
		{
			//c(1)
			if (m_head == 0)
			{
				memcpy(tmp, &m_data[0], length());
				tmp[length()] = '\0';
			}
			//c(2)
			else
			{
				memcpy(tmp, &m_data[m_head], m_tail - m_head);
				tmp[length()] = '\0';
			}
		}
		//c(3)
		else
		{
			memcpy(tmp, &m_data[m_head], m_size - m_head);
			memcpy(&tmp[m_size - m_head], &m_data[0], m_tail);
			tmp[length()] = '\0';
		}

		char fmt[2048] = { 0 };
		snprintf(fmt, 
			2048,
			"buffer::[head : %d, tail : %d, length : %d, size : %d, free_size : %d, data : \"%s\"]",
			m_head, m_tail, length(), size(), free_size(), tmp
		);
		return std::string(fmt);
	}

	int32_t buffer::length() const
	{
		if (empty())
		{
			return 0;
		}

		if (m_head < m_tail)
		{
			return m_tail - m_head;
		}

		return m_size - m_head + m_tail;
	}

	void buffer::init()
	{
		m_data = nullptr;

		m_size = m_head = m_tail = 0;
	}

	void buffer::cleanup()
	{
		if (m_data)
		{
			XFREE(m_data);

			m_data = nullptr;

			m_size = m_head = m_tail = 0;
		}
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

		char* new_data = (char*)XMALLOC(newsize);
		XASSERT(new_data);
		if (m_head < m_tail)
		{
			memcpy(new_data, &m_data[m_head], m_tail - m_head);
		}
		else if (m_head > m_tail)
		{
			memcpy(new_data, &m_data[m_head], m_size - m_head);
			memcpy(&new_data[m_size - m_head], m_data, m_tail);
		}

		XFREE(m_data);
		m_data = new_data;
		m_head = 0;
		m_tail = length();
		m_size = newsize;
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
			m_head = 0;
			m_tail = len;
			m_size = size;
			memcpy(m_data, src, len);
		}
	}
}