#pragma once

#include <string>

using std::string;

struct LogSettings
{
    string log_dir = "logs";
    string log_prefix = "[Debug]";
    /**
     * The date format is used for file creation
     * Think about allowed characters on win vs unix!
     */
    string date_format = "%Y-%m-%d_%H-%M-%S";
};

void logger_initialize(LogSettings settings);
void logger_dispose();

void log(const string msg);
void logf(const string msg, ...);

string format(const string message, ...);
