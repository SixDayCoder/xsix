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

		bool		  empty() const { return m_head == m_tail; }

		void		  clear();

		std::string   retrieve_all_as_string();

	public:

		int32_t	length()  const;

		int32_t	size() const { return m_size; }

		int32_t	get_free_size() const { return m_size - length(); }

		const int32_t headpos() const { return m_head; }

		const int32_t tailpos() const { return m_tail; }

	public:

		int32_t	append(const char* src, int32_t size);

		int32_t	write_to(void* dst, int32_t size);

		int32_t peek(void* dst, int32_t size);

		bool	skip(int32_t size);

	private:

		void cleanup();

		bool resize(int32_t newsize);

		void copy_from(const char* src, int32_t len, int32_t size);

		void neaten();

	private:

		char*	 m_data;
		int32_t  m_size = 0;
		int32_t  m_head = 0;
		int32_t  m_tail = 0;
	};
}