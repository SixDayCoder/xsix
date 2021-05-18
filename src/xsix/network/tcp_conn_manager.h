#pragma once

#include "tcp_conn.h"

namespace xsix
{

	class TCPServer;

	class TCPConnManager
	{
	public:

		friend class TCPServer;
	
	public:

		int32_t		add_conn(TCPConnPtr connptr);

		TCPConnPtr  remove_conn(int32_t connid);

		TCPConnPtr  find_conn(int32_t connid);

	public:

		void broadcast(const char* msg, int32_t size);

		void print_conn_state();

	private:

		int32_t						  m_next_tcp_conn_id = 0;
		std::map<int32_t, TCPConnPtr> m_tcp_conn_map;

	};
}

