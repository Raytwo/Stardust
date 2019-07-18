#include <switch.h>
#include "service.hpp"

#define BRANCH_UNCONDITIONAL 0x14000000
#define BRANCH_LINK 0x94000000

Result Pleiades::GetArbitraryValue(Out<u32> number) {
	number.SetValue(0x69);
	
	return ResultSuccess;
}

Result Pleiades::memcpy(u64 dest, u64 src, u64 size, u64 process_id) {
	Result ret;
	Handle debug;
	ret = svcDebugActiveProcess(&debug, process_id);
	if(!ret) {
		void* tmp = malloc(0x1);
		*((u8*)tmp) = 0x05;
		ret = svcWriteDebugProcessMemory(debug, tmp, dest, 0x1);
		*((u8*)tmp) = 0x14;
		ret = svcWriteDebugProcessMemory(debug, tmp, dest+ 0x3, 0x1);
	}
	svcCloseHandle(debug);
	return ResultSuccess;
}

Result Pleiades::WriteBranch(u64 writeaddress, u64 funcaddress, u64 process_id) {
	Handle debug;
	Result ret;
	char instruction[4];

	u32 relative = (s32)(funcaddress - (writeaddress + 0x4)) >> 2;
	*(u32*)instruction = BRANCH_UNCONDITIONAL| (relative & 0x3FFFFFF);

	ret = svcDebugActiveProcess(&debug, process_id);
	if(!ret) {
		ret = svcWriteDebugProcessMemory(debug, instruction, writeaddress, sizeof(instruction));
	}
	svcCloseHandle(debug);

	return ResultSuccess;
}

Result Pleiades::WriteBranchLink(u64 writeaddress, u64 funcaddress, u64 process_id) {
	Handle debug;
	Result ret;
	char instruction[4];

	u32 relative = (s32)(funcaddress - (writeaddress + 0x4)) >> 2;
	*(u32*)instruction = BRANCH_LINK| (relative & 0x3FFFFFF);

	ret = svcDebugActiveProcess(&debug, process_id);
	if(!ret) {
		ret = svcWriteDebugProcessMemory(debug, instruction, writeaddress, sizeof(instruction));
	}
	svcCloseHandle(debug);

	return ResultSuccess;
}