#pragma once
#include <switch.h>
#include <stratosphere.hpp>

class Pleiades final : public IServiceObject {
private:
	enum class CommandId {
		GetArbitraryValue = 0
	};
private:
	Result GetArbitraryValue(Out<u32> number);
public:
	DEFINE_SERVICE_DISPATCH_TABLE {
		MAKE_SERVICE_COMMAND_META(Pleiades, GetArbitraryValue)
	};
};