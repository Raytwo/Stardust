#pragma once
#include <switch/kernel/ipc.h>
#include "nn/os.h"
#include "nn/sf/hipc.h"
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

static Result GetPID()
{
    u64 pid = 0;
    svcGetProcessId(&pid, CUR_PROCESS_HANDLE);
    return pid;
}

void serverThread() {
    testVar = 50;
    return;
}

static Result GetCurrentProcessHandle() {
    Result ret = 0;
    //Handle client, server;
    nn::os::ThreadType thread;
    nn::svc::Handle pmdmntHandle;

    FunctionPointer(void*, memalign_0, (size_t, size_t), 0x0348238);

    stack = memalign_0(0x1000, 0x1000);
    //ret = svcCreateSession(&server, &client, 0, 0);
    ret = nn::os::CreateThread(&thread, &serverThread, NULL, stack, 0x1000, 0, 2);
    Logger::Log("svcCreateThread result: 0x%08X\n", ret);
    ret = nn::os::StartThread(&thread);
    Logger::Log("svcStartThread result: 0x%08X\n", ret);
    nn::os::WaitThread(&thread);
    Logger::Log("TestVar value: 0x%08X\n", testVar);
    Logger::Log("pmdmntHandle value before: 0x%08X\n", pmdmntHandle);
    nn::sf::hipc::InitializeHipcServiceResolution();
    ret = nn::sf::hipc::ConnectToHipcService(&pmdmntHandle, "pm:dmnt");
    Logger::Log("ConnectToHipcService result: 0x%08X\n", ret);
    nn::sf::hipc::FinalizeHipcServiceResolution();
    Logger::Log("pmdmntHandle value before: 0x%08X\n", pmdmntHandle);

    IpcCommand c;
    ipcInitialize(&c);

    struct {
        u64 magic;
        u64 cmd_id;
        u64 pid;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 65000;
    raw->pid = GetPID();

    //dirty but oh well
    Service tmp;
    tmp.handle = pmdmntHandle.handle;
    ret = serviceIpcDispatch(&tmp);

    if(R_SUCCEEDED(ret)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
            u32 handle;
        } *resp = r.Raw;

        ret = resp->handle;
        Logger::Log("Process handle hopefully: 0x%08X\n", ret);
    }

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

