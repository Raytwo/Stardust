#pragma once

#include "types.h"

namespace nn
{
    namespace util
    {
        Result VSNPrintf(char* s, u64 n, const char* format, va_list arg);
    };
};