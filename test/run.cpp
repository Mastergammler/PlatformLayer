#include "imports.h"
#include "tests.cpp"

int main(int argc, char** argv)
{
    Clock timer = {};
    LogSettings settings = {};
    settings.log_prefix = "[Test]";
    settings.log_dir = "logs/test";
    logger_initialize(settings);
    timer_start(timer);

    int testSuccess = 0;

    // TODO: write tests into an array
    //  -> then add logging and timing to it

    int val = 0;
    for (int i = 0; i < 1000; i++)
    {
        val += i * 26;
    }

    float elapsed = time_since_start(timer);
    logf("Ran %i tests within %.3f ms", 0, elapsed);
    logger_dispose();
    return testSuccess;
}
