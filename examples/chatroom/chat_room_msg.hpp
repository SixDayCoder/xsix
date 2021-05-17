#pragma once
#include "xsix/common_define.h"

struct chat_room_msg_header
{
	uint16_t size = 0;
};

struct chat_room_msg
{
public:

	chat_room_msg() 
	{
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
		header.size = (uint16_t)size;
	}

	void write_to_buf(char* buf, int32_t* bytes) const
	{
		uint16_t size = htons(header.size);	
		memcpy(buf, &size, sizeof(size));	
		memcpy(buf + sizeof(chat_room_msg_header), content, header.size);

		*bytes = sizeof(chat_room_msg_header) + header.size;
		//printf("send size(%d:%d) content(%s)\n", header.size, size, buf);
	}
};