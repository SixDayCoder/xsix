#include "chat_room_msg.hpp"
#include "xsix/actor/actor_core.h"
#include "xsix/network/tcp_server.h"

#include <iostream>
using namespace std;

class ChatRoomActor : public xsix::ActorBase
{
public:

	ChatRoomActor() :
		xsix::ActorBase(),
		m_tcp_server(new xsix::TCPServer(m_context, 9999))
	{
		m_tcp_server->set_reuse_addr(true);

		m_tcp_server->set_accept_handler(
			std::bind(&ChatRoomActor::accept_handler, this,
				std::placeholders::_1,
				std::placeholders::_2,
				std::placeholders::_3)
		);

		m_tcp_server->set_conn_message_handler(
			std::bind(&ChatRoomActor::message_handler, this, std::placeholders::_1)
		);

		m_tcp_server->set_conn_close_handler(
			[](xsix::TCPConnPtr conn, const asio::error_code& ec) {
				if (conn)
				{
					cout << "conn quit chatroom : " << conn->get_id() << endl;
				}
			}
		);

		m_tcp_server->accept();
	}

private:

	chat_room_msg get_sendback_msg(int32_t id, const chat_room_msg& msg)
	{
		char buf[2048] = { 0 };
		snprintf(buf, 2048, "guest:%d say:%s", id, msg.content);

		chat_room_msg sendmsg;
		sendmsg.set_content(buf, strlen(buf));

		return sendmsg;
	}

	void broad_cast(const chat_room_msg& msg)
	{
		cout << msg.content << endl;

		int32_t bytes = 0;
		char sendbuf[1024] = { 0 };
		msg.write_to_buf(sendbuf, &bytes);
		
		m_tcp_server->get_conn_mgr().broadcast(sendbuf, bytes);
	}

	void accept_handler(xsix::TCPServerPtr server, xsix::TCPConnPtr conn, const asio::error_code& ec)
	{
		if (!server || !conn)
		{
			return;
		}

		cout << "welcome conn join : " << conn->get_id() << endl;
	}

	void message_handler(xsix::TCPConnPtr conn)
	{
		if (!conn)
		{
			return;
		}

		if (conn->m_recv_buffer.length() < sizeof(chat_room_msg_header))
		{
			return;
		}

		int32_t peeksize = 0;
		chat_room_msg msg;
		while (!conn->m_recv_buffer.empty())
		{
			msg.clear();

			peeksize = conn->m_recv_buffer.peek(&msg.header, sizeof(chat_room_msg_header));
			if (peeksize != sizeof(chat_room_msg_header))
			{
				break;
			}

			msg.header.contentsize = ntohs(msg.header.contentsize);
			if (msg.header.contentsize <= 0 || conn->m_recv_buffer.length() < msg.header.contentsize)
			{
				break;
			}

			conn->m_recv_buffer.skip(sizeof(chat_room_msg_header));
			conn->m_recv_buffer.write_to(msg.content, msg.header.contentsize);
			chat_room_msg sendbackmsg = get_sendback_msg(conn->get_id(), msg);
			broad_cast(sendbackmsg);
		}
	}

public:

	virtual int32_t get_tick_interval() const override { return 200; }

	virtual void tick()
	{
		xsix::ActorBase::tick();
		m_tcp_server->tick();
		//m_tcp_server->get_conn_mgr().print_conn_state();
	}

private:

	asio::io_context   m_context;
	xsix::TCPServerPtr m_tcp_server;
};


int main()
{
	xsix::ActorCore core;
	core.register_actor(xsix::ActorBasePtr(new ChatRoomActor()));
	core.run();

	return 0;
}
