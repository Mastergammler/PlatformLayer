#include "../src/logging/compile.cpp"
#include "../src/engine/compile.cpp"

#ifdef _WIN32
#include "../src/win32/compile.cpp"
#else
// TODO: linux platform layer
#endif
