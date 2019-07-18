#pragma once
#include <switch.h>
#include <stratosphere.hpp>

class Pleiades final : public IServiceObject {
private:
	enum class CommandId {
		GetArbitraryValue = 0,
		memcpy = 1,
		WriteBranch = 2,
		WriteBranchLink = 3,
	};
private:
	Result GetArbitraryValue(Out<u32> number);
	Result memcpy(u64 dest, u64 src, u64 size, u64 process_id);
	Result WriteBranch(u64 writeaddress, u64 funcaddress, u64 process_id);
	Result WriteBranchLink(u64 writeaddress, u64 funcaddress, u64 process_id);
public:
	DEFINE_SERVICE_DISPATCH_TABLE {
		MAKE_SERVICE_COMMAND_META(Pleiades, GetArbitraryValue),
		MAKE_SERVICE_COMMAND_META(Pleiades, memcpy),
		MAKE_SERVICE_COMMAND_META(Pleiades, WriteBranch),
		MAKE_SERVICE_COMMAND_META(Pleiades, WriteBranchLink),
	};
};