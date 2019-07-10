#pragma once

#include <switch.h>
#include "nn/os.h"
#include "nn/sf/hipc.h"
#include "logger.h"

#define CUR_PROCESS_HANDLE 0xFFFF8001
#define CUR_THREAD_HANDLE 0xFFFF8000

extern u64 BASEADDR;

//Don't work globally for now
#define FunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) \
RETURN_TYPE (*NAME)ARGS = (RETURN_TYPE (*)ARGS)ADDRESS + BASEADDR

#define OffsetFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) \
RETURN_TYPE (*NAME)ARGS = *(RETURN_TYPE (**)ARGS)ADDRESS + BASEADDR

#define DataPointer(type, name, address) \
type &name = *(type *)(address + BASEADDR)

u64 getPID();
Result getCurrentProcessHandle();
u64 getBaseAddress();
