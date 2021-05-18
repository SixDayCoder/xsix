#include "chat_room_msg.hpp"
#include "xsix/actor/actor_core.h"
#include "xsix/network/tcp_client.h"

#include <iostream>
#include <functional>
#include <chrono>
#include <random>
#include <atomic>
using namespace std;

std::thread generate_msg_thread;
std::atomic_bool stop_random(false);

std::string random_str()
{
	std::string str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	std::random_device rdv;	
	static std::default_random_engine e{ rdv() };
	static std::uniform_int_distribution<uint32_t> u1(20, 80);
	static std::uniform_int_distribution<uint32_t> u2(0, str.length());

	auto cnt = u1(e);
	std::string result;
	for (int32_t i = 0; i < cnt; ++i)
	{
		int32_t index = u2(e);
		result.push_back(str[index]);
	}
	return result;
}

void generate_msg(xsix::TCPClientPtr client)
{
	std::default_random_engine e;
	std::uniform_int_distribution<uint32_t> u(1, 3);
	while (!stop_random.load())
	{
		std::string s = random_str();
		
		chat_room_msg msg;
		msg.set_content(s.c_str(), s.length());

		int32_t bytes = 0;
		char sendbuf[1024] = { 0 };
		msg.write_to_buf(sendbuf, &bytes);

		client->send(sendbuf, bytes);
		std::this_thread::sleep_for(std::chrono::seconds(u(e)));
	}
}

void connected(xsix::TCPClientPtr ptr)
{
	if (!ptr)
	{
		return;
	}
	cout << "connected ready input!" << endl;
	generate_msg_thread = std::thread(std::bind(generate_msg, ptr));
}

void chat(xsix::TCPClientPtr conn)
{
	if (!conn)
	{
		return;
	}
	
	if (conn->m_recv_buffer.length() < sizeof(chat_room_msg_header))
	{
		return;
	}

	//parse message
	std::string s = conn->m_recv_buffer.retrieve_all_as_string();
	conn->m_recv_buffer.clear();

	chat_room_msg msg;
	memcpy(&msg.header, s.c_str(), sizeof(chat_room_msg_header));

	uint16_t content_length = 0;
	memcpy(&content_length, &msg.header, sizeof(uint16_t));
	content_length = ntohs(content_length);
	msg.header.size = content_length;

	if (s.length() >= content_length + sizeof(chat_room_msg_header))
	{
		memcpy(msg.content, s.c_str() + sizeof(chat_room_msg_header), content_length);
		printf("%s\n", msg.content);
	}
}

void close_handler(xsix::TCPClientPtr client)
{
	if (!client)
	{
		return;
	}
	stop_random.store(true);
}

int main()
{
	asio::io_context ctx;
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string("127.0.0.1"), 9999);
	xsix::TCPClientPtr client(new xsix::TCPClient(ctx));

	client->set_connected_handler(connected);
	client->set_message_handler(chat);
	client->set_close_handler(close_handler);
	client->connect(ep);
	client->loop();
	generate_msg_thread.join();
	printf("finished!\n");
	
	return 0;
}
