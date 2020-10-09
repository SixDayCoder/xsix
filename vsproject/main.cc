#include "xsix/network/network.h"
#include "xsix/time/timestamp.h"
#include <stdio.h>

int main()
{
	xsix::socketapi::network_env_init();
	printf("hello xsix\n");
	getchar();
	xsix::socketapi::network_env_cleanup();
	return 0;
}