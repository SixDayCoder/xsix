#include <stdio.h>
#include "xsix/xsix_network.h"

int main()
{
	xsix::NetworkEnv::Init();
	printf("hello libxsix\n");
	xsix::NetworkEnv::Clean();
	getchar();
	return 0;
}