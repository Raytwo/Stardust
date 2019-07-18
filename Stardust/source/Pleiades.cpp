#include <switch.h>
#include "nn/sf/hipc.h"
#include "Pleiades.h"
#include "MemHelpers.h"

static Service g_pleiades;

Result pleiadesInitialize(void) 
{
	if(serviceIsActive(&g_pleiades))
		return 0;

	nn::svc::Handle pleiades;
	Result ret;

	nn::sf::hipc::InitializeHipcServiceResolution();
	ret = nn::sf::hipc::ConnectToHipcService(&pleiades, "pleiades");
	g_pleiades.handle = pleiades.handle;
	nn::sf::hipc::FinalizeHipcServiceResolution();

	return ret;
}

Result pleiadesWriteBranch(u64 dest, u64 func)
{
	Result rc;
    IpcCommand c;
    ipcInitialize(&c);

    struct {
        u64 magic;
        u64 cmd_id;
        u64 dest;
        u64 func_addr;
        u64 pid;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 2;
    raw->dest = dest;
    raw->func_addr = func;
    raw->pid = getPID();

    rc = serviceIpcDispatch(&g_pleiades);

    if(R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct
        {
            u64 magic;
            u64 result;
        } *resp = r.Raw;

        return resp->result;     
    }

    return -1;
}

Result pleiadesWriteBranchLink(u64 dest, u64 func)
{
	Result rc;
    IpcCommand c;
    ipcInitialize(&c);

    struct {
        u64 magic;
        u64 cmd_id;
        u64 dest;
        u64 func_addr;
        u64 pid;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 3;
    raw->dest = dest;
    raw->func_addr = func;
    raw->pid = getPID();

    rc = serviceIpcDispatch(&g_pleiades);

    if(R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct
        {
            u64 magic;
            u64 result;
        } *resp = r.Raw;

        return resp->result;     
    }

    return -1;
}