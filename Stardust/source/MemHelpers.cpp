#include "MemHelpers.h"

u64 BASEADDR = 0;

u64 getPID()
{
    u64 pid = 0;
    svcGetProcessId(&pid, CUR_PROCESS_HANDLE);
    return pid;
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

Result getCurrentProcessHandle() {
    Result ret = 0;
    nn::svc::Handle pmdmntHandle;

    nn::sf::hipc::InitializeHipcServiceResolution();
    nn::sf::hipc::ConnectToHipcService(&pmdmntHandle, "pm:dmnt");
    nn::sf::hipc::FinalizeHipcServiceResolution();


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
    raw->pid = getPID();

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
        } *resp = r.Raw;

        ret = resp->result;

        if(R_SUCCEEDED(ret)) {
            ret = r.Handles[0];
            Logger::Log("Obtained handle: 0x%08X\n", ret);
            return ret;
        }
    }

    return -1;
}