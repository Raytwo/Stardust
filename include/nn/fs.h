#pragma once

#include "types.h"

namespace nn
{
    namespace fs
    {

        struct FileHandle {
            void *handle;
        };

        namespace detail
        {
            Result CheckMountName(char const *);

            struct FileAccessor {
                //0x10 - OpenMode
                //0x1D8 - Filesize?
            };
        }
        
        struct WriteOption {
            bool flush = 0;
        };

        Result CreateDirectory(char const* directorypath);
        //You need to flush prior to using it or give it as a flag to WriteFile
        Result CloseFile(nn::fs::FileHandle);
        Result ReadFile(nn::fs::FileHandle, s64 position, void* buffer, u64 filesize);
        Result FlushFile(nn::fs::FileHandle);
        Result SetFileSize(nn::fs::FileHandle, s64 size);
        Result GetFileSize(s64* size, nn::fs::FileHandle);
        Result GetFileOpenMode(nn::fs::FileHandle handle);
        Result CreateFile(char const* filepath, s64 size, int flags);
        //Uses the 3 last bit of the value to determine file permissions? xyz, x = append, y = write, z = read
        Result OpenFile(nn::fs::FileHandle*, char const* path, s32 mode);
        Result WriteFile(nn::fs::FileHandle handle, long position, void const* buffer, unsigned long size, WriteOption const &option);

        // SD
        bool IsSdCardInserted();
        bool IsSdCardAccessible();
        Result MountSdCard(char const *);
        Result Unmount(char const*);
    };
};