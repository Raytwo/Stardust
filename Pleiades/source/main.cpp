#include <switch.h>
#include <stratosphere.hpp>

#include "service.hpp"

extern "C" {
    extern u32 __start__;

    u32 __nx_applet_type = AppletType_None;

    static char heap[0x4000];

    void __libnx_initheap();
    void __appInit();
    void __appExit();
}

void __libnx_initheap(void)
{
	extern char* fake_heap_start;
	extern char* fake_heap_end;

	fake_heap_start = &heap[0];
	fake_heap_end   = &heap[sizeof(heap)];
}

void  __appInit(void)
{
    SetFirmwareVersionForLibnx();

    DoWithSmSession([&]() {
        R_ASSERT(fsInitialize());
    });

    R_ASSERT(fsdevMountSdmc());
}

void  __appExit(void)
{
    fsdevUnmountAll();
    fsExit();
}

int main(int argc, char* argv[])
{
    static auto s_server_manager = WaitableManager(1);
    s_server_manager.AddWaitable(new ServiceServer<Pleiades>("pleiades", 4));
    
    s_server_manager.Process();

    return 0;
}
