#include "xsix/network/tcp_server.h"
#include "xsix/network/acceptor.h"

namespace xsix
{

	TCPServer::TCPServer(EventLoop* eventloop, uint16_t listen_port) :
		m_eventloop(eventloop),
		m_acceptor(new Acceptor(eventloop, listen_port))
				
	{
		m_next_conn_id.store(0);

		m_acceptor->set_new_conn_callback(
			std::bind(&TCPServer::new_conn, this, 
				std::placeholders::_1, 
				std::placeholders::_2)
		);

		printf("tcp server ctor\n");
	}

	TCPServer::~TCPServer()
	{
		printf("tcp server dtor\n");
	}

	void TCPServer::run()
	{
		m_acceptor->listen();
	}

	void TCPServer::new_conn(int sockfd, const NetAddr& addr)
	{
		printf("new tcp conn established : <sockfd : %d, ip : %s, port : %d>\n", 
			sockfd, addr.ipaddress().c_str(), addr.port()
		);
		TCPConnPtr conn(new TCPConn(m_eventloop, sockfd));
	}

}