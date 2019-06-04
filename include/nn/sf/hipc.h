#pragma once

#include "types.h"

namespace nn {
    namespace svc {
        struct Handle{
            u32 handle;
        };
    };

    namespace sf {
        namespace hipc {
            Result InitializeHipcServiceResolution();
            Result ConnectToHipcService(nn::svc::Handle*, const char* service);
            Result FinalizeHipcServiceResolution();
        };
    };
};