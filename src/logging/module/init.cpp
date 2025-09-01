#include "../internal.h"

#if _WIN32
#include <consoleapi.h>
#endif

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
    // most of this is done with unix automatically
#if _WIN32
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
        // TEST: DEFENDER BLOCK FIX
        // On Win11 this seems to trigger windows defender
        // so we're not using it, and it's not even that usefull,
        // since if the app crashes, then the console closes as well
        // + Also kinda bad UX
        // Defender detects a 'Win32/Wacapew.a!ml'
        // AllocConsole();
    }
#endif
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

    if (logger.log_file.fail())
    {
        string msg = format("Unable to open log file '%s'",
                            log_filePath.c_str());
        Debug(msg);
        return;
    }
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

    string date = get_time_str(settings.date_format.c_str());
    string logFile = format("%s/log-%s.log",
                            settings.log_dir.c_str(),
                            date.c_str());
    InitLogger(logger, logFile);
    logf("----- << %s >> -----", get_time_str().c_str());
}

void logger_dispose()
{
    logger.log_condition.notify_one();
    logger.running = false;
    logger.log_thread.join();
    FlushFileLogs(logger);
    logger.log_file.close();

    Debug("Logger disposed");
    cout << flush;
}
