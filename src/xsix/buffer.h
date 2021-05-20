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

		bool		  empty() const { return (m_head == m_tail) && !full(); }
		bool		  full() const { return m_full; }
		void		  clear();
		std::string   retrieve_all_as_string();

	public:

		int32_t	length()  const;
		int32_t	size() const { return m_size; }

		const int32_t headpos() const { return m_head; }
		const int32_t tailpos() const { return m_tail; }

		int32_t		  get_free_size() const { return m_size - length(); }
		std::string   get_debug_text();

	public:

		int32_t	append(const void* src, int32_t size);
		int32_t append(const std::string& s);
		int32_t	write_to(void* dst, int32_t size);
		int32_t peek(void* dst, int32_t size);
		bool	skip(int32_t size);

	private:

		void cleanup();
		void realloc(int32_t newsize);
		bool resize(int32_t newsize);
		void copy_from(const char* src, int32_t len, int32_t size);
		void check_full_after_append(int32_t oldtail, int32_t newtail);
		void check_full_after_writeto(int32_t oldhead, int32_t newhead);

	private:

		char*	 m_data;
		int32_t  m_size = 0;
		int32_t  m_head = 0;
		int32_t  m_tail = 0;
		bool	 m_full = false;
	};
}