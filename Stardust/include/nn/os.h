#pragma once

#include "types.h"

typedef void(*func_ptr_t)(void *);

namespace nn {

  typedef u128 TimeSpan; //This doesn't work so far, don't mind it.

  namespace os {
    typedef s32 MemoryPermission;

    namespace detail {
        Result SetMemoryPermission(u64, u64, nn::os::MemoryPermission);
    };

    typedef struct ThreadType { uint8_t x[0x1D0]; } ThreadType;


    Result CreateThread(nn::os::ThreadType* handle, func_ptr_t function, void *arg, void *stack, u64 priority, s32 proc_id);
    Result CreateThread(nn::os::ThreadType* handle, func_ptr_t function, void *arg, void *stack_bottom, size_t stack_size, int priority, int ideal_core);
    Result StartThread(nn::os::ThreadType* handle);
    Result WaitThread(nn::os::ThreadType* handle);
    Result SleepThread(nn::TimeSpan nanoseconds);
  };
};