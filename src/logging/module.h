#pragma once

#include <string>

struct LogSettings
{
    std::string log_dir = "logs";
    std::string log_prefix = "[Debug]";
    std::string date_format = "%Y-%m-%d %H:%M:%S";
};

void Logger_Init(LogSettings settings);
void Logger_Dispose();

void Log(const std::string msg);
void Logf(const std::string msg, ...);

std::string format(const std::string message, ...);
