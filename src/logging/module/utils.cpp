#include "../internal.h"

string format(const string message, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, message);
    vsprintf_s(buffer, message.c_str(), args);
    va_end(args);

    string formatted(buffer);
    return formatted;
}

string get_time_str(const char* format)
{
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);

    tm time_info;
    localtime_s(&time_info, &now_c);

    ostringstream oss;
    oss << put_time(&time_info, format);
    return oss.str();
}
