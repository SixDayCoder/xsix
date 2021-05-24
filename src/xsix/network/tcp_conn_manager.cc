
#include "tcp_conn_manager.h"


namespace xsix
{

	int32_t TCPConnManager::add_conn(TCPConnPtr connptr)
	{
		if (connptr)
		{
			if (connptr->get_id() == m_next_tcp_conn_id)
			{
				return -1;
			}
			
			m_tcp_conn_map.insert(std::make_pair(m_next_tcp_conn_id, connptr));
			connptr->set_id(m_next_tcp_conn_id++);
			return connptr->get_id();
		}
		return -1;
	}

	xsix::TCPConnPtr TCPConnManager::remove_conn(int32_t connid)
	{
		auto it = m_tcp_conn_map.find(connid);
		if (it == m_tcp_conn_map.end())
		{
			return nullptr;
		}

		xsix::TCPConnPtr ptr = it->second;
		m_tcp_conn_map.erase(connid);

		printf("remove connid : %d\n", ptr->get_id());
		return ptr;
	}

	xsix::TCPConnPtr TCPConnManager::find_conn(int32_t connid)
	{
		auto it = m_tcp_conn_map.find(connid);
		if (it == m_tcp_conn_map.end())
		{
			return nullptr;
		}
		return it->second;
	}

	void TCPConnManager::broadcast(const char* msg, int32_t size)
	{
		for (auto it = m_tcp_conn_map.begin(); it != m_tcp_conn_map.end(); ++it)
		{
			if (it->second)
			{
				it->second->send(msg, size);
			}
		}
	}

	void TCPConnManager::print_conn_state()
	{
		for (auto it = m_tcp_conn_map.begin(); it != m_tcp_conn_map.end(); ++it)
		{
			if (it->second)
			{
				printf("conn:%d, recvsize:(%d:%d) recvht(%d:%d) sendsize:(%d:%d) sendht:(%d,%d) fullstate(%s,%s)\n",
					it->second->get_id(),
					it->second->m_recv_buffer.length(), it->second->m_recv_buffer.get_free_size(),
					it->second->m_recv_buffer.headpos(), it->second->m_recv_buffer.tailpos(),
					it->second->m_send_buffer.length(), it->second->m_send_buffer.get_free_size(),
					it->second->m_send_buffer.headpos(), it->second->m_send_buffer.tailpos(),
					it->second->m_recv_buffer.full() ? "full" : "no",
					it->second->m_send_buffer.full() ? "full" : "no"
					);
			}
		}
	}

}

