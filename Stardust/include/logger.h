#pragma once

#include "nn/fs.h"

class Logger {
public:
    static Result Initialize(const char* filepath);
    static Result Log(const char* format, ...);
private:
    static bool isInitialized;
    static const char* path;
};