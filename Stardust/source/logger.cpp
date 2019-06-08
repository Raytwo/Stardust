#include <string.h>
#include <stdarg.h>
#include "logger.h"
#include "nn/util.h"

bool Logger::isInitialized = false;
const char* Logger::path = "";

Result Logger::Initialize(const char* filepath) {
    if(isInitialized)
        return 0;

    path = filepath;
    nn::fs::CreateFile(path, 0, 0);
    isInitialized = true;
    return 0;
}

Result Logger::Log(const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    nn::util::VSNPrintf(buffer, 256, format, args);
    va_end(args);

    s64 filesize;
    nn::fs::FileHandle file;
    //Open file with AW permissions
    nn::fs::OpenFile(&file, path, 6);
    nn::fs::GetFileSize(&filesize, file);

    nn::fs::WriteOption flags;
    flags.flush = true;
    nn::fs::WriteFile(file, filesize, buffer, strlen(buffer), flags);

    nn::fs::CloseFile(file);
    return 0;
}