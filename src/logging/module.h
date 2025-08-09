#pragma once

#include <string>

using std::string;

struct LogSettings
{
    string log_dir = "logs";
    string log_prefix = "[Debug]";
    string date_format = "%Y-%m-%d %H:%M:%S";
};

void logger_initialize(LogSettings settings);
void logger_dispose();

void log(const string msg);
void logf(const string msg, ...);

string format(const string message, ...);
