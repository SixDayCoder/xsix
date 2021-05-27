#include "chat_room_msg.hpp"
#include "xsix/network/tcp_server.hpp"

//class ChatRoom
//{
//public:
//
//	ChatRoom() : m_tcp_server(new xsix::TCPServer(m_context, 9999))
//	{
//		m_tcp_server->set_accept_handler(
//			std::bind(&ChatRoom::accept_handler, this,
//				std::placeholders::_1,
//				std::placeholders::_2,
//				std::placeholders::_3)
//		);
//
//		m_tcp_server->set_conn_message_handler(
//			std::bind(&ChatRoom::message_handler, this, std::placeholders::_1)
//		);
//
//		m_tcp_server->set_conn_close_handler(
//			[](xsix::TCPConnPtr conn, const asio::error_code& ec) {
//				if (conn)
//				{
//					cout << "conn quit chatroom : " << conn->get_id() << endl;
//				}
//			}
//		);
//
//		m_tcp_server->accept();
//	}
//
//	void loop()
//	{
//		m_tcp_server->loop();
//	}
//
//private:
//
//	void accept_handler(xsix::TCPServerPtr server, xsix::TCPConnPtr conn, const asio::error_code& ec)
//	{
//		if (!server || !conn)
//		{
//			return;
//		}
//		cout << "welcome conn join : " << conn->get_id() << endl;
//	}
//
//	void message_handler(xsix::TCPConnPtr conn)
//	{
//		if (!conn)
//		{
//			return;
//		}
//
//		int32_t peeksize = 0;
//		while (!conn->m_recv_buffer.empty())
//		{
//			if (conn->m_recv_buffer.length() < sizeof(chat_room_msg_header))
//			{
//				break;
//			}
//
//			chat_room_msg msg;
//			peeksize = conn->m_recv_buffer.peek(&msg.header, sizeof(chat_room_msg_header));
//			if (peeksize != sizeof(chat_room_msg_header))
//			{
//				break;
//			}
//
//			uint16_t oldsize = msg.header.contentsize;
//			msg.header.contentsize = ntohs(msg.header.contentsize);
//			if (msg.header.contentsize <= 0 || 
//				conn->m_recv_buffer.length() < msg.header.contentsize + sizeof(chat_room_msg_header))
//			{
//				std::cout << " id : " << conn->get_id()
//						  << " old size : "  << oldsize
//						  << " calc size : " << msg.header.contentsize 
//						  << " buffer size " << conn->m_recv_buffer.length() << std::endl;  
//				break;
//			}
//
//			XASSERT(conn->m_recv_buffer.skip(sizeof(chat_room_msg_header)));
//			XASSERT(conn->m_recv_buffer.write_to(&msg.content[0], msg.header.contentsize));
//
//			//sendback
//			char buf[4096] = { 0 };
//			int32_t sendbacksize = snprintf(buf, 4096, "guest(%d):%s", conn->get_id(), msg.get_content().c_str());
//			chat_room_msg sendbackmsg;
//			sendbackmsg.set_content(buf, sendbacksize);
//			auto sendbuf = sendbackmsg.get_buf();
//			m_tcp_server->broadcast(&sendbuf[0], sendbuf.size());
//		}
//	}
//
//private:
//
//	asio::io_context   m_context;
//	xsix::TCPServerPtr m_tcp_server;
//};


int main()
{
	return 0;
}
