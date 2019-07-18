#include <switch.h>
#include "nn/sf/hipc.h"
#include "Pleiades.h"
#include "MemHelpers.h"

static Service g_pleiades;
static u64 BASEADDR;

Result pleiadesInitialize(u64 base_address) 
{
	if(serviceIsActive(&g_pleiades))
		return 0;

	nn::svc::Handle pleiades;
	Result ret;

	nn::sf::hipc::InitializeHipcServiceResolution();
	ret = nn::sf::hipc::ConnectToHipcService(&pleiades, "pleiades");
	g_pleiades.handle = pleiades.handle;
	nn::sf::hipc::FinalizeHipcServiceResolution();

	BASEADDR = base_address;

	return ret;
}

Result pleiadesWriteBranch(u64 write_address, u64 dest)
{
	if(!BASEADDR)
		return -1;

	Result rc;
    IpcCommand c;
    ipcInitialize(&c);

    struct {
        u64 magic;
        u64 cmd_id;
        u64 write_addr;
        u64 dest;
        u64 pid;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 2;
    raw->write_addr = BASEADDR + write_address;
    raw->dest = BASEADDR + dest;
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

Result pleiadesWriteBranchLink(u64 write_address, u64 dest)
{
	if(!BASEADDR)
		return -1;

	Result rc;
    IpcCommand c;
    ipcInitialize(&c);

    struct {
        u64 magic;
        u64 cmd_id;
        u64 write_addr;
        u64 dest;
        u64 pid;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 3;
    raw->write_addr = BASEADDR + write_address;
    raw->dest = BASEADDR + dest;
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