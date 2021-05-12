#include <cstdio>

#include "xsix/actor/actor_base.h"
#include "xsix/actor/actor_core.h"

class TestActor : public xsix::ActorBase
{
public:

	TestActor(int32_t id) : ActorBase(id) {}

	virtual int32_t get_tick_interval() const override { return 1000 * get_id(); }

	virtual void tick() override
	{
		ActorBase::tick();
		set_state(STATE_READY_TICK);
	}
};

int main()
{
	xsix::ActorCore core;
	core.register_actor(xsix::ActorBasePtr(new TestActor(1)));
	core.register_actor(xsix::ActorBasePtr(new TestActor(2)));
	core.register_actor(xsix::ActorBasePtr(new TestActor(3)));
	core.register_actor(xsix::ActorBasePtr(new TestActor(4)));
	core.run();
	return 0;
}