#pragma once
#include "xsix/common_define.h"

namespace xsix
{
	class buffer
	{
	public:

		enum 
		{
			BUFFER_SIZE_SMALL = 64,	
			BUFFER_SIZE_RAW   = 1024,								//1KB
			BUFFER_SIZE_MAX   = BUFFER_SIZE_RAW * BUFFER_SIZE_RAW,  //1MB
		};

	public:

		explicit buffer(const std::string& s);

		buffer(int32_t init_size = BUFFER_SIZE_SMALL);

		~buffer();

	public:

		std::string  get_debug_text();

	public:

		bool	empty() const { return m_head == m_tail; }

		int32_t	write(const char* src, int32_t size);

		int32_t	read(char* dst, int32_t size);

		int32_t	length()  const;

		int32_t	size() const { return m_size; }

		int32_t	free_size() const  { return ((m_head <= m_tail) ? (m_size - m_tail + m_head - 1) : (m_head - m_tail - 1)); }

	private:

		void init();

		void cleanup();

		bool resize(int32_t newsize);

		void copy_from(const char* src, int32_t len, int32_t size);

	private:

		char*	 m_data;

		int32_t  m_size;

		int32_t  m_head;

		int32_t  m_tail;
	};
}