#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <switch.h>

// Sysmodules should not use applet*.
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

// Main program entrypoint
int main(int argc, char* argv[])
{
    return 0;
}
