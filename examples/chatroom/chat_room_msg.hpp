#pragma once
#include "xsix/common_define.h"
#include <iostream>

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
		memset(content, 0, sizeof(content));
	}

	chat_room_msg_header header;
	char content[1024];

public:

	void set_content(const char* src, int32_t size)
	{
		if (!src || size < 0)
		{
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