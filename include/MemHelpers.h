#pragma once
#include "nn/os.h"
#include "logger.h"

#define CUR_PROCESS_HANDLE 0xFFFF8001
#define CUR_THREAD_HANDLE 0xFFFF8000

static u64 BASEADDR = 0;
static u64 *stack = 0;
static u32 testVar = 0;

#define FunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) \
RETURN_TYPE (*NAME)ARGS = (RETURN_TYPE (*)ARGS)ADDRESS + BASEADDR

#define OffsetFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) \
RETURN_TYPE (*NAME)ARGS = *(RETURN_TYPE (**)ARGS)(ADDRESS + BASEADDR)

#define DataPointer(type, name, address) \
type &name = *(type *)(address + BASEADDR)

void serverThread() {
    testVar = 50;
    return;
}

static Result GetCurrentProcessHandle() {
    Result ret = 0;
    //Handle client, server;
    nn::os::ThreadType thread;

    FunctionPointer(void*, memalign_0, (size_t, size_t), 0x0348238);

    stack = memalign_0(0x1000, 0x4000);
    Logger::Log("Value of stack after: 0x%08X\n", stack);
    ret = nn::os::CreateThread(&thread, &serverThread, NULL, stack, 0x4000, 0, 2);
    Logger::Log("svcCreateThread result: 0x%08X\n", ret);
    ret = nn::os::StartThread(&thread);
    Logger::Log("svcStartThread result: 0x%08X\n", ret);
    nn::os::WaitThread(&thread);
    Logger::Log("TestVar value: %d\n", testVar);
    //svcCreateSession(&server, &client, 0, 0);
    return 0;
}

u64 getBaseAddress()
{
    if(BASEADDR)
        return BASEADDR;

    u64 addr = 0;
    while (1)
    {
        MemoryInfo info;
        u32 pageinfo;
        Result ret = svcQueryMemory(&info, &pageinfo, addr);
        
        //Check if we found the main code region. Yes, this is terrible. Deal with it.
        if (info.size == 0x0034A000 && info.perm == Perm_Rx)
        {
            addr = info.addr;
            break;
        }

        addr = info.addr + info.size;

        if (!addr || ret) break;
    }
    
    BASEADDR = addr;
    return addr;
}

