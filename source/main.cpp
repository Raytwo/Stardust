#include <switch.h>
#include "ModuleObject.hpp"
#include "nn/fs.h"
#include "logger.h"

__attribute__((section(".bss"))) rtld::ModuleObject __nx_module_runtime;

#define FunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) \
RETURN_TYPE (*NAME)ARGS = (RETURN_TYPE (*)ARGS)ADDRESS + ANCHOR_ABS 

//Stardust's address relative to main: 0x22E4000
//Stardust's address relative to subsdk0: 0xA89000‬

u64 ANCHOR_ABS;

u64 getCodeStart()
{
    if (ANCHOR_ABS)
        return ANCHOR_ABS;

    u64 addr = 0;
    while (1)
    {
        MemoryInfo info;
        u32 pageinfo;
        Result ret = svcQueryMemory(&info, &pageinfo, addr);
        
        if (info.size == 0x0034A000 && info.perm == Perm_Rx)
        {
            addr = info.addr;
            Logger::Log("getCodeStart addr: 0x%08X\n", info.addr);
            Logger::Log("getCodeStart size: 0x%08X\n", info.size);
            break;
        }

        addr = info.addr + info.size;

        if (!addr || ret) break;
    }
    
    ANCHOR_ABS = addr;
    return addr;
}

//Mount the SDMC 
Result stardustInit() {
    Result result;
    const char* tebi = "penis";
    //Check if the SD card is accessible
    nn::fs::IsSdCardAccessible();
    //Mount the SD card
    nn::fs::MountSdCard("sdmc");
    //Create our working directory
    result = nn::fs::CreateDirectory("sdmc:/Stardust/");
    Logger::Initialize("sdmc:/Stardust/debug.log");
    Logger::Log("Result from CreateDirectory: 0x%04X\n", result);
    result = getCodeStart();
    FunctionPointer(s64, openOnlineManual, (), 0x2808d8);
    result = openOnlineManual();
    Logger::Log("Return value from openOnlineManual: 0x%08X\n", result);
    return 0;
}

//Current hook entrypoint: 0x2983b0
int main()
{
    stardustInit();
    return 0;
}

