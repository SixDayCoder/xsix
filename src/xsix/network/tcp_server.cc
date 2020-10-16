#include "xsix/network/tcp_server.h"
#include "xsix/network/acceptor.h"

namespace xsix
{

	TCPServer::TCPServer(EventLoop* eventloop, uint16_t listen_port) :
		m_eventloop(eventloop),
		m_acceptor(new Acceptor(eventloop, listen_port)),
		m_next_conn_id(0)
				
	{
		m_acceptor->set_new_conn_callback(
			std::bind(&TCPServer::new_conn, this, 
				std::placeholders::_1, 
				std::placeholders::_2)
		);
	}

	TCPServer::~TCPServer()
	{
	}

	void TCPServer::run()
	{
		m_acceptor->listen();
	}

	void TCPServer::new_conn(int sockfd, const NetAddr& addr)
	{
		printf("[TCPServer] new tcp conn established : <sockfd : %d, ip : %s, port : %d>\n", 
			sockfd, addr.ipaddress().c_str(), addr.port()
		);

		TCPConnPtr conn(new TCPConn(m_eventloop, sockfd, m_next_conn_id));	
		conn->set_recv_message_callback(m_conn_recv_message_cb);
		conn->set_remove_conn_callback(std::bind(&TCPServer::remove_conn, this, std::placeholders::_1));
		conn->on_conn_established();

		m_conn_map.insert(std::make_pair(conn->get_id(), conn));
		m_next_conn_id++;
	}

	void TCPServer::remove_conn(const TCPConnPtr& conn)
	{
		printf("[TCPServer] remove tcp conn : <id, : %d, sockfd : %d>\n", conn->get_id(), conn->get_fd());
		m_conn_map.erase(conn->get_id());
		conn->on_conn_destoryed();
	}

}