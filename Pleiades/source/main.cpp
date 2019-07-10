#include <switch.h>
#include <stratosphere.hpp>

u32 __nx_applet_type = AppletType_None;

static char heap[0x4000];

void __libnx_initheap(void)
{
	extern char* fake_heap_start;
	extern char* fake_heap_end;

	fake_heap_start = &heap[0];
	fake_heap_end   = &heap[sizeof(heap)];
}

// Init/exit services, update as needed.
void __attribute__((weak)) __appInit(void)
{
    SetFirmwareVersionForLibnx();
    smInitialize();
    fsInitialize();
    fsdevMountSdmc();
}

void __attribute__((weak)) userAppExit(void);

void __attribute__((weak)) __appExit(void)
{
    fsdevUnmountAll();
    fsExit();
    smExit();
}

int main(int argc, char* argv[])
{
    Result ret;
    //SaltySD_printf("Pleiades is started.");
    //ret = svcManageNamedPort(&pleiades, "Pleiades", 100);

    while(1)
    {
        /*
        if(!svcWaitSynchronizationSingle(pleiades, 1000))
        {
            serviceHandler();
        }*/

        svcSleepThread(1000*1000);
    }
    return 0;
}
