#include "../internal.h"

#ifdef _WIN32
const char* NL = "\r\n";
#else
const char* NL = "\n";
#endif

void Debug(const string msg)
{
    cout << logSettings.log_prefix << " " << msg << NL;
}

void log(const string s)
{
    Debug(s);
    {
        unique_lock<mutex> lock(logger.log_mutex);
        logger.log_buffer.push_back(s);
    }
    logger.log_condition.notify_one();
}

void logf(const string message, ...)
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
    Debug(formatted);
    {
        unique_lock<mutex> lock(logger.log_mutex);
        logger.log_buffer.push_back(formatted);
    }
    logger.log_condition.notify_one();
}
