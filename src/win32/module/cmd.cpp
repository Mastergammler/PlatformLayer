#include "../internal.h"

void win32_read_cmd_args(vector<string>& argStore)
{
    int nArgs;
    LPWSTR* argList = CommandLineToArgvW(GetCommandLineW(), &nArgs);

    if (argList)
    {
        for (int i = 0; i < nArgs; i++)
        {
            // max arg length
            char buffer[256];
            WideCharToMultiByte(CP_ACP,
                                0,
                                argList[i],
                                -1,
                                buffer,
                                sizeof(buffer),
                                NULL,
                                NULL);
            argStore.push_back(buffer);
        }
    }
}
