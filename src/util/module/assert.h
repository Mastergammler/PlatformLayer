#ifndef NDEBUG
#define ASSERT(condition, message)                                             \
    if (!(condition))                                                          \
    {                                                                          \
        logf("Assertion '%s' failed at %s() in %s line:%d \n\t >> %s <<",      \
             #condition,                                                       \
             __FUNCTION__,                                                     \
             __FILE__,                                                         \
             __LINE__,                                                         \
             message);                                                         \
        abort();                                                               \
    }
#else
#define ASSERT(condition, message)                                             \
    {                                                                          \
    }
#endif
