#include "IPCHandler.h"

Handle pleiades;

static inline void SaltySD_printf(const char* format, ...)
{
    char buffer[256];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 256, format, args);
    va_end(args);
    
    svcOutputDebugString(buffer, strlen(buffer));
    
    FILE* f = fopen("sdmc:/SaltySD/saltysd.log", "ab");
    if (f)
    {
        fwrite(buffer, strlen(buffer), 1, f);
        fclose(f);
    }
}

Result handleServiceCmd(int cmd)
{
    SaltySD_printf("Command num: %d\n", cmd);
    return 0;
}

void serviceHandler()
{
    Result ret;
    int should_terminate = false;

    while(1)
    {
        Handle session;
        ret = svcAcceptSession(&session, pleiades);

        if(!ret)
        {
            int handle_index;
            Handle replySession = 0;

            while(1)
            {
                ret = svcReplyAndReceive(&handle_index, &session, 1, replySession, U64_MAX);

                if(should_terminate)
                    break;

                if(ret)
                    break;

                IpcParsedCommand r;
                ipcParse(&r);

                struct {
                    u64 magic;
                    u64 command;
                    u64 reserved[2];
                } *resp = r.Raw;

                handleServiceCmd(resp->command);

                if(should_terminate)
                    break;

                replySession = session;
                svcSleepThread(1000*1000);
            }

            svcCloseHandle(session);
        }

        if(should_terminate)
            break;

        svcSleepThread(1000*1000*100);
    }
}