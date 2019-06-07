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
    openOnlineManual();

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

