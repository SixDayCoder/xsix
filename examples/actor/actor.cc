#include <cstdio>

#include "xsix/actor/actor_base.h"

int main()
{
	xsix::ActorBasePtr actor(new xsix::ActorBase(0));
	printf("hello actor!\n");
	return 0;
}