#include "ModuleObject.hpp"
#include "nn/fs.h"
#include "MemHelpers.h"
#include "logger.h"

__attribute__((section(".bss"))) rtld::ModuleObject __nx_module_runtime;

//Stardust's address relative to main: 0x22E4000
//Stardust's address relative to subsdk0: 0xA89000‬

//Mount the SDMC 
Result stardustInit() {
    Result result;

    //Check if the SD card is accessible
    if(!nn::fs::IsSdCardAccessible())
        return -1; //We can't work without the SD.

    //Mount the SD card
    if(nn::fs::MountSdCard("sdmc"))
        return -1; //The application doesn't have its permissions patched for SDCard access.

    //Create our working directory
    nn::fs::CreateDirectory("sdmc:/Stardust/");

    //Have the Logger create the logging file if it doesn't exist and use it.
    Logger::Initialize("sdmc:/Stardust/debug.log");

    //Defines pointers to those variables
    DataPointer(s32, music_volume, 0xf596e8);
    DataPointer(s32, sfx_volume, 0xf596eC);

    //Create a function pointer using a method from the game as a PoC
    FunctionPointer(s64, openOnlineManual, (), 0x2808d8);

    Logger::Log("BASEADDR value: 0x%08X\n", BASEADDR);
    Logger::Log("openOnlineManual offset: 0x%08X\n", openOnlineManual);

    nn::svc::Handle pleiades;

    nn::sf::hipc::InitializeHipcServiceResolution();
    result = nn::sf::hipc::ConnectToHipcService(&pleiades, "pleiades");
    Logger::Log("ConnectToHipcService result: 0x%08X\n", result);

    Service plei;
    plei.handle = pleiades.handle;
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
    raw->dest = (BASEADDR + 0x280900); //Ptr
    raw->func_addr = (BASEADDR + 0x280914);
    raw->pid = getPID();

    rc = serviceIpcDispatch(&plei);

    if(R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct
        {
            u64 magic;
            u64 result;
        } *resp = r.Raw;

        //Logger::Log("[Cmd 0] GetArbitraryValue result: 0x%08X\n", resp->value);        
    }
    nn::sf::hipc::FinalizeHipcServiceResolution();
    //openOnlineManual();

    return 0;
}

//Current hook entrypoint: 0x2983b0
int main()
{
    getBaseAddress();

    stardustInit();

    nn::fs::Unmount("sdmc");
    return 0;
}

