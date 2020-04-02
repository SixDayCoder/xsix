#include "xsix/xsix_tcp_client.h"
#include <stdio.h>

int main()
{
	XSIX_NETWORK_INIT;
	
	xsix::TCPClient client;	
	XASSERT(client.Connect(xsix::NetAddr("127.0.0.1", 8888)));
	
	client.PushSendBytes("hello1", 6);
	client.PushSendBytes("hello2", 6);
	client.PushSendBytes("hello3", 6);
	client.Run();

	getchar();

	XSIX_NETWORK_CLEAN;
	return 0;
}