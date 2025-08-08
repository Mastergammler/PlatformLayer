#include "internal.h"

#include <chrono>
#include <string>
#include <thread>

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

static int countdown = 10;

void game_update()
{
    if (countdown <= 0)
    {
        Log("Kaboom - engine exploded");
        engine_stop();
    }
    else
    {
        string msg = format(" - %i - ", countdown--);
        Log(msg);
        platform_window_title(msg);
    }
    sleep_for(milliseconds(1000));
}
