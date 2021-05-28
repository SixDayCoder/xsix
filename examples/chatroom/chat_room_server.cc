#include "chat_room_msg.hpp"
#include "xsix/network/tcp_connection.hpp"
#include "xsix/network/tcp_server.hpp"
#include "xsix/thread_safe_list.hpp"

class Connection : public xsix::net::TCPConnection
{
public:

	Connection(asio::io_context& ctx) : 
		xsix::net::TCPConnection(ctx)
		{}

public:

	virtual void start() override
	{
		read_header();
	}

	void read_header()
	{
		if (!m_tcp_socket.is_open())
		{
			return;
		}

		m_tmp_msg.clear();
		m_tcp_socket.async_read_some(asio::buffer(&m_tmp_msg.header, sizeof(chat_room_msg_header)),
			[&](const asio::error_code& ec, std::size_t bytes) {
				if (ec)
				{
					handle_error(ec);
					return;
				}
				read_body();
			}
		);
	}

	void read_body()
	{
		if (!m_tcp_socket.is_open())
		{
			return;
		}
		uint16_t real_content_size = ntohs(m_tmp_msg.header.contentsize);
		m_tcp_socket.async_read_some(asio::buffer(&m_tmp_msg.content[0], real_content_size),
			[&](const asio::error_code& ec, std::size_t bytes) {
				if (ec)
				{
					handle_error(ec);
					return;
				}
				handle_message();
				read_header();
			}
		);
	}

	virtual void handle_message() override
	{
		uint16_t real_content_size = ntohs(m_tmp_msg.header.contentsize);
		std::string msg(&m_tmp_msg.content[0], real_content_size);

		chat_room_msg sendbackmsg = build_sendback_msg(msg);
		send_msg_to_client(sendbackmsg);
	}

	void send_msg_to_client(const chat_room_msg& msg)
	{
		bool iswriting = !m_send_queue.empty();
		m_send_queue.push_back(msg);
		if (!iswriting)
		{
			do_write();
		}
	}

	void broadcast_msg_to_client(const chat_room_msg& msg)
	{
		//m_chatroom.broadcast(msg);
	}

	void do_write()
	{
		chat_room_msg sendbackmsg = m_send_queue.front();
		auto sendbuf = sendbackmsg.get_buf();
		asio::async_write(m_tcp_socket, asio::buffer(sendbuf, sendbuf.size()),
			[&](const asio::error_code& ec, std::size_t bytes) {
				if (ec)
				{
					handle_error(ec);
					return;
				}
				m_send_queue.pop_front();
				if (!m_send_queue.empty())
				{
					do_write();
				}
			}
		);
	}

	chat_room_msg build_sendback_msg(const std::string& msg)
	{
		char buf[4096] = { 0 };
		int32_t sendbacksize = snprintf(buf, 4096, "guest(%d):%s", get_id(), msg.c_str());
		chat_room_msg sendbackmsg;
		sendbackmsg.set_content(buf, sendbacksize);
		return sendbackmsg;
	}

	virtual void handle_error(const asio::error_code& ec) override
	{
		xsix::net::TCPConnection::handle_error(ec);
		if (ec == asio::error::eof ||
			ec == asio::error::connection_reset)
		{
			std::cout << "[Connection] client closed : " << get_id() << std::endl;
		}
	}

private:
	chat_room_msg	m_tmp_msg;
	xsix::ThreadSafeList<chat_room_msg> m_send_queue;
};
using ConnectionPtr = std::shared_ptr<Connection>;


class ChatRoomServer : public xsix::net::TCPServer<Connection>
{
public:

	ChatRoomServer(asio::io_context& ctx, uint16_t port) :
		xsix::net::TCPServer<Connection>(ctx, port) {}

public:

	void broadcast(const chat_room_msg& msg)
	{
		for (auto it = m_tcp_conn_map.begin(); it != m_tcp_conn_map.end(); ++it)
		{
			if (it->second->is_valid())
			{
				it->second->send_msg_to_client(msg);
			}
		}
	}

public:

	virtual void handle_accept_new_connection(ConnectionPtr connptr) override
	{
		xsix::net::TCPServer<Connection>::handle_accept_new_connection(connptr);
		std::cout << "[ChatRoomServer] welcome member : " << connptr->get_id() << std::endl;
	}

	virtual void handle_remove_connection(ConnectionPtr connptr) override
	{
		xsix::net::TCPServer<Connection>::handle_remove_connection(connptr);
		std::cout << "[ChatRoomServer] leave member: " << connptr->get_id() << std::endl;
	}
};
using ChatRoomServerPtr = std::shared_ptr<ChatRoomServer>;


int main()
{
	asio::io_context ctx;
	ChatRoomServerPtr server(new ChatRoomServer(ctx, 9999));
	server->start();
	server->loop();
	return 0;
}
