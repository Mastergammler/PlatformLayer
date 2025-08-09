#include "../internal.h"

static FILE* std_out = nullptr;
static FILE* std_err = nullptr;

Logger logger = {};
LogSettings logSettings;

/**
 * Attaches the output streams to the current console
 *
 * The reason i have to do this in the first place, is because winMain writes to
 * a differnt output stream by default, to the debug console usually, and not
 * the std out, when opening a windows using WinMain
 */
void InitOutputStreams(bool openConsole = false)
{
    // pipeing out to the console that starts it
    if (AttachConsole(ATTACH_PARENT_PROCESS))
    {
        freopen_s(&std_out, "CONOUT$", "w", stdout);
        freopen_s(&std_err, "CONOUT$", "w", stderr);

        cout.clear();
        cerr.clear();
        wcout.clear();
        wcerr.clear();
    }
    else if (openConsole)
    {
        AllocConsole();
    }
}

void FlushFileLogs(Logger& logger)
{
    vector<string> logsToWrite;
    {
        unique_lock<mutex> lock(logger.log_mutex);
        logger.log_condition.wait(lock, [&logger]() {
            return !logger.log_buffer.empty() || !logger.running;
        });
        logsToWrite = std::move(logger.log_buffer);
        logger.log_buffer.clear();
    }
    for (const string& log : logsToWrite)
    {
        logger.log_file << log << endl;
    }
}

void InitLogger(Logger& logger, const string& log_filePath)
{
    logger.log_file.open(log_filePath, ios::out | ios::app);
    logger.running = true;
    logger.log_thread = thread([&logger]() {
        while (logger.running)
        {
            cout << flush;
            FlushFileLogs(logger);
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    });
}

// TODO: create logging dir if non existent!
void logger_initialize(LogSettings settings)
{
    logSettings = settings;
    InitOutputStreams();

    string date = get_time_str();
    string logFile = format("%s/log-%s",
                            settings.log_dir.c_str(),
                            date.c_str());
    InitLogger(logger, logFile);
    logf("----- << %s >> -----", date.c_str());
}

void logger_dispose()
{
    logger.log_condition.notify_one();
    logger.running = false;
    logger.log_thread.join();
    FlushFileLogs(logger);
    logger.log_file.close();
}
