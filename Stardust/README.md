# Stardust
WIP mod loader for Sonic Mania Switch (v1.04)

## Instructions before use (this is very not user-friendly yet)
#### Make sure to use the latest version of [Atmosphère](https://github.com/Atmosphere-NX/Atmosphere).
- Patching the game's NPDM file to obtain [SdCard FS permissions](https://switchbrew.org/wiki/NPDM#FS_Access_Control) (A copy of the json file for that is located in /utils).
- Add a BL instruction to your code by replacing the instruction at 0x2983b0.
- Go to /atmosphere/titles/01009AA000FAA000/exefs/ and put your generated subsdk1 and the patched NPDM file in that directory.

### Credits and Special thanks
The contributors of [Starlight](https://github.com/shadowninja108/Starlight) (in particular shadowninja108 for putting up with my questions)  
[Thog](https://github.com/Thog): ModuleObject reimplementation.  
[ShinyQuagsire](https://github.com/shinyquagsire23): SaltyNX code.  
[jugeeya](https://github.com/jugeeya): For hints and assistance figuring out ASLR.  
[misson20000](https://github.com/misson20000): For his help with threads, IPC and nnSDK stuff.  
ReSwitched(jakibaki, SciresM, leoetlino): Answering some of my questions and helping me deal with threads.  
