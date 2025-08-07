#pragma once

#include "imports.h"
#include "module.h"

struct Logger
{
    ofstream log_file;
    vector<string> log_buffer;
    mutex log_mutex;
    condition_variable log_condition;
    thread log_thread;

    bool running;
};

extern Logger logger;
extern LogSettings logSettings;

string format(const string message, ...);
string get_time_str(const char* format = "%Y-%m-%d %H:%M:%S");
