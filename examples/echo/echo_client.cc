#include "xsix/network/tcp_client.h"

int main()
{
	asio::io_context ctx;
	xsix::TCPClient client(ctx);

	asio::ip::tcp::endpoint ep(asio::ip::address::from_string("127.0.0.1"), 8888);
	//asio::ip::tcp::endpoint ep(asio::ip::address::from_string("81.70.194.171"), 8888);
	client.connect(ep);
	client.loop();
	
	return 0;
}