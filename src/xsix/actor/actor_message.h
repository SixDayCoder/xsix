#pragma once

#include "xsix/common_define.h"
#include <memory>

namespace xsix
{
	class ActorBase;

	class IActorMessage;
	using ActorMsgPtr = std::shared_ptr<IActorMessage>;

	class IActorMessage
	{
	public:

		virtual std::string get_name() const = 0;

		virtual void handle_msg(ActorBase& actor) = 0;

	};
}