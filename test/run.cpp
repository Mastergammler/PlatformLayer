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

    int testCount = sizeof(TESTS) / sizeof(TESTS[0]);
    int successCount = 0;
    int failedCount = 0;

    for (int i = 0; i < testCount; i++)
    {
        timer_update(timer);
        int result = TESTS[i].func();
        float testTime = time_since_update(timer);
        if (result == 0)
        {
            logf("\033[32m[%.3f ms] %s\033[0m", TESTS[i].name, testTime);
            successCount++;
        }
        else
        {
            logf("\033[31m    [FAIL] %s (%.3f ms, res: %i) \033[0m",
                 TESTS[i].name,
                 testTime,
                 result);
            failedCount++;
        }
    }

    float elapsed = time_since_start(timer);
    log("");
    logf("Passed: %i    Failed: %i       %.3f ms",
         successCount,
         failedCount,
         elapsed);
    logger_dispose();
    return failedCount;
}
