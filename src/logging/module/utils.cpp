#include "../internal.h"

string format(const string message, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, message);
// TODO: PLATFORM - proper separation
#ifdef _WIN32
    vsprintf_s(buffer, message.c_str(), args);
#else
    vsprintf(buffer, message.c_str(), args);
#endif
    va_end(args);

    string formatted(buffer);
    return formatted;
}

string get_time_str(const char* format)
{
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);

    tm time_info;

// TODO: PLATFORM - proper separation
#ifdef _WIN32
    localtime_s(&time_info, &now_c);
#else
    time_info = *localtime(&now_c);
#endif

    ostringstream oss;
    oss << put_time(&time_info, format);
    return oss.str();
}
