#include "../internal.h"

#include <codecvt>
#include <locale>

string WStringToString(const wstring& wstr)
{
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(wstr);
}

string CLSIDToString(const CLSID& clsid)
{
    LPOLESTR clsidString = nullptr;
    if (FAILED(StringFromCLSID(clsid, &clsidString)))
    {
        return "<0000>";
    }

    wstring result(clsidString); // copy to wstring
    CoTaskMemFree(clsidString);  // free the allocated string by COM
    return WStringToString(result);
}

void LogAsioDrivers()
{
    vector<wstring> drivers;

    // Registry path for ASIO drivers (on 64-bit Windows this may vary)

    wchar_t registry[256] = L"SOFTWARE\\ASIO";
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, registry, 0, KEY_READ, &hKey) ==
        ERROR_SUCCESS)
    {
        DWORD i = 0;
        wchar_t name[256];
        DWORD nameSize = sizeof(name) / sizeof(name[0]);
        while (RegEnumKeyExW(hKey,
                             i,
                             name,
                             &nameSize,
                             nullptr,
                             nullptr,
                             nullptr,
                             nullptr) == ERROR_SUCCESS)

        {
            drivers.push_back(name);
            i++;
            nameSize = sizeof(name) / sizeof(name[0]);
        }
        RegCloseKey(hKey);
    }

    for (int i = 0; i < drivers.size(); i++)
    {
        wstring str = drivers[i];
        logf("[ASIO] Found driver: '%s'", WStringToString(str).c_str());
    }

    if (drivers.size() == 0)
    {
        string registryStr = string(registry, registry + wcslen(registry));
        logf("[ASIO] Found no driver entries under registry entry HKLM '%s'",
             registryStr.c_str());
        return;
    }

    wstring driverKeyPath = L"SOFTWARE\\ASIO\\" + drivers[0];
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                      driverKeyPath.c_str(),
                      0,
                      KEY_READ,
                      &hKey) != ERROR_SUCCESS)
    {
        logf("[ASIO] Failed to open driver key");
        CoUninitialize();
        return;
    }

    wchar_t clsidStr[64];
    DWORD size = sizeof(clsidStr);
    if (RegQueryValueExW(hKey,
                         L"CLSID",
                         nullptr,
                         nullptr,
                         (LPBYTE)clsidStr,
                         &size) != ERROR_SUCCESS)
    {
        logf("[ASIO] Failed to get CLSID from driver key");
        RegCloseKey(hKey);
        CoUninitialize();
        return;
    }
    RegCloseKey(hKey);
    CLSID clsid;
    if (FAILED(CLSIDFromString(clsidStr, &clsid)))
    {
        logf("[ASIO] Failed to convert CLSID string");
        CoUninitialize();
        return;
    }

    logf("[ASIO] CLSID of driver '%s' is %s",
         WStringToString(drivers[0]).c_str(),
         CLSIDToString(clsid).c_str());
}
