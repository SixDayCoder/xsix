#include "xsix/xsix_network.h"
#include <stdio.h>

int main()
{
	xsix::NetworkEnv::Init();

	xsix::NetAddr addr("127.0.0.1", 8888);
	xsix::ClientSocket client;
	printf("client ready connect (%s:%d)\n", addr.IP().c_str(), addr.Port());
	XASSERT(client.Connect(addr));
	client.SendBytes("helloxsix", 9);
	char recv[1024] = { 0 };
	if (client.RecvBytes(recv, 1024))
	{
		printf("receive : %s\n", recv);
		client.Close();
	}
	xsix::NetworkEnv::Clean();
	getchar();
	return 0;
}