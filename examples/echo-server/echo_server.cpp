#include "xsix/xsix_network.h"
#include "xsix/xsix_timestamp.h"
#include <stdio.h>

int main()
{
	xsix::NetworkEnv::Init();
	xsix::ServerSocket server;
	server.SetReusePort();
	XASSERT(server.Bind(8888));
	XASSERT(server.Listen());
	printf("listen start\n");
	while (true)
	{
		xsix::NetAddr addr;
		SOCKET fd = server.Accept(&addr);
		if (fd != INVALID_SOCKET)
		{
			printf("coming new conn (%s:%d)\n", addr.IP().c_str(), addr.Port());
			xsix::Socket sock(fd);
			char buffer[1024] = { 0 };
			int32_t nRecv = sock.RecvBytes(buffer, 1024);
			if (nRecv != SOCKET_ERROR)
			{
				if (nRecv == 0)
				{
					printf("client socket close\n");
					sock.Close();
				}
				else
				{
					printf("recv from (%s:%d) %d bytes, content is %s\n", addr.IP().c_str(), addr.Port(), nRecv, buffer);
					std::string reply = "echo " + std::string(buffer);
					sock.SendBytes(reply.c_str(), reply.size());
					sock.Close();
				}
			}
		}
		xsix::Timestamp now = xsix::Timestamp::Now();
		printf("now time : %d\n", now.UnixMills());
	}

	xsix::NetworkEnv::Clean();
	getchar();
	return 0;
}