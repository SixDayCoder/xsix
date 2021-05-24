#pragma once
#include "xsix/common_define.h"
#include <iostream>
#include <random>
#include <atomic>

struct chat_room_msg_header
{
	uint16_t contentsize = 0;
};

struct chat_room_msg
{
public:

	chat_room_msg() 
	{
		clear();
	}

	void clear()
	{
		header.contentsize = 0;
		memset(content, 0, 1024);
	}

	chat_room_msg(const chat_room_msg& msg)
	{
		clear();
		this->header = msg.header;
		memcpy(this->content, msg.content, 1024);
	}
	
	chat_room_msg& operator=(const chat_room_msg& msg)
	{
		if (this != &msg)
		{
			clear();
			this->header = msg.header;
			memcpy(this->content, msg.content, 1024);
		}
		return *this;
	}

	chat_room_msg_header header;
	char content[1024];

public:

	static chat_room_msg get_random_msg()
	{
		std::string str = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

		std::random_device rdv;
		static std::default_random_engine e{ rdv() };
		static std::uniform_int_distribution<uint32_t> u1(10, 20);
		static std::uniform_int_distribution<uint32_t> u2(0, 60);

		auto cnt = u1(e) + 1;
		std::string result("");
		for (int32_t i = 0; i < cnt; ++i)
		{
			int32_t index = u2(e);
			result.push_back(str[index]);
		}
		
		chat_room_msg msg;
		msg.set_content(result.c_str(), result.length());
		return msg;
	}

public:

	void set_content(const char* src, int32_t size)
	{
		if (!src || size < 0)
		{
			std::cout << "set_content fail!" << std::endl;
			return;
		}
		memcpy(content, src, size);
		header.contentsize = (uint16_t)size;
	}

	void write_to_buf(char* buf, int32_t* bytes) const
	{
		uint16_t size = htons(header.contentsize);
		memcpy(buf, &size, sizeof(size));	
		memcpy(buf + sizeof(chat_room_msg_header), content, header.contentsize);
		*bytes = sizeof(chat_room_msg_header) + header.contentsize;
	}

	friend std::ostream& operator<<(std::ostream& out, const chat_room_msg& msg)
	{
		out << "content : " << msg.content << " size : " << msg.header.contentsize;
		return out;
	}
};