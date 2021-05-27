#include "chat_room_msg.hpp"
#include "xsix/network/tcp_client.hpp"
//
//#include <iostream>
//#include <functional>
//#include <chrono>
//using namespace std;
//
//void send_random_msg(xsix::TCPClientPtr client)
//{
//	std::string s = chat_room_msg::get_random_msg();
//	chat_room_msg msg;
//	msg.set_content(s.c_str(), s.length());
//	auto sendbuf = msg.get_buf();
//	client->send(&sendbuf[0], sendbuf.size());
//}
//
//void connected(xsix::TCPClientPtr ptr)
//{
//	if (!ptr)
//	{
//		return;
//	}
//	cout << "connected ready input!" << endl;
//	send_random_msg(ptr);
//}
//
//void chat(xsix::TCPClientPtr conn)
//{
//	if (!conn)
//	{
//		return;
//	}
//	
//	if (conn->m_recv_buffer.length() < sizeof(chat_room_msg_header))
//	{
//		return;
//	}
//
//	int32_t peeksize = 0;
//	while (!conn->m_recv_buffer.empty())
//	{
//		chat_room_msg msg;
//		peeksize = conn->m_recv_buffer.peek(&msg.header, sizeof(chat_room_msg_header));
//		if (peeksize != sizeof(chat_room_msg_header))
//		{
//			std::cout <<  "peeksize error" << std::endl;
//			break;
//		}
//
//		uint16_t oldsize = msg.header.contentsize;
//		msg.header.contentsize = ntohs(msg.header.contentsize);
//		if (msg.header.contentsize <= 0 || 
//			conn->m_recv_buffer.length() < msg.header.contentsize + sizeof(chat_room_msg_header))
//		{
//			std::cout << " client "
//					  << " old size : " << oldsize
//					  << " calc size : " << msg.header.contentsize
//					  << " buffer size " << conn->m_recv_buffer.length() << std::endl;
//			break;
//		}
//
//		XASSERT(conn->m_recv_buffer.skip(sizeof(chat_room_msg_header)));
//		XASSERT(conn->m_recv_buffer.write_to(&msg.content[0], msg.header.contentsize));
//
//		std::cout << msg.get_content() << std::endl;
//	}
//
//	send_random_msg(conn);
//}

int main()
{
	//asio::ip::tcp::endpoint ep(asio::ip::address::from_string("127.0.0.1"), 9999);
	//xsix::TCPClientPtr client(new xsix::TCPClient());

	//client->set_connected_handler(connected);
	//client->set_message_handler(chat);
	//client->async_connect(ep);
	//client->loop();
	//
	return 0;
}
