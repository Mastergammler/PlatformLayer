#include "../../src/io/module/config_file.cpp"
#include "../../src/util/module/parsing.cpp"
#include "../imports.h"

// TODO: better tests - with actual test file etc
// - negative values currently untested
int parse_config()
{
    parse_config_file("res/config/settings.conf");
    return 0;
}
