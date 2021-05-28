#pragma once
#include "xsix/common_define.h"
#include <iostream>
#include <random>
#include <atomic>
#include <vector>
#include <array>

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
	}

	chat_room_msg_header   header;
	std::array<char, 1024> content;


public:

	static std::string get_random_msg()
	{
		std::string str = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

		std::random_device rdv;
		static std::default_random_engine e{ rdv() };
		static std::uniform_int_distribution<uint32_t> u1(10, 20);
		static std::uniform_int_distribution<uint32_t> u2(0, 60);

		auto cnt = u1(e) + 1;
		std::string result("");
		for (int32_t i = 0; i < 10; ++i)
		{
			int32_t index = u2(e);
			result.push_back(str[index]);
		}
	
		return result;
	}

public:

	void set_content(const char* src, int32_t size)
	{
		memcpy(&content[0], src, size);
		header.contentsize = (uint16_t)size;
	}

	std::string get_content()
	{
		std::string s;
		s.assign(&content[0], header.contentsize);
		return s;
	}

	std::vector<char> get_buf() const
	{
		char buf[4096] = { 0 };
		uint16_t size = htons(header.contentsize);
		memcpy(buf, &size, sizeof(chat_room_msg_header));
		memcpy(buf + sizeof(chat_room_msg_header), &content[0], header.contentsize);	
		return std::vector<char>(buf, buf + sizeof(chat_room_msg_header) + header.contentsize);
	}

	friend std::ostream& operator<<(std::ostream& out, const chat_room_msg& msg)
	{
		out << "content : " << msg.content.data() << " size : " << msg.header.contentsize;
		return out;
	}
};