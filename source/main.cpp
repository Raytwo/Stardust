#include <switch.h>
#include "ModuleObject.hpp"
#include "nn/fs.h"
#include "logger.h"

__attribute__((section(".bss"))) rtld::ModuleObject __nx_module_runtime;

u64 BASEADDR;

#define FunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) \
RETURN_TYPE (*NAME)ARGS = (RETURN_TYPE (*)ARGS)ADDRESS + BASEADDR

#define DataPointer(type, name, address) \
type &name = *(type *)(address + BASEADDR)

//Stardust's address relative to main: 0x22E4000
//Stardust's address relative to subsdk0: 0xA89000‬

u64 getBaseAddress()
{
    if (BASEADDR)
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

    //Call a method from the game as a PoC
    FunctionPointer(s64, openOnlineManual, (), 0x2808d8);
    result = openOnlineManual();
    Logger::Log("Return value from openOnlineManual: 0x%08X\n", result);
    DataPointer(char, dunno, 0x432408);
    Logger::Log("Value of dunno: %d\n", dunno);
    return 0;
}

//Current hook entrypoint: 0x2983b0
int main()
{
    getBaseAddress();

    if(stardustInit())
        return -1;

    nn::fs::Unmount("sdmc");
    return 0;
}

