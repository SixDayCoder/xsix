
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
}

