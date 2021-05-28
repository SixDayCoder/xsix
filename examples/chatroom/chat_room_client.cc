#include "chat_room_msg.hpp"
#include "xsix/network/tcp_client.hpp"
#include "xsix/thread_safe_list.hpp"

class ChatRoomClient : public xsix::net::TCPClient
{
public:

	ChatRoomClient(asio::io_context& ctx) : xsix::net::TCPClient(ctx) {}

private:

	virtual void start() override
	{
		send_random_msg();
		read_header();
	}

	void send_random_msg()
	{
		std::string s = chat_room_msg::get_random_msg();
		chat_room_msg msg;
		msg.set_content(s.c_str(), s.length());

		bool iswriting = !m_send_queue.empty();
		m_send_queue.push_back(msg);
		if (!iswriting)
		{
			do_write();
		}
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

protected:

	virtual void handle_connection_established() override
	{
		xsix::net::TCPClient::handle_connection_established();

		std::cout << "[ChatRoomClient] connection established "
			<< " local address : " << local_address()
			<< " remote address : " << remote_address() << std::endl;
	}

	virtual void handle_message() override
	{
		uint16_t real_content_size = ntohs(m_tmp_msg.header.contentsize);
		std::string msg(&m_tmp_msg.content[0], real_content_size);
		std::cout << "[ChatRoomClient] recv : " << msg << std::endl;
		send_random_msg();
	}

	virtual void handle_error(const asio::error_code& ec) override
	{
		xsix::net::TCPClient::handle_error(ec);

		if (ec == asio::error::eof ||
			ec == asio::error::connection_reset)
		{
			std::cout << "[ChatRoomClient] server closed : " << ec.message() << std::endl;
		}
		else
		{
			std::cout << "[ChatRoomClient] client error : " << ec.message() << std::endl;
		}
	}

private:

	chat_room_msg m_tmp_msg;
	xsix::ThreadSafeList<chat_room_msg> m_send_queue;
};
using ChatRoomClientPtr = std::shared_ptr<ChatRoomClient>;

int main()
{
	asio::io_context ctx;
	ChatRoomClientPtr client(new ChatRoomClient(ctx));
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string("127.0.0.1"), 9999);
	client->async_connect(ep);
	client->loop();
	return 0;
}
